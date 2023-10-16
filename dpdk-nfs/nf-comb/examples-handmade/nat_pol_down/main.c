#include <stdlib.h>

#include "nf-log.h"
#include "nf-util.h"
#include "nf.h"
#include "nf_config.h"
#include "state.h"

struct nf_config config;
struct FlowManager *flow_manager;
struct State_pol *dynamic_ft;

bool nf_init(void) {
  flow_manager = flow_manager_allocate(
      config.start_port, config.external_addr, config.wan_device,
      config.expiration_time, config.max_flows);
  unsigned capacity = config.dyn_capacity;
  dynamic_ft = alloc_state_pol(capacity, rte_eth_dev_count_avail());
  return flow_manager != NULL AND dynamic_ft != NULL;
}

int nf_process(uint16_t device, uint8_t **buffer, uint16_t packet_length,
               vigor_time_t now, struct rte_mbuf *mbuf) {

  //NF_INFO("It is %" PRId64, now);

  flow_manager_expire(flow_manager, now);
  //NF_INFO("Flows have been expired");

  struct rte_ether_hdr *rte_ether_header = nf_then_get_rte_ether_header(buffer);
  struct rte_ipv4_hdr *rte_ipv4_header =
      nf_then_get_rte_ipv4_header(rte_ether_header, buffer);
  if (rte_ipv4_header == NULL) {
    //NF_INFO("Not IPv4, dropping");
    return device;
  }
  struct tcpudp_hdr *tcpudp_header =
      nf_then_get_tcpudp_header(rte_ipv4_header, buffer);
  if (tcpudp_header == NULL) {
    //NF_INFO("Not TCP/UDP, dropping");
    return device;
  }

  //NF_INFO("Forwarding an IPv4 packet on device %" PRIu16, device);

  policer_expire_entries(now, dynamic_ft);

  uint16_t dst_device_nat;
  uint16_t dst_device_pol;

  if (device == config.wan_device) {

    //NF_INFO("Device %" PRIu16 " is external", device);

    struct FlowId internal_flow;
    if (flow_manager_get_external(flow_manager, tcpudp_header->dst_port, now,
                                  &internal_flow)) {
      //NF_INFO("Found internal flow.");
      //LOG_FLOWID(&internal_flow, //NF_INFO);

      if (internal_flow.dst_ip != rte_ipv4_header->src_addr |
          internal_flow.dst_port != tcpudp_header->src_port |
          internal_flow.protocol != rte_ipv4_header->next_proto_id) {
        //NF_INFO("Spoofing attempt, dropping.");
        dst_device_nat = device;
      } else {
        rte_ipv4_header->dst_addr = internal_flow.src_ip;
        tcpudp_header->dst_port = internal_flow.src_port;
        dst_device_nat = internal_flow.internal_device;
      }

    } else {
      //NF_INFO("Unknown flow, dropping");
      dst_device_nat = device;
    }

    // Police incoming packets.
    bool fwd = policer_check_tb(rte_ipv4_header->dst_addr, packet_length, now,
                                dynamic_ft);
    dst_device_pol = fwd ? config.lan_device : device;

  } else {

    struct FlowId id = { .src_port = tcpudp_header->src_port,
                         .dst_port = tcpudp_header->dst_port,
                         .src_ip = rte_ipv4_header->src_addr,
                         .dst_ip = rte_ipv4_header->dst_addr,
                         .protocol = rte_ipv4_header->next_proto_id,
                         .internal_device = device };

    

    //NF_INFO("For id:");
    //LOG_FLOWID(&id, //NF_INFO);

    //NF_INFO("Device %" PRIu16 " is internal (not %" PRIu16 ")", device,
     //       config.wan_device);

    uint16_t external_port;
    if (!flow_manager_get_internal(flow_manager, &id, now, &external_port) &&
        !flow_manager_allocate_flow(flow_manager, &id, device, now,
                                    &external_port)) {
      dst_device_nat = device;
    } else {
      //NF_INFO("Forwarding from ext port:%d", external_port);

      rte_ipv4_header->src_addr = config.external_addr;
      tcpudp_header->src_port = external_port;
      dst_device_nat = config.wan_device;
    }

    dst_device_pol = device == config.lan_device ? config.wan_device : device;
  }

  if (dst_device_nat != device) {
    nf_set_rte_ipv4_udptcp_checksum(rte_ipv4_header, tcpudp_header, buffer);
    concretize_devices(&dst_device_nat, rte_eth_dev_count_avail());
  }

  // conflict resolution
  uint16_t dst_device = dst_device_nat | dst_device_pol;

  if (dst_device != device) {
    rte_ether_header->s_addr = config.device_macs[dst_device];
    rte_ether_header->d_addr = config.endpoint_macs[dst_device];
  }

  //NF_INFO("-- Nat choice: %" PRIu16, dst_device_nat);
  //NF_INFO("-- Pol choice: %" PRIu16, dst_device_pol);
  //NF_INFO("-- Sending it to  %" PRIu16, dst_device);

  return dst_device;
}