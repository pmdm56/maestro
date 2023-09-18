#pragma once

// #include "loop.h"
#include "lib/verified/double-chain.h"
#include "lib/verified/map.h"
#include "lib/verified/vector.h"
#include "lib/verified/cht.h"
#include "lib/verified/lpm-dir-24-8.h"
#include "lib/proof/coherence.h"
#include "lib/verified/vigor-time.h"
#include "utils.h"
#include "nf_config.h"
#include "nf-log.h"

struct State_pol {
  struct Map* dyn_map;
  struct Vector* dyn_keys;
  struct DoubleChain* dyn_heap;
  struct Vector* dyn_vals;
  uint32_t capacity;
  uint32_t dev_count;
};

struct State_fw {
  struct Map* fm;
  struct Vector* fv;
  struct Vector* int_devices;
  struct DoubleChain* heap;
  int max_flows;
  uint32_t fw_device;
};

struct FlowManager {
  struct State_fw *state;
  vigor_time_t expiration_time; /*seconds*/
};

struct State_pol* alloc_state_pol(uint32_t capacity, uint32_t dev_count);
struct State_fw* alloc_state_fw(int max_flows, uint32_t fw_device);

struct FlowManager *flow_manager_allocate(uint16_t fw_device,
                                          vigor_time_t expiration_time,
                                          uint64_t max_flows);

void flow_manager_allocate_or_refresh_flow(struct FlowManager *manager,
                                           struct FlowId *id,
                                           uint32_t internal_device,
                                           vigor_time_t time);
void flow_manager_expire(struct FlowManager *manager, vigor_time_t time);
bool flow_manager_get_refresh_flow(struct FlowManager *manager,
                                   struct FlowId *id, vigor_time_t time,
                                   uint32_t *internal_device);

int policer_expire_entries(vigor_time_t time, struct State_pol* dynamic_ft);
bool policer_check_tb(uint32_t dst, uint16_t size, vigor_time_t time, struct State_pol* dynamic_ft);

