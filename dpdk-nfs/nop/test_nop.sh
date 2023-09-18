#!/bin/bash

set -euo pipefail

SCRIPT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)

function cleanup {
  sudo killall nf 2>/dev/null || true
  sudo killall iperf 2>/dev/null || true
  sudo ip netns delete lan 2>/dev/null || true
  sudo ip netns delete wan 2>/dev/null || true
}
trap cleanup EXIT


function test_nop {

  sudo ./build/app/nf \
        --vdev "net_tap0,iface=test_wan" \
        --vdev "net_tap1,iface=test_lan" \
        --no-huge \
        --no-shconf -- \
        --lan 1 \
        --wan 0 \
        >/dev/null 2>/dev/null &
  NF_PID=$!

  while [ ! -f /sys/class/net/test_lan/tun_flags -o \
          ! -f /sys/class/net/test_lan/tun_flags ]; do
    echo "Waiting for NF to launch...";
    sleep 1;
  done
  sleep 2

  sudo ip netns add lan
  sudo ip link set test_lan netns lan
  sudo ip netns exec lan ifconfig test_lan up 10.0.0.1
  LAN_MAC=$(sudo ip netns exec lan ifconfig test_lan | head -n 4 | tail -n 1 | awk '{ print $2 }')

  sudo ip netns add wan
  sudo ip link set test_wan netns wan
  sudo ip netns exec wan ifconfig test_wan up 10.0.0.2
  WAN_MAC=$(sudo ip netns exec wan ifconfig test_wan | head -n 4 | tail -n 1 | awk '{ print $2 }')

  sudo ip netns exec lan arp -i test_lan -s 10.0.0.2 $WAN_MAC
  sudo ip netns exec wan arp -i test_wan -s 10.0.0.1 $LAN_MAC

  sudo ip netns exec lan ping 10.0.0.2

  sudo killall nf
  wait $NF_PID 2>/dev/null || true

  sudo ip netns delete lan
  sudo ip netns delete wan
}

make clean
make
test_nop

echo "Done."
