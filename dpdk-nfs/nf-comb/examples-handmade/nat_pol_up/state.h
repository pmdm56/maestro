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

struct State_nat {
  struct Map* fm;
  struct Vector* fv;
  struct DoubleChain* heap;
  int max_flows;
  int start_port;
  uint32_t ext_ip;
  uint32_t nat_device;
};

struct FlowManager {
  struct State_nat *state;
  uint32_t expiration_time; /*nanoseconds*/
};

struct State_nat* alloc_state(int max_flows, int start_port, uint32_t ext_ip, uint32_t nat_device);
struct FlowManager *
flow_manager_allocate(uint16_t starting_port, uint32_t nat_ip,
                      uint16_t nat_device, /* NOTE: only required for verif to
                                              show that internal != external;
                                              can be removed once "our NAT" ==
                                              router + "only NAT" */
                      uint32_t expiration_time, uint64_t max_flows);

bool flow_manager_allocate_flow(struct FlowManager *manager, struct FlowId *id,
                                uint16_t internal_device, vigor_time_t time,
                                uint16_t *external_port);
void flow_manager_expire(struct FlowManager *manager, vigor_time_t time);
bool flow_manager_get_internal(struct FlowManager *manager, struct FlowId *id,
                               vigor_time_t time, uint16_t *external_port);
bool flow_manager_get_external(struct FlowManager *manager,
                               uint16_t external_port, vigor_time_t time,
                               struct FlowId *out_flow);

struct State_pol* alloc_state_pol(uint32_t capacity, uint32_t dev_count);
int policer_expire_entries(vigor_time_t time, struct State_pol* dynamic_ft);
bool policer_check_tb(uint32_t dst, uint16_t size, vigor_time_t time, struct State_pol* dynamic_ft);

