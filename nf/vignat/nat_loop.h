#ifndef LOOP_H_INCLUDED
#define LOOP_H_INCLUDED

#include "lib/containers/double-chain.h"
#include "lib/containers/double-map.h"
#include "nat_flow.h"
#include "lib/coherence.h"
#include "lib/nf_time.h"

/*@
predicate evproc_loop_invariant(struct Map* mp,
                                struct Vector* in_vec,
                                struct DoubleChain *chp,
                                unsigned int lcore_id,
                                time_t time, int max_flows, int start_port) =
          mapp<flow_id>(mp, flow_idp, _flow_id_hash, nop_true, mapc(max_flows, ?m, ?maddr)) &*&
          vectorp<flow_id>(in_vec, flow_idp, ?ifs, ?ifaddrs) &*&
          double_chainp(?ch, chp) &*&
          map_vec_chain_coherent<flow_id>(m, ifs, ch) &*&
          true == forall2(ifs, ifaddrs, (kkeeper)(maddr)) &*&
          lcore_id == 0 &*& //<-- We are verifying for a single core.
          last_time(time) &*&
          dchain_high_fp(ch) <= time &*&
          length(ifs) == max_flows &*&
          dchain_index_range_fp(ch) == max_flows &*&
          max_flows < INT_MAX;
@*/

void loop_iteration_assumptions(struct Map** m,
                                struct Vector** v,
                                struct DoubleChain** ch,
                                unsigned int lcore_id,
                                time_t time, int max_flows, int start_port);

void loop_iteration_assertions(struct Map** m,
                               struct Vector** v,
                               struct DoubleChain** ch,
                               unsigned int lcore_id,
                               time_t time, int max_flows, int start_port);

void loop_invariant_consume(struct Map** m,
                            struct Vector** v,
                            struct DoubleChain** ch,
                            unsigned int lcore_id,
                            time_t time, int max_flows, int start_port);
/*@ requires *m |-> ?mp &*& *v |-> ?vkp &*& *ch |-> ?chp &*&
             evproc_loop_invariant(mp, vkp, chp, lcore_id,
                                   time, max_flows, start_port); @*/
/*@ ensures *m |-> mp &*& *v |-> vkp &*& *ch |-> chp; @*/

void loop_invariant_produce(struct Map** m,
                            struct Vector** v,
                            struct DoubleChain** ch,
                            unsigned int* lcore_id,
                            time_t *time, int max_flows, int start_port);
/*@ requires *m |-> ?mp &*& *v |-> ?vkp &*& *ch |-> ?chp &*&
             *lcore_id |-> _ &*&
             *time |-> _; @*/
/*@ ensures *m |-> mp &*& *v |-> vkp &*& *ch |-> chp &*&
            *lcore_id |-> ?lcid &*&
            *time |-> ?t &*&
            evproc_loop_invariant(mp, vkp, chp, lcid,
                                  t, max_flows, start_port); @*/

void loop_iteration_begin(struct Map** m,
                          struct Vector** v,
                          struct DoubleChain** ch,
                          unsigned int lcore_id,
                          time_t time, int max_flows, int start_port);

void loop_iteration_end(struct Map** m,
                        struct Vector** v,
                        struct DoubleChain** ch,
                        unsigned int lcore_id,
                        time_t time, int max_flows, int start_port);

void loop_enumeration_begin(struct Map** m,
                            struct Vector** v,
                            struct DoubleChain** ch,
                            unsigned int lcore_id,
                            time_t time, int max_flows, int start_port,
                            int cnt);
//@ requires true;
//@ ensures true;

void loop_enumeration_end(struct Map** m,
                          struct Vector** v,
                          struct DoubleChain** ch,
                          unsigned int lcore_id,
                          time_t time, int max_flows, int start_port);
//@ requires true;
//@ ensures true;

#endif//LOOP_H_INCLUDED
