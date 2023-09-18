#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

#include "stdint.h"
#include "lib/verified/vigor-time.h"
#include "lib/verified/boilerplate-util.h"
#include <stdbool.h>

struct FlowId {
  uint16_t src_port;
  uint16_t dst_port;
  uint32_t src_ip;
  uint32_t dst_ip;
  uint16_t internal_device;
  uint8_t protocol;
};


struct DynamicValue {
  uint64_t bucket_size;
  vigor_time_t bucket_time;
};

struct ip_addr {
  uint32_t addr;
};


bool DynamicValue_eq(void* a, void* b);
void DynamicValue_allocate(void* obj);
unsigned DynamicValue_hash(void* obj);
unsigned ip_addr_hash(void* obj);
bool ip_addr_eq(void* a, void* b);
void ip_addr_allocate(void* obj);

unsigned FlowId_hash(void* obj);
bool FlowId_eq(void* a, void* b);
void FlowId_allocate(void* obj);

#define LOG_FLOWID(obj, p); \
  p("{"); \
  p("src_port: %d", (obj)->src_port); \
  p("dst_port: %d", (obj)->dst_port); \
  p("src_ip: %d", (obj)->src_ip); \
  p("dst_ip: %d", (obj)->dst_ip); \
  p("internal_device: %d", (obj)->internal_device); \
  p("protocol: %d", (obj)->protocol); \
  p("}");



#define LOG_DYNAMICVALUE(obj, p); \
  p("{"); \
  p("bucket_size: %d", (obj)->bucket_size); \
  p("bucket_time: %d", (obj)->bucket_time); \
  p("}");

#define LOG_IP_ADDR(obj, p); \
  p("{"); \
  p("addr: %d", (obj)->addr); \
  p("}");

#endif //_UTILS_H_INCLUDED_
