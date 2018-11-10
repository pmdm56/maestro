#ifndef _LB_BALANCER_H_INCLUDED_ // cannot use pragma once, included by VeriFast
#define _LB_BALANCER_H_INCLUDED_


#include "lib/nf_time.h"

#include <stdbool.h>
#include <stdint.h>
#include <rte_ether.h>

#include "lib/stubs/mbuf_content.h"
#include "lib/containers/vector.h"
#include "lib/containers/double-chain.h"

#ifdef KLEE_VERIFICATION
#include <klee/klee.h>
#include "lib/stubs/containers/str-descr.h"
#endif//KLEE_VERIFICATION


struct LoadBalancedFlow {
	uint32_t src_ip;
	uint32_t dst_ip;
	uint16_t src_port;
	uint16_t dst_port;
	uint8_t protocol;
};

struct LoadBalancedBackend {
	uint16_t nic;
  struct ether_addr mac;
  uint32_t ip;
};


/*@
  inductive lb_flowi = lb_flowc(int, int, int, int, int);
  predicate lb_flowp(struct LoadBalancedFlow* ptr; lb_flowi flow) =
    struct_LoadBalancedFlow_padding(ptr) &*&
    ptr->src_ip |-> ?sip &*&
    ptr->dst_ip |-> ?dip &*&
    ptr->src_port |-> ?sp &*&
    ptr->dst_port |-> ?dp &*&
    ptr->protocol |-> ?p &*&
    flow == lb_flowc(sip, dip, sp, dp, p);

  fixpoint unsigned lb_flow_hash_2(lb_flowi ea);

  fixpoint unsigned lb_ip_hash_fp(unsigned ip);

  inductive lb_backendi = lb_backendc(int, ether_addri, int);

  predicate lb_backendp(struct LoadBalancedBackend* ptr; lb_backendi backend) =
    struct_LoadBalancedBackend_padding(ptr) &*&
    ptr->nic |-> ?i &*&
    ether_addrp(&ptr->mac, ?mac) &*&
    ptr->ip |-> ?ip &*&
    backend == lb_backendc(i, mac, ip);

@*/


bool lb_flow_equality(void* objA, void* objB);
/*@ requires [?fr1]lb_flowp(objA, ?f1) &*&
             [?fr2]lb_flowp(objB, ?f2); @*/
/*@ ensures [fr1]lb_flowp(objA, f1) &*&
            [fr2]lb_flowp(objB, f2) &*&
            (result ? (f1 == f2) : (f1 != f2)); @*/

uint32_t lb_flow_hash(void* obj);
/*@ requires [?fr]lb_flowp(obj, ?f); @*/
/*@ ensures [fr]lb_flowp(obj, f) &*&
            result == lb_flow_hash_2(f); @*/

void lb_flow_init(void* obj);
/*@ requires chars(obj, sizeof(struct LoadBalancedFlow), _); @*/
/*@ ensures lb_flowp(obj, _); @*/

void lb_backend_init(void* obj);
/*@ requires chars(obj, sizeof(struct LoadBalancedBackend), _); @*/
/*@ ensures lb_backendp(obj, _); @*/

void null_init(void* obj);
/*@ requires chars(obj, sizeof(uint32_t), _); @*/
/*@ ensures u_integer(obj, _); @*/

bool lb_ip_equality(void* objA, void* objB);
/*@ requires [?fr1]u_integer(objA, ?f1) &*&
             [?fr2]u_integer(objB, ?f2); @*/
/*@ ensures [fr1]u_integer(objA, f1) &*&
            [fr2]u_integer(objB, f2) &*&
            (result ? (f1 == f2) : (f1 != f2)); @*/

uint32_t lb_ip_hash(void* obj);
/*@ requires [?fr]u_integer(obj, ?f); @*/
/*@ ensures [fr]u_integer(obj, f) &*&
            result == lb_ip_hash_fp(f); @*/

void lb_fill_cht(struct Vector* cht,
                 int cht_height,
                 int backend_capacity);
/*@ requires vectorp<uint32_t>(cht, ?entp, ?values, ?addrs); @*/
/*@ ensures vectorp<uint32_t>(cht, entp, values, addrs); @*/

int
lb_find_preferred_available_backend(uint64_t hash, struct Vector* cht,
                                    struct DoubleChain* active_backends,
                                    uint32_t cht_height,
                                    uint32_t backend_capacity,
                                    int *chosen_backend);
/*@ requires vectorp<uint32_t>(cht, ?entp, ?values, ?addrs) &*&
             double_chainp(?ch, active_backends) &*&
             *chosen_backend |-> _; @*/
/*@ ensures vectorp<uint32_t>(cht, entp, values, addrs) &*&
            double_chainp(ch, active_backends) &*&
            *chosen_backend |-> ?chosen &*&
            (result == 0 ?
              true        :
              result == 1 &*&
              0 <= chosen &*&
              chosen < dchain_index_range_fp(ch)); @*/

struct LoadBalancer;
struct LoadBalancer* lb_allocate_balancer(uint32_t flow_capacity,
                                          uint32_t backend_capacity,
                                          uint32_t cht_height,
                                          uint32_t backend_expiration_time,
                                          uint32_t flow_expiration_time);
struct LoadBalancedBackend lb_get_backend(struct LoadBalancer* balancer, struct LoadBalancedFlow* flow, time_t now);
void lb_expire_flows(struct LoadBalancer* balancer, time_t now);
void lb_expire_backends(struct LoadBalancer* balancer, time_t now);
void lb_process_heartbit(struct LoadBalancer* balancer,
                         struct LoadBalancedFlow* flow,
                         struct ether_addr mac_addr,
                         int nic,
                         time_t now);

#ifdef KLEE_VERIFICATION
struct Map** lb_get_flow_to_flow_id(struct LoadBalancer* balancer);
struct Vector** lb_get_flow_heap(struct LoadBalancer* balancer);
struct DoubleChain** lb_get_flow_chain(struct LoadBalancer* balancer);
struct Vector** lb_get_flow_id_to_backend_id(struct LoadBalancer* balancer);
struct Vector** lb_get_backend_ips(struct LoadBalancer* balancer);
struct Vector** lb_get_backends(struct LoadBalancer* balancer);
struct Map** lb_get_ip_to_backend_id(struct LoadBalancer* balancer);
struct DoubleChain** lb_get_active_backends(struct LoadBalancer* balancer);
struct Vector** lb_get_cht(struct LoadBalancer* balancer);

extern struct str_field_descr lb_flow_fields[];
extern struct str_field_descr lb_backend_fields[];
extern struct nested_field_descr lb_backend_nested_fields[];
int lb_flow_fields_number();
int lb_backend_fields_number();
#endif//KLEE_VERIFICATION


#endif // _LB_BALANCER_H_INCLUDED_
