from state import flow_emap
EXP_TIME = 10
EXT_IP_ADDR = ext_ip
EXT_PORT = 1
if a_packet_received and EXP_TIME <= now:
    flow_emap.expire_all(now - EXP_TIME)
h3 = pop_header(tcpudp, on_mismatch=([],[]))
h2 = pop_header(ipv4, on_mismatch=([],[]))
h1 = pop_header(ether, on_mismatch=([],[]))

internal_flow_id = FlowIdc(h3.src_port, h3.dst_port, h2.saddr, h2.daddr, received_on_port, h2.npid)
if (received_on_port != EXT_PORT and
    not flow_emap.has(internal_flow_id) and
    not flow_emap.full()):
    idx = the_index_allocated
    flow_emap.add(internal_flow_id, idx, now)
    return ([EXT_PORT],
            [ether(h1, saddr=..., daddr=...),
             ipv4(h2, cksum=..., saddr=EXT_IP_ADDR),
             tcpudp(h3, src_port=idx + start_port)])
else:
    pass
