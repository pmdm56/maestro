#!/bin/bash

set -euo pipefail

SCRIPT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)

function cleanup {
  sudo killall nf 2>/dev/null || true
  sudo killall iperf 2>/dev/null || true
  sudo ip netns delete lan 2>/dev/null || true
  sudo ip netns delete wan 2>/dev/null || true
  sudo killall tcpdump 2>/dev/null || true 
}
trap cleanup EXIT


function test_nat {

MAC_0=$(printf '02:%02X:%02X:%02X:%02X:%02X\n' $((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)))
MAC_1=$(printf '04:%02X:%02X:%02X:%02X:%02X\n' $((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)))


  sudo ./build/app/nf \
        --vdev "net_tap0,iface=test_lan" \
        --vdev "net_tap1,iface=test_wan" \
        --no-huge \
        --no-shconf -- \
        --eth-dest 0,$MAC_0 \
        --eth-dest 1,$MAC_1 \
        --expire 10000000 \
        --extip "10.0.0.7" \
        --lan-dev 0 \
        --max-flows 65536 \
        --starting-port 0 \
        --wan 1 & NF_PID=$! 

  while [ ! -f /sys/class/net/test_lan/tun_flags -o \
          ! -f /sys/class/net/test_lan/tun_flags ]; do
    echo "Waiting for NF to launch...";
    sleep 1;
  done
  sleep 2

  sudo ip netns add lan
  sudo ip link set test_lan netns lan
  sudo ip netns exec lan ifconfig test_lan up 10.0.0.1
  sudo ip netns exec lan ifconfig test_lan hw ether $MAC_0

  sudo ip netns add wan
  sudo ip link set test_wan netns wan
  sudo ip netns exec wan ifconfig test_wan up 10.0.0.2
  sudo ip netns exec wan ifconfig test_wan hw ether $MAC_1

  sudo ip netns exec lan arp -i test_lan -s 10.0.0.2 $MAC_1
  sudo ip netns exec wan arp -i test_wan -s 10.0.0.1 $MAC_0

  sudo ip netns exec lan tcpdump -i test_lan -w $SCRIPT_DIR/lan.pcap & TCP_DUMP_PID_LAN=$!
  sudo ip netns exec wan tcpdump -i test_wan -w $SCRIPT_DIR/wan.pcap & TCP_DUMP_PID_WAN=$!

  #test known flow between client and external server
  sudo ip netns exec wan iperf -us -i 1 & SERVER_PID=$!

  sudo ip netns exec lan iperf -uc 10.0.0.2 -u -t 10 >/dev/null

  sudo killall iperf
  wait $SERVER_PID 2>/dev/null || true

  sudo killall nf
  wait $NF_PID 2>/dev/null || true

  sudo killall tcpdump
  wait $TCP_DUMP_PID_LAN 2>/dev/null || true
  wait $TCP_DUMP_PID_WAN 2>/dev/null || true

  sudo ip netns delete lan
  sudo ip netns delete wan
}


make clean
make
test_nat

echo "Done."
