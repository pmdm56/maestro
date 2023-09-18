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
      config.wan_device, config.expiration_time, config.max_flows);
  unsigned capacity = config.dyn_capacity;
  dynamic_ft = alloc_state_pol(capacity, rte_eth_dev_count_avail());
  return flow_manager != NULL AND dynamic_ft != NULL;
  return true;
}

int nf_process(uint16_t device, uint8_t **buffer, uint16_t packet_length,
               vigor_time_t now, struct rte_mbuf *mbuf) {

  NF_INFO("It is %" PRId64, now);

  flow_manager_expire(flow_manager, now);
  NF_INFO("Flows have been expired");

  struct rte_ether_hdr *rte_ether_header = nf_then_get_rte_ether_header(buffer);
  struct rte_ipv4_hdr *rte_ipv4_header =
      nf_then_get_rte_ipv4_header(rte_ether_header, buffer);

  if (rte_ipv4_header == NULL) {
    NF_INFO("Not IPv4, dropping");
    return device;
  }

  struct tcpudp_hdr *tcpudp_header =
      nf_then_get_tcpudp_header(rte_ipv4_header, buffer);
  if (tcpudp_header == NULL) {
    NF_INFO("Not TCP/UDP, dropping");
    return device;
  }

  policer_expire_entries(now, dynamic_ft);

  NF_INFO("Forwarding an IPv4 packet on device %" PRIu16, device);

  uint16_t dst_device_fw;
  uint16_t dst_device_pol;

  if (device == config.wan_device) {

    // Inverse the src and dst for the "reply flow"
    struct FlowId id = {
      .src_port = tcpudp_header->dst_port,
      .dst_port = tcpudp_header->src_port,
      .src_ip = rte_ipv4_header->dst_addr,
      .dst_ip = rte_ipv4_header->src_addr,
      .protocol = rte_ipv4_header->next_proto_id,
    };

    // Firewall -> Forward if its a known flow
    // Policer -> Police incoming packets

    uint32_t dst_device_long;
    if (!flow_manager_get_refresh_flow(flow_manager, &id, now,
                                       &dst_device_long)) {
      NF_INFO("-- Unknown external flow, dropping");
      dst_device_fw = device;
    } else
      dst_device_fw = dst_device_long;

    // Police incoming packets.
    bool fwd = policer_check_tb(rte_ipv4_header->dst_addr, packet_length, now,
                                dynamic_ft);
    dst_device_pol = fwd ? config.lan_device : device;

  } else {

    // Policer Simply forward outgoing packets.
    NF_INFO("-- Outgoing packet. Not policing.");

    struct FlowId id = {
      .src_port = tcpudp_header->src_port,
      .dst_port = tcpudp_header->dst_port,
      .src_ip = rte_ipv4_header->src_addr,
      .dst_ip = rte_ipv4_header->dst_addr,
      .protocol = rte_ipv4_header->next_proto_id,
    };

    flow_manager_allocate_or_refresh_flow(flow_manager, &id, device, now);

    // Firewall -> all outgoing packets from any devices are registered in
    // flowmanager Policer -> just accept outgoing packet from lan but not
    // policing
    dst_device_fw = config.wan_device;
    dst_device_pol = device == config.lan_device ? config.wan_device : device;
  }

  // conflict resolution
  uint16_t dst_device = dst_device_fw AND dst_device_pol;

  if (dst_device != device) {
    rte_ether_header->s_addr = config.device_macs[dst_device];
    rte_ether_header->d_addr = config.endpoint_macs[dst_device];
  }

  NF_INFO("-- Fw choice: %" PRIu16, dst_device_fw);
  NF_INFO("-- Pol choice: %" PRIu16, dst_device_pol);
  NF_INFO("-- Sending it to  %" PRIu16, dst_device);

  return dst_device;
}