#include "utils.h"

bool DynamicValue_eq(void* a, void* b)
{
  struct DynamicValue* id1 = (struct DynamicValue*) a;
  struct DynamicValue* id2 = (struct DynamicValue*) b;
  return (id1->bucket_size == id2->bucket_size)
     AND (id1->bucket_time == id2->bucket_time);
}

void DynamicValue_allocate(void* obj)
{
  struct DynamicValue* id = (struct DynamicValue*) obj;
  id->bucket_size = 0;
  id->bucket_time = 0;
}

unsigned DynamicValue_hash(void* obj)
{
  struct DynamicValue* id = (struct DynamicValue*) obj;
  unsigned hash = 0;
  hash = (unsigned int)(__builtin_ia32_crc32di(hash, (unsigned long long)(id->bucket_size&0xfffffffffff))&0xffffffff);
  hash = (unsigned int)(__builtin_ia32_crc32di(hash, (unsigned long long)(id->bucket_time&0xfffffffffff))&0xffffffff);
  return hash;
}

bool ip_addr_eq(void* a, void* b)
{
  struct ip_addr* id1 = (struct ip_addr*) a;
  struct ip_addr* id2 = (struct ip_addr*) b;
  return (id1->addr == id2->addr);
}

void ip_addr_allocate(void* obj)
{
  struct ip_addr* id = (struct ip_addr*) obj;
  id->addr = 0;
}

unsigned ip_addr_hash(void* obj)
{
  struct ip_addr* id = (struct ip_addr*) obj;
  unsigned hash = 0;
  hash = __builtin_ia32_crc32si(hash, id->addr);
  return hash;
}

bool FlowId_eq(void* a, void* b)
{
  struct FlowId* id1 = (struct FlowId*) a;
  struct FlowId* id2 = (struct FlowId*) b;
  return (id1->src_port == id2->src_port)
     AND (id1->dst_port == id2->dst_port)
     AND (id1->src_ip == id2->src_ip)
     AND (id1->dst_ip == id2->dst_ip)
     AND (id1->internal_device == id2->internal_device)
     AND (id1->protocol == id2->protocol);

}


void FlowId_allocate(void* obj)
{
  struct FlowId* id = (struct FlowId*) obj;
  id->src_port = 0;
  id->dst_port = 0;
  id->src_ip = 0;
  id->dst_ip = 0;
  id->internal_device = 0;
  id->protocol = 0;
}

unsigned FlowId_hash(void* obj)
{
  struct FlowId* id = (struct FlowId*) obj;
  unsigned hash = 0;
  hash = __builtin_ia32_crc32si(hash, id->src_port);
  hash = __builtin_ia32_crc32si(hash, id->dst_port);
  hash = __builtin_ia32_crc32si(hash, id->src_ip);
  hash = __builtin_ia32_crc32si(hash, id->dst_ip);
  hash = __builtin_ia32_crc32si(hash, id->internal_device);
  hash = __builtin_ia32_crc32si(hash, id->protocol);
  return hash;
}