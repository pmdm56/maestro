#include "state.h"
#include <stdlib.h>
#include "lib/verified/boilerplate-util.h"

struct State_fw* fw_state = NULL;
struct State_pol* pol_state = NULL;

struct State_fw* alloc_state_fw(int max_flows, uint32_t fw_device)
{
  if (fw_state != NULL) return fw_state;
  struct State_fw* ret = malloc(sizeof(struct State_fw));
  if (ret == NULL) return NULL;
  ret->fm = NULL;
  if (map_allocate(FlowId_eq, FlowId_hash, max_flows, &(ret->fm)) == 0) return NULL;
  ret->fv = NULL;
  if (vector_allocate(sizeof(struct FlowId), max_flows, FlowId_allocate, &(ret->fv)) == 0) return NULL;
  ret->int_devices = NULL;
  if (vector_allocate(sizeof(uint32_t), max_flows, null_init, &(ret->int_devices)) == 0) return NULL;
  ret->heap = NULL;
  if (dchain_allocate(max_flows, &(ret->heap)) == 0) return NULL;
  ret->max_flows = max_flows;
  ret->fw_device = fw_device;
  fw_state = ret;
  return ret;
}

struct State_pol* alloc_state_pol(uint32_t capacity, uint32_t dev_count)
{
  if (pol_state != NULL) return pol_state;
  struct State_pol* ret = malloc(sizeof(struct State_pol));
  if (ret == NULL) return NULL;
  ret->dyn_map = NULL;
  if (map_allocate(ip_addr_eq, ip_addr_hash, capacity, &(ret->dyn_map)) == 0) return NULL;
  ret->dyn_keys = NULL;
  if (vector_allocate(sizeof(struct ip_addr), capacity, ip_addr_allocate, &(ret->dyn_keys)) == 0) return NULL;
  ret->dyn_heap = NULL;
  if (dchain_allocate(capacity, &(ret->dyn_heap)) == 0) return NULL;
  ret->dyn_vals = NULL;
  if (vector_allocate(sizeof(struct DynamicValue), capacity, DynamicValue_allocate, &(ret->dyn_vals)) == 0) return NULL;
  ret->capacity = capacity;
  ret->dev_count = dev_count;
  pol_state = ret;
  return ret;
}


struct FlowManager *flow_manager_allocate(uint16_t fw_device,
                                          vigor_time_t expiration_time,
                                          uint64_t max_flows) {
  struct FlowManager *manager =
      (struct FlowManager *)malloc(sizeof(struct FlowManager));
  if (manager == NULL) {
    return NULL;
  }
  manager->state = alloc_state_fw(max_flows, fw_device);
  if (manager->state == NULL) {
    return NULL;
  }

  manager->expiration_time = expiration_time;

  return manager;
}

void flow_manager_allocate_or_refresh_flow(struct FlowManager *manager,
                                           struct FlowId *id,
                                           uint32_t internal_device,
                                           vigor_time_t time) {
  int index;
  if (map_get(manager->state->fm, id, &index)) {
    dchain_rejuvenate_index(manager->state->heap, index, time);
    return;
  }
  if (!dchain_allocate_new_index(manager->state->heap, &index, time)) {
    // No luck, the flow table is full, but we can at least let the
    // outgoing traffic out.
    return;
  }

  struct FlowId *key = 0;
  vector_borrow(manager->state->fv, index, (void **)&key);
  memcpy((void *)key, (void *)id, sizeof(struct FlowId));
  map_put(manager->state->fm, key, index);
  vector_return(manager->state->fv, index, key);
  uint32_t *int_dev;
  vector_borrow(manager->state->int_devices, index, (void **)&int_dev);
  *int_dev = internal_device;
  vector_return(manager->state->int_devices, index, int_dev);
}

void flow_manager_expire(struct FlowManager *manager, vigor_time_t time) {
  assert(time >= 0);  // we don't support the past
  assert(sizeof(vigor_time_t) <= sizeof(uint64_t));
  uint64_t time_u = (uint64_t)time;  // OK because of the two asserts
  vigor_time_t last_time =
      time_u - manager->expiration_time * 1000;  // us to ns
  expire_items_single_map(manager->state->heap, manager->state->fv,
                          manager->state->fm, last_time);
}

bool flow_manager_get_refresh_flow(struct FlowManager *manager,
                                   struct FlowId *id, vigor_time_t time,
                                   uint32_t *internal_device) {
  int index;
  if (map_get(manager->state->fm, id, &index) == 0) {
    return false;
  }
  uint32_t *int_dev;
  vector_borrow(manager->state->int_devices, index, (void **)&int_dev);
  *internal_device = *int_dev;
  vector_return(manager->state->int_devices, index, int_dev);
  dchain_rejuvenate_index(manager->state->heap, index, time);
  return true;
}

int policer_expire_entries(vigor_time_t time, struct State_pol* dynamic_ft) {
  assert(time >= 0); // we don't support the past
  vigor_time_t exp_time =
      VIGOR_TIME_SECONDS_MULTIPLIER * (config.burst / config.rate);
  uint64_t time_u = (uint64_t)time;
  // OK because time >= config.burst / config.rate >= 0
  vigor_time_t min_time = time_u - exp_time;

  return expire_items_single_map(dynamic_ft->dyn_heap, dynamic_ft->dyn_keys,
                                 dynamic_ft->dyn_map, min_time);
}

bool policer_check_tb(uint32_t dst, uint16_t size, vigor_time_t time, struct State_pol* dynamic_ft) {
  int index = -1;
  int present = map_get(dynamic_ft->dyn_map, &dst, &index);
  if (present) {
    dchain_rejuvenate_index(dynamic_ft->dyn_heap, index, time);

    struct DynamicValue *value = 0;
    vector_borrow(dynamic_ft->dyn_vals, index, (void **)&value);

    assert(0 <= time);
    uint64_t time_u = (uint64_t)time;
    assert(sizeof(vigor_time_t) == sizeof(int64_t));
    assert(value->bucket_time >= 0);
    assert(value->bucket_time <= time_u);
    uint64_t time_diff = time_u - value->bucket_time;
    if (time_diff <
        config.burst * VIGOR_TIME_SECONDS_MULTIPLIER / config.rate) {
      uint64_t added_tokens =
          time_diff * config.rate / VIGOR_TIME_SECONDS_MULTIPLIER;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
      vigor_note(0 <= time_diff * config.rate / VIGOR_TIME_SECONDS_MULTIPLIER);
#pragma GCC diagnostic pop
      assert(value->bucket_size <= config.burst);
      value->bucket_size += added_tokens;
      if (value->bucket_size > config.burst) {
        value->bucket_size = config.burst;
      }
    } else {
      value->bucket_size = config.burst;
    }
    value->bucket_time = time_u;

    bool fwd = false;
    if (value->bucket_size > size) {
      value->bucket_size -= size;
      fwd = true;
    }

    vector_return(dynamic_ft->dyn_vals, index, value);

    return fwd;
  } else {
    if (size > config.burst) {
      NF_DEBUG("  Unknown flow with packet larger than burst size. Dropping.");
      return false;
    }

    int allocated =
        dchain_allocate_new_index(dynamic_ft->dyn_heap, &index, time);
    if (!allocated) {
      NF_DEBUG("No more space in the policer table");
      return false;
    }
    uint32_t *key;
    struct DynamicValue *value = 0;
    vector_borrow(dynamic_ft->dyn_keys, index, (void **)&key);
    vector_borrow(dynamic_ft->dyn_vals, index, (void **)&value);
    *key = dst;
    value->bucket_size = config.burst - size;
    value->bucket_time = time;
    map_put(dynamic_ft->dyn_map, key, index);
    // the other half of the key is in the map
    vector_return(dynamic_ft->dyn_keys, index, key);
    vector_return(dynamic_ft->dyn_vals, index, value);

    NF_DEBUG("  New flow. Forwarding.");
    return true;
  }
}