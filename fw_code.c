bool FlowId_eq(void* a, void* b) ;
uint32_t FlowId_hash(void* obj) ;
void FlowId_allocate(void* obj) ;
void null_init(void* obj) ;
struct Map* map;
struct Vector* vector;
struct Vector* vector_1;
struct DoubleChain* dchain;

bool nf_init() {
  int map_allocation_succeeded__1 = map_allocate(FlowId_eq, FlowId_hash, 65536u, &map);

  // 114
  // 115
  // 116
  // 117
  if (map_allocation_succeeded__1) {
    int vector_alloc_success__4 = vector_allocate(16u, 65536u, FlowId_allocate, &vector);

    // 114
    // 115
    // 116
    if (vector_alloc_success__4) {
      int vector_alloc_success__7 = vector_allocate(4u, 65536u, null_init, &vector_1);

      // 114
      // 115
      if (vector_alloc_success__7) {
        int is_dchain_allocated__10 = dchain_allocate(65536u, &dchain);

        // 114
        if (is_dchain_allocated__10) {
          return 1;
        }

        // 115
        else {
          return 0;
        } // !is_dchain_allocated__10

      }

      // 116
      else {
        return 0;
      } // !vector_alloc_success__7

    }

    // 117
    else {
      return 0;
    } // !vector_alloc_success__4

  }

  // 118
  else {
    return 0;
  } // !map_allocation_succeeded__1

}

int nf_process(uint16_t device, uint8_t* packet, uint16_t packet_length, int64_t now) {
  int number_of_freed_flows__27 = expire_items_single_map(dchain, vector, map, now - 100000000000ul);
  struct rte_ether_hdr* ether_header_1 = (struct rte_ether_hdr*)(packet);

  // 120
  // 121
  // 122
  // 123
  // 124
  // 125
  if (0u == ((8u != ether_header_1->ether_type) | ((4294967282u + packet_length) < 20ul))) {
    struct rte_ipv4_hdr* ipv4_header_1 = (struct rte_ipv4_hdr*)(packet + 14u);

    // 120
    // 121
    // 122
    // 123
    // 124
    if (0u == ((0u == ((6u == ipv4_header_1->next_proto_id) | (17u == ipv4_header_1->next_proto_id))) | ((4294967262u + packet_length) < 4ul))) {
      struct tcpudp_hdr* tcpudp_header_1 = (struct tcpudp_hdr*)(packet + (14u + 20u));

      // 120
      // 121
      if (device) {
        uint8_t map_key[13];
        map_key[0u] = tcpudp_header_1->dst_port & 0xff;
        map_key[1u] = (tcpudp_header_1->dst_port >> 8) & 0xff;
        map_key[2u] = tcpudp_header_1->src_port & 0xff;
        map_key[3u] = (tcpudp_header_1->src_port >> 8) & 0xff;
        map_key[4u] = ipv4_header_1->dst_addr & 0xff;
        map_key[5u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
        map_key[6u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
        map_key[7u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
        map_key[8u] = ipv4_header_1->src_addr & 0xff;
        map_key[9u] = (ipv4_header_1->src_addr >> 8) & 0xff;
        map_key[10u] = (ipv4_header_1->src_addr >> 16) & 0xff;
        map_key[11u] = (ipv4_header_1->src_addr >> 24) & 0xff;
        map_key[12u] = ipv4_header_1->next_proto_id;
        int map_value_out;
        int map_has_this_key__39 = map_get(map, &map_key, &map_value_out);

        // 120
        if (0u == map_has_this_key__39) {
          // dropping
          return device;
        }

        // 121
        else {
          uint8_t* vector_value_out = 0u;
          vector_borrow(vector_1, map_value_out, (void**)(&vector_value_out));
          vector_return(vector_1, map_value_out, vector_value_out);
          dchain_rejuvenate_index(dchain, map_value_out, now);
          ether_header_1->d_addr.addr_bytes[0ul] = 1u;
          ether_header_1->d_addr.addr_bytes[1ul] = 35u;
          ether_header_1->d_addr.addr_bytes[2ul] = 69u;
          ether_header_1->d_addr.addr_bytes[3ul] = 103u;
          ether_header_1->d_addr.addr_bytes[4ul] = 137u;
          ether_header_1->d_addr.addr_bytes[5ul] = 0u;
          ether_header_1->s_addr.addr_bytes[0ul] = 0u;
          ether_header_1->s_addr.addr_bytes[1ul] = 0u;
          ether_header_1->s_addr.addr_bytes[2ul] = 0u;
          ether_header_1->s_addr.addr_bytes[3ul] = 0u;
          ether_header_1->s_addr.addr_bytes[4ul] = 0u;
          ether_header_1->s_addr.addr_bytes[5ul] = 0u;
          return 0;
        } // !(0u == map_has_this_key__39)

      }

      // 122
      // 123
      // 124
      else {
        uint8_t map_key[13];
        map_key[0u] = tcpudp_header_1->src_port & 0xff;
        map_key[1u] = (tcpudp_header_1->src_port >> 8) & 0xff;
        map_key[2u] = tcpudp_header_1->dst_port & 0xff;
        map_key[3u] = (tcpudp_header_1->dst_port >> 8) & 0xff;
        map_key[4u] = ipv4_header_1->src_addr & 0xff;
        map_key[5u] = (ipv4_header_1->src_addr >> 8) & 0xff;
        map_key[6u] = (ipv4_header_1->src_addr >> 16) & 0xff;
        map_key[7u] = (ipv4_header_1->src_addr >> 24) & 0xff;
        map_key[8u] = ipv4_header_1->dst_addr & 0xff;
        map_key[9u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
        map_key[10u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
        map_key[11u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
        map_key[12u] = ipv4_header_1->next_proto_id;
        int map_value_out;
        int map_has_this_key__61 = map_get(map, &map_key, &map_value_out);

        // 122
        // 123
        if (0u == map_has_this_key__61) {
          uint32_t new_index__64;
          int out_of_space__64 = !dchain_allocate_new_index(dchain, &new_index__64, now);

          // 122
          if (false == ((out_of_space__64) & (0u == number_of_freed_flows__27))) {
            uint8_t* vector_value_out = 0u;
            vector_borrow(vector, new_index__64, (void**)(&vector_value_out));
            vector_value_out[0u] = tcpudp_header_1->src_port & 0xff;
            vector_value_out[1u] = (tcpudp_header_1->src_port >> 8) & 0xff;
            vector_value_out[2u] = tcpudp_header_1->dst_port & 0xff;
            vector_value_out[3u] = (tcpudp_header_1->dst_port >> 8) & 0xff;
            vector_value_out[4u] = ipv4_header_1->src_addr & 0xff;
            vector_value_out[5u] = (ipv4_header_1->src_addr >> 8) & 0xff;
            vector_value_out[6u] = (ipv4_header_1->src_addr >> 16) & 0xff;
            vector_value_out[7u] = (ipv4_header_1->src_addr >> 24) & 0xff;
            vector_value_out[8u] = ipv4_header_1->dst_addr & 0xff;
            vector_value_out[9u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
            vector_value_out[10u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
            vector_value_out[11u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
            vector_value_out[12u] = ipv4_header_1->next_proto_id;
            vector_value_out[13u] = 171u;
            vector_value_out[14u] = 171u;
            vector_value_out[15u] = 171u;
            map_put(map, vector_value_out, new_index__64);
            vector_return(vector, new_index__64, vector_value_out);
            uint8_t* vector_value_out_1 = 0u;
            vector_borrow(vector_1, new_index__64, (void**)(&vector_value_out_1));
            vector_value_out_1[0u] = 0u;
            vector_value_out_1[1u] = 0u;
            vector_value_out_1[2u] = 0u;
            vector_value_out_1[3u] = 0u;
            vector_return(vector_1, new_index__64, vector_value_out_1);
            ether_header_1->d_addr.addr_bytes[0ul] = 1u;
            ether_header_1->d_addr.addr_bytes[1ul] = 35u;
            ether_header_1->d_addr.addr_bytes[2ul] = 69u;
            ether_header_1->d_addr.addr_bytes[3ul] = 103u;
            ether_header_1->d_addr.addr_bytes[4ul] = 137u;
            ether_header_1->d_addr.addr_bytes[5ul] = 1u;
            ether_header_1->s_addr.addr_bytes[0ul] = 0u;
            ether_header_1->s_addr.addr_bytes[1ul] = 0u;
            ether_header_1->s_addr.addr_bytes[2ul] = 0u;
            ether_header_1->s_addr.addr_bytes[3ul] = 0u;
            ether_header_1->s_addr.addr_bytes[4ul] = 0u;
            ether_header_1->s_addr.addr_bytes[5ul] = 0u;
            return 1;
          }

          // 123
          else {
            ether_header_1->d_addr.addr_bytes[0ul] = 1u;
            ether_header_1->d_addr.addr_bytes[1ul] = 35u;
            ether_header_1->d_addr.addr_bytes[2ul] = 69u;
            ether_header_1->d_addr.addr_bytes[3ul] = 103u;
            ether_header_1->d_addr.addr_bytes[4ul] = 137u;
            ether_header_1->d_addr.addr_bytes[5ul] = 1u;
            ether_header_1->s_addr.addr_bytes[0ul] = 0u;
            ether_header_1->s_addr.addr_bytes[1ul] = 0u;
            ether_header_1->s_addr.addr_bytes[2ul] = 0u;
            ether_header_1->s_addr.addr_bytes[3ul] = 0u;
            ether_header_1->s_addr.addr_bytes[4ul] = 0u;
            ether_header_1->s_addr.addr_bytes[5ul] = 0u;
            return 1;
          } // !(false == ((out_of_space__64) & (0u == number_of_freed_flows__27)))

        }

        // 124
        else {
          dchain_rejuvenate_index(dchain, map_value_out, now);
          ether_header_1->d_addr.addr_bytes[0ul] = 1u;
          ether_header_1->d_addr.addr_bytes[1ul] = 35u;
          ether_header_1->d_addr.addr_bytes[2ul] = 69u;
          ether_header_1->d_addr.addr_bytes[3ul] = 103u;
          ether_header_1->d_addr.addr_bytes[4ul] = 137u;
          ether_header_1->d_addr.addr_bytes[5ul] = 1u;
          ether_header_1->s_addr.addr_bytes[0ul] = 0u;
          ether_header_1->s_addr.addr_bytes[1ul] = 0u;
          ether_header_1->s_addr.addr_bytes[2ul] = 0u;
          ether_header_1->s_addr.addr_bytes[3ul] = 0u;
          ether_header_1->s_addr.addr_bytes[4ul] = 0u;
          ether_header_1->s_addr.addr_bytes[5ul] = 0u;
          return 1;
        } // !(0u == map_has_this_key__61)

      } // !device

    }

    // 125
    else {
      // dropping
      return device;
    } // !(0u == ((0u == ((6u == ipv4_header_1->next_proto_id) | (17u == ipv4_header_1->next_proto_id))) | ((4294967262u + packet_length) < 4ul)))

  }

  // 126
  else {
    // dropping
    return device;
  } // !(0u == ((8u != ether_header_1->ether_type) | ((4294967282u + packet_length) < 20ul)))

}
