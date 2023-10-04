
bool FlowId_eq(void* a, void* b) ;
uint32_t FlowId_hash(void* obj) ;
void FlowId_allocate(void* obj) ;
bool ip_addr_eq(void* a, void* b) ;
uint32_t ip_addr_hash(void* obj) ;
void ip_addr_allocate(void* obj) ;
void DynamicValue_allocate(void* obj) ;
struct Map* map;
struct Vector* vector;
struct DoubleChain* dchain;
struct Map* map_1;
struct Vector* vector_1;
struct DoubleChain* dchain_1;
struct Vector* vector_2;

bool nf_init() {
  int map_allocation_succeeded__0 = map_allocate(FlowId_eq, FlowId_hash, 65536u, &map);

  // 14
  // 15
  // 16
  // 17
  // 18
  // 19
  // 20
  if (map_allocation_succeeded__0) {
    int vector_alloc_success__2 = vector_allocate(16u, 65536u, FlowId_allocate, &vector);

    // 14
    // 15
    // 16
    // 17
    // 18
    // 19
    if (vector_alloc_success__2) {
      int is_dchain_allocated__4 = dchain_allocate(65536u, &dchain);

      // 14
      // 15
      // 16
      // 17
      // 18
      if (is_dchain_allocated__4) {
        int map_allocation_succeeded__6 = map_allocate(ip_addr_eq, ip_addr_hash, 65536u, &map_1);

        // 14
        // 15
        // 16
        // 17
        if (map_allocation_succeeded__6) {
          int vector_alloc_success__8 = vector_allocate(4u, 65536u, ip_addr_allocate, &vector_1);

          // 14
          // 15
          // 16
          if (vector_alloc_success__8) {
            int is_dchain_allocated__10 = dchain_allocate(65536u, &dchain_1);

            // 14
            // 15
            if (is_dchain_allocated__10) {
              int vector_alloc_success__12 = vector_allocate(16u, 65536u, DynamicValue_allocate, &vector_2);

              // 14
              if (vector_alloc_success__12) {
                return 1;
              }

              // 15
              else {
                return 0;
              } // !vector_alloc_success__12

            }

            // 16
            else {
              return 0;
            } // !is_dchain_allocated__10

          }

          // 17
          else {
            return 0;
          } // !vector_alloc_success__8

        }

        // 18
        else {
          return 0;
        } // !map_allocation_succeeded__6

      }

      // 19
      else {
        return 0;
      } // !is_dchain_allocated__4

    }

    // 20
    else {
      return 0;
    } // !vector_alloc_success__2

  }

  // 21
  else {
    return 0;
  } // !map_allocation_succeeded__0

}

int nf_process(uint16_t device, uint8_t* packet, uint16_t packet_length, int64_t now) {
  int number_of_freed_flows__24 = expire_items_single_map(dchain, vector, map, now - 100000000000ul);
  struct rte_ether_hdr* ether_header_1 = (struct rte_ether_hdr*)(packet);

  // 41
  // 46
  // 51
  // 73
  // 77
  // 87
  // 92
  // 97
  // 102
  // 116
  // 120
  // 130
  // 135
  // 140
  // 145
  // 161
  // 165
  // 175
  // 180
  // 185
  // 190
  // 195
  // 207
  // 210
  // 219
  // 223
  // 227
  // 231
  if (0u == ((8u != ether_header_1->ether_type) | ((4294967282u + packet_length) < 20ul))) {
    struct rte_ipv4_hdr* ipv4_header_1 = (struct rte_ipv4_hdr*)(packet + 14u);

    // 41
    // 46
    // 51
    // 73
    // 77
    // 87
    // 92
    // 97
    // 102
    // 116
    // 120
    // 130
    // 135
    // 140
    // 145
    // 161
    // 165
    // 175
    // 180
    // 185
    // 190
    if (0u == ((0u == ((6u == ipv4_header_1->next_proto_id) | (17u == ipv4_header_1->next_proto_id))) | ((4294967262u + packet_length) < 4ul))) {
      struct tcpudp_hdr* tcpudp_header_1 = (struct tcpudp_hdr*)(packet + (14u + 20u));

      // 41
      // 46
      // 51
      if (device) {
        int32_t dchain_is_index_allocated__31 = dchain_is_index_allocated(dchain, tcpudp_header_1->dst_port);

        // 41
        // 46
        if (dchain_is_index_allocated__31) {
          uint8_t* vector_value_out = 0u;
          vector_borrow(vector, tcpudp_header_1->dst_port, (void**)(&vector_value_out));
          vector_return(vector, tcpudp_header_1->dst_port, vector_value_out);

          // 41
          if (0u == (((vector_value_out[8ul] != ipv4_header_1->src_addr) | (vector_value_out[2ul] != tcpudp_header_1->src_port)) | (vector_value_out[14ul] != ipv4_header_1->next_proto_id))) {
            int checksum__36 = rte_ipv4_udptcp_cksum(ipv4_header_1, tcpudp_header_1);
            int number_of_freed_flows__37 = expire_items_single_map(dchain_1, vector_1, map_1, now - 10000000000ul);
            tcpudp_header_1->dst_port = vector_value_out[0ul];
            ipv4_header_1->hdr_checksum = checksum__36 & 0xffff;
            ipv4_header_1->dst_addr = vector_value_out[4ul];
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
          }

          // 46
          else {
            int number_of_freed_flows__42 = expire_items_single_map(dchain_1, vector_1, map_1, now - 10000000000ul);
            return 0;
          } // !(0u == (((vector_value_out[8ul] != ipv4_header_1->src_addr) | (vector_value_out[2ul] != tcpudp_header_1->src_port)) | (vector_value_out[14ul] != ipv4_header_1->next_proto_id)))

        }

        // 51
        else {
          int number_of_freed_flows__47 = expire_items_single_map(dchain_1, vector_1, map_1, now - 10000000000ul);
          return 0;
        } // !dchain_is_index_allocated__31

      }

      // 73
      // 77
      // 87
      // 92
      // 97
      // 102
      // 116
      // 120
      // 130
      // 135
      // 140
      // 145
      // 161
      // 165
      // 175
      // 180
      // 185
      // 190
      else {
        uint8_t map_key[15];
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
        map_key[12u] = 0u;
        map_key[13u] = 0u;
        map_key[14u] = ipv4_header_1->next_proto_id;
        int map_value_out;
        int map_has_this_key__52 = map_get(map, &map_key, &map_value_out);

        // 73
        // 77
        // 87
        // 92
        // 97
        // 102
        // 116
        // 120
        // 130
        // 135
        // 140
        // 145
        if (0u == map_has_this_key__52) {
          uint32_t new_index__54;
          int out_of_space__54 = !dchain_allocate_new_index(dchain, &new_index__54, now);

          // 73
          // 77
          // 87
          // 92
          // 97
          // 102
          if (false == ((out_of_space__54) & (0u == number_of_freed_flows__24))) {
            uint8_t* vector_value_out = 0u;
            vector_borrow(vector, new_index__54, (void**)(&vector_value_out));
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
            vector_value_out[12u] = 0u;
            vector_value_out[13u] = 0u;
            vector_value_out[14u] = ipv4_header_1->next_proto_id;
            vector_value_out[15u] = 171u;
            map_put(map, vector_value_out, new_index__54);
            vector_return(vector, new_index__54, vector_value_out);
            int checksum__59 = rte_ipv4_udptcp_cksum(ipv4_header_1, tcpudp_header_1);
            int number_of_freed_flows__60 = expire_items_single_map(dchain_1, vector_1, map_1, now - 10000000000ul);
            uint8_t map_key_1[4];
            map_key_1[0u] = ipv4_header_1->dst_addr & 0xff;
            map_key_1[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
            map_key_1[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
            map_key_1[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
            int map_value_out_1;
            int map_has_this_key__61 = map_get(map_1, &map_key_1, &map_value_out_1);

            // 73
            // 77
            if (0u == map_has_this_key__61) {
              uint32_t new_index__63;
              int out_of_space__63 = !dchain_allocate_new_index(dchain_1, &new_index__63, now);

              // 73
              if (false == ((out_of_space__63) & (0u == number_of_freed_flows__60))) {
                uint8_t* vector_value_out_1 = 0u;
                vector_borrow(vector_1, new_index__63, (void**)(&vector_value_out_1));
                vector_value_out_1[0u] = ipv4_header_1->dst_addr & 0xff;
                vector_value_out_1[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
                vector_value_out_1[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
                vector_value_out_1[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
                uint8_t* vector_value_out_2 = 0u;
                vector_borrow(vector_2, new_index__63, (void**)(&vector_value_out_2));
                vector_value_out_2[0u] = 10000000000ul - packet_length;
                vector_value_out_2[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
                vector_value_out_2[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
                vector_value_out_2[3u] = 84u;
                vector_value_out_2[4u] = 2u;
                vector_value_out_2[5u] = 0u;
                vector_value_out_2[6u] = 0u;
                vector_value_out_2[7u] = 0u;
                vector_value_out_2[8u] = now & 0xff;
                vector_value_out_2[9u] = (now >> 8) & 0xff;
                vector_value_out_2[10u] = (now >> 16) & 0xff;
                vector_value_out_2[11u] = (now >> 24) & 0xff;
                vector_value_out_2[12u] = (now >> 32) & 0xff;
                vector_value_out_2[13u] = (now >> 40) & 0xff;
                vector_value_out_2[14u] = (now >> 48) & 0xff;
                vector_value_out_2[15u] = (now >> 56) & 0xff;
                map_put(map_1, vector_value_out_1, new_index__63);
                vector_return(vector_1, new_index__63, vector_value_out_1);
                vector_return(vector_2, new_index__63, vector_value_out_2);
                tcpudp_header_1->src_port = new_index__54 & 0xffff;
                ipv4_header_1->hdr_checksum = checksum__59 & 0xffff;
                ipv4_header_1->src_addr = 117440522u;
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

              // 77
              else {
                tcpudp_header_1->src_port = new_index__54 & 0xffff;
                ipv4_header_1->hdr_checksum = checksum__59 & 0xffff;
                ipv4_header_1->src_addr = 117440522u;
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
                // dropping
                return device;
              } // !(false == ((out_of_space__63) & (0u == number_of_freed_flows__60)))

            }

            // 87
            // 92
            // 97
            // 102
            else {
              dchain_rejuvenate_index(dchain_1, map_value_out_1, now);
              uint8_t* vector_value_out_1 = 0u;
              vector_borrow(vector_2, map_value_out_1, (void**)(&vector_value_out_1));
              vector_value_out_1[0u] = 10000000000ul - packet_length;
              vector_value_out_1[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
              vector_value_out_1[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
              vector_value_out_1[3u] = 84u;
              vector_value_out_1[4u] = 2u;
              vector_value_out_1[5u] = 0u;
              vector_value_out_1[6u] = 0u;
              vector_value_out_1[7u] = 0u;
              vector_value_out_1[8u] = now & 0xff;
              vector_value_out_1[9u] = (now >> 8) & 0xff;
              vector_value_out_1[10u] = (now >> 16) & 0xff;
              vector_value_out_1[11u] = (now >> 24) & 0xff;
              vector_value_out_1[12u] = (now >> 32) & 0xff;
              vector_value_out_1[13u] = (now >> 40) & 0xff;
              vector_value_out_1[14u] = (now >> 48) & 0xff;
              vector_value_out_1[15u] = (now >> 56) & 0xff;

              // 87
              // 92
              // 97
              if ((now - vector_value_out_1[8ul]) < 10000000000ul) {

                // 87
                // 92
                if (false == (10000000000ul < (vector_value_out_1[0ul] + ((1000000000ul * (now - vector_value_out_1[8ul])) / 1000000000ul)))) {

                  // 87
                  if (false == (packet_length < (vector_value_out_1[0ul] + ((1000000000ul * (now - vector_value_out_1[8ul])) / 1000000000ul)))) {
                    vector_return(vector_2, map_value_out_1, vector_value_out_1);
                    tcpudp_header_1->src_port = new_index__54 & 0xffff;
                    ipv4_header_1->hdr_checksum = checksum__59 & 0xffff;
                    ipv4_header_1->src_addr = 117440522u;
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
                    // dropping
                    return device;
                  }

                  // 92
                  else {
                    vector_return(vector_2, map_value_out_1, vector_value_out_1);
                    tcpudp_header_1->src_port = new_index__54 & 0xffff;
                    ipv4_header_1->hdr_checksum = checksum__59 & 0xffff;
                    ipv4_header_1->src_addr = 117440522u;
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
                  } // !(false == (packet_length < (vector_value_out_1[0ul] + ((1000000000ul * (now - vector_value_out_1[8ul])) / 1000000000ul))))

                }

                // 97
                else {
                  vector_return(vector_2, map_value_out_1, vector_value_out_1);
                  tcpudp_header_1->src_port = new_index__54 & 0xffff;
                  ipv4_header_1->hdr_checksum = checksum__59 & 0xffff;
                  ipv4_header_1->src_addr = 117440522u;
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
                } // !(false == (10000000000ul < (vector_value_out_1[0ul] + ((1000000000ul * (now - vector_value_out_1[8ul])) / 1000000000ul))))

              }

              // 102
              else {
                vector_return(vector_2, map_value_out_1, vector_value_out_1);
                tcpudp_header_1->src_port = new_index__54 & 0xffff;
                ipv4_header_1->hdr_checksum = checksum__59 & 0xffff;
                ipv4_header_1->src_addr = 117440522u;
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
              } // !((now - vector_value_out_1[8ul]) < 10000000000ul)

            } // !(0u == map_has_this_key__61)

          }

          // 116
          // 120
          // 130
          // 135
          // 140
          // 145
          else {
            int number_of_freed_flows__103 = expire_items_single_map(dchain_1, vector_1, map_1, now - 10000000000ul);
            uint8_t map_key_1[4];
            map_key_1[0u] = ipv4_header_1->dst_addr & 0xff;
            map_key_1[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
            map_key_1[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
            map_key_1[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
            int map_value_out_1;
            int map_has_this_key__104 = map_get(map_1, &map_key_1, &map_value_out_1);

            // 116
            // 120
            if (0u == map_has_this_key__104) {
              uint32_t new_index__106;
              int out_of_space__106 = !dchain_allocate_new_index(dchain_1, &new_index__106, now);

              // 116
              if (false == ((out_of_space__106) & (0u == number_of_freed_flows__103))) {
                uint8_t* vector_value_out = 0u;
                vector_borrow(vector_1, new_index__106, (void**)(&vector_value_out));
                vector_value_out[0u] = ipv4_header_1->dst_addr & 0xff;
                vector_value_out[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
                vector_value_out[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
                vector_value_out[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
                uint8_t* vector_value_out_1 = 0u;
                vector_borrow(vector_2, new_index__106, (void**)(&vector_value_out_1));
                vector_value_out_1[0u] = 10000000000ul - packet_length;
                vector_value_out_1[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
                vector_value_out_1[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
                vector_value_out_1[3u] = 84u;
                vector_value_out_1[4u] = 2u;
                vector_value_out_1[5u] = 0u;
                vector_value_out_1[6u] = 0u;
                vector_value_out_1[7u] = 0u;
                vector_value_out_1[8u] = now & 0xff;
                vector_value_out_1[9u] = (now >> 8) & 0xff;
                vector_value_out_1[10u] = (now >> 16) & 0xff;
                vector_value_out_1[11u] = (now >> 24) & 0xff;
                vector_value_out_1[12u] = (now >> 32) & 0xff;
                vector_value_out_1[13u] = (now >> 40) & 0xff;
                vector_value_out_1[14u] = (now >> 48) & 0xff;
                vector_value_out_1[15u] = (now >> 56) & 0xff;
                map_put(map_1, vector_value_out, new_index__106);
                vector_return(vector_1, new_index__106, vector_value_out);
                vector_return(vector_2, new_index__106, vector_value_out_1);
                return 1;
              }

              // 120
              else {
                // dropping
                return device;
              } // !(false == ((out_of_space__106) & (0u == number_of_freed_flows__103)))

            }

            // 130
            // 135
            // 140
            // 145
            else {
              dchain_rejuvenate_index(dchain_1, map_value_out_1, now);
              uint8_t* vector_value_out = 0u;
              vector_borrow(vector_2, map_value_out_1, (void**)(&vector_value_out));
              vector_value_out[0u] = 10000000000ul - packet_length;
              vector_value_out[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
              vector_value_out[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
              vector_value_out[3u] = 84u;
              vector_value_out[4u] = 2u;
              vector_value_out[5u] = 0u;
              vector_value_out[6u] = 0u;
              vector_value_out[7u] = 0u;
              vector_value_out[8u] = now & 0xff;
              vector_value_out[9u] = (now >> 8) & 0xff;
              vector_value_out[10u] = (now >> 16) & 0xff;
              vector_value_out[11u] = (now >> 24) & 0xff;
              vector_value_out[12u] = (now >> 32) & 0xff;
              vector_value_out[13u] = (now >> 40) & 0xff;
              vector_value_out[14u] = (now >> 48) & 0xff;
              vector_value_out[15u] = (now >> 56) & 0xff;

              // 130
              // 135
              // 140
              if ((now - vector_value_out[8ul]) < 10000000000ul) {

                // 130
                // 135
                if (false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {

                  // 130
                  if (false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {
                    vector_return(vector_2, map_value_out_1, vector_value_out);
                    // dropping
                    return device;
                  }

                  // 135
                  else {
                    vector_return(vector_2, map_value_out_1, vector_value_out);
                    return 1;
                  } // !(false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

                }

                // 140
                else {
                  vector_return(vector_2, map_value_out_1, vector_value_out);
                  return 1;
                } // !(false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

              }

              // 145
              else {
                vector_return(vector_2, map_value_out_1, vector_value_out);
                return 1;
              } // !((now - vector_value_out[8ul]) < 10000000000ul)

            } // !(0u == map_has_this_key__104)

          } // !(false == ((out_of_space__54) & (0u == number_of_freed_flows__24)))

        }

        // 161
        // 165
        // 175
        // 180
        // 185
        // 190
        else {
          dchain_rejuvenate_index(dchain, map_value_out, now);
          int checksum__147 = rte_ipv4_udptcp_cksum(ipv4_header_1, tcpudp_header_1);
          int number_of_freed_flows__148 = expire_items_single_map(dchain_1, vector_1, map_1, now - 10000000000ul);
          uint8_t map_key_1[4];
          map_key_1[0u] = ipv4_header_1->dst_addr & 0xff;
          map_key_1[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
          map_key_1[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
          map_key_1[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
          int map_value_out_1;
          int map_has_this_key__149 = map_get(map_1, &map_key_1, &map_value_out_1);

          // 161
          // 165
          if (0u == map_has_this_key__149) {
            uint32_t new_index__151;
            int out_of_space__151 = !dchain_allocate_new_index(dchain_1, &new_index__151, now);

            // 161
            if (false == ((out_of_space__151) & (0u == number_of_freed_flows__148))) {
              uint8_t* vector_value_out = 0u;
              vector_borrow(vector_1, new_index__151, (void**)(&vector_value_out));
              vector_value_out[0u] = ipv4_header_1->dst_addr & 0xff;
              vector_value_out[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
              vector_value_out[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
              vector_value_out[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
              uint8_t* vector_value_out_1 = 0u;
              vector_borrow(vector_2, new_index__151, (void**)(&vector_value_out_1));
              vector_value_out_1[0u] = 10000000000ul - packet_length;
              vector_value_out_1[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
              vector_value_out_1[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
              vector_value_out_1[3u] = 84u;
              vector_value_out_1[4u] = 2u;
              vector_value_out_1[5u] = 0u;
              vector_value_out_1[6u] = 0u;
              vector_value_out_1[7u] = 0u;
              vector_value_out_1[8u] = now & 0xff;
              vector_value_out_1[9u] = (now >> 8) & 0xff;
              vector_value_out_1[10u] = (now >> 16) & 0xff;
              vector_value_out_1[11u] = (now >> 24) & 0xff;
              vector_value_out_1[12u] = (now >> 32) & 0xff;
              vector_value_out_1[13u] = (now >> 40) & 0xff;
              vector_value_out_1[14u] = (now >> 48) & 0xff;
              vector_value_out_1[15u] = (now >> 56) & 0xff;
              map_put(map_1, vector_value_out, new_index__151);
              vector_return(vector_1, new_index__151, vector_value_out);
              vector_return(vector_2, new_index__151, vector_value_out_1);
              tcpudp_header_1->src_port = map_value_out & 0xffff;
              ipv4_header_1->hdr_checksum = checksum__147 & 0xffff;
              ipv4_header_1->src_addr = 117440522u;
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

            // 165
            else {
              tcpudp_header_1->src_port = map_value_out & 0xffff;
              ipv4_header_1->hdr_checksum = checksum__147 & 0xffff;
              ipv4_header_1->src_addr = 117440522u;
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
              // dropping
              return device;
            } // !(false == ((out_of_space__151) & (0u == number_of_freed_flows__148)))

          }

          // 175
          // 180
          // 185
          // 190
          else {
            dchain_rejuvenate_index(dchain_1, map_value_out_1, now);
            uint8_t* vector_value_out = 0u;
            vector_borrow(vector_2, map_value_out_1, (void**)(&vector_value_out));
            vector_value_out[0u] = 10000000000ul - packet_length;
            vector_value_out[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
            vector_value_out[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
            vector_value_out[3u] = 84u;
            vector_value_out[4u] = 2u;
            vector_value_out[5u] = 0u;
            vector_value_out[6u] = 0u;
            vector_value_out[7u] = 0u;
            vector_value_out[8u] = now & 0xff;
            vector_value_out[9u] = (now >> 8) & 0xff;
            vector_value_out[10u] = (now >> 16) & 0xff;
            vector_value_out[11u] = (now >> 24) & 0xff;
            vector_value_out[12u] = (now >> 32) & 0xff;
            vector_value_out[13u] = (now >> 40) & 0xff;
            vector_value_out[14u] = (now >> 48) & 0xff;
            vector_value_out[15u] = (now >> 56) & 0xff;

            // 175
            // 180
            // 185
            if ((now - vector_value_out[8ul]) < 10000000000ul) {

              // 175
              // 180
              if (false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {

                // 175
                if (false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {
                  vector_return(vector_2, map_value_out_1, vector_value_out);
                  tcpudp_header_1->src_port = map_value_out & 0xffff;
                  ipv4_header_1->hdr_checksum = checksum__147 & 0xffff;
                  ipv4_header_1->src_addr = 117440522u;
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
                  // dropping
                  return device;
                }

                // 180
                else {
                  vector_return(vector_2, map_value_out_1, vector_value_out);
                  tcpudp_header_1->src_port = map_value_out & 0xffff;
                  ipv4_header_1->hdr_checksum = checksum__147 & 0xffff;
                  ipv4_header_1->src_addr = 117440522u;
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
                } // !(false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

              }

              // 185
              else {
                vector_return(vector_2, map_value_out_1, vector_value_out);
                tcpudp_header_1->src_port = map_value_out & 0xffff;
                ipv4_header_1->hdr_checksum = checksum__147 & 0xffff;
                ipv4_header_1->src_addr = 117440522u;
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
              } // !(false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

            }

            // 190
            else {
              vector_return(vector_2, map_value_out_1, vector_value_out);
              tcpudp_header_1->src_port = map_value_out & 0xffff;
              ipv4_header_1->hdr_checksum = checksum__147 & 0xffff;
              ipv4_header_1->src_addr = 117440522u;
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
            } // !((now - vector_value_out[8ul]) < 10000000000ul)

          } // !(0u == map_has_this_key__149)

        } // !(0u == map_has_this_key__52)

      } // !device

    }

    // 195
    // 207
    // 210
    // 219
    // 223
    // 227
    // 231
    else {
      int number_of_freed_flows__191 = expire_items_single_map(dchain_1, vector_1, map_1, now - 10000000000ul);

      // 195
      if (device) {
        return 0;
      }

      // 207
      // 210
      // 219
      // 223
      // 227
      // 231
      else {
        uint8_t map_key[4];
        map_key[0u] = ipv4_header_1->dst_addr & 0xff;
        map_key[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
        map_key[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
        map_key[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
        int map_value_out;
        int map_has_this_key__196 = map_get(map_1, &map_key, &map_value_out);

        // 207
        // 210
        if (0u == map_has_this_key__196) {
          uint32_t new_index__198;
          int out_of_space__198 = !dchain_allocate_new_index(dchain_1, &new_index__198, now);

          // 207
          if (false == ((out_of_space__198) & (0u == number_of_freed_flows__191))) {
            uint8_t* vector_value_out = 0u;
            vector_borrow(vector_1, new_index__198, (void**)(&vector_value_out));
            vector_value_out[0u] = ipv4_header_1->dst_addr & 0xff;
            vector_value_out[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
            vector_value_out[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
            vector_value_out[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
            uint8_t* vector_value_out_1 = 0u;
            vector_borrow(vector_2, new_index__198, (void**)(&vector_value_out_1));
            vector_value_out_1[0u] = 10000000000ul - packet_length;
            vector_value_out_1[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
            vector_value_out_1[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
            vector_value_out_1[3u] = 84u;
            vector_value_out_1[4u] = 2u;
            vector_value_out_1[5u] = 0u;
            vector_value_out_1[6u] = 0u;
            vector_value_out_1[7u] = 0u;
            vector_value_out_1[8u] = now & 0xff;
            vector_value_out_1[9u] = (now >> 8) & 0xff;
            vector_value_out_1[10u] = (now >> 16) & 0xff;
            vector_value_out_1[11u] = (now >> 24) & 0xff;
            vector_value_out_1[12u] = (now >> 32) & 0xff;
            vector_value_out_1[13u] = (now >> 40) & 0xff;
            vector_value_out_1[14u] = (now >> 48) & 0xff;
            vector_value_out_1[15u] = (now >> 56) & 0xff;
            map_put(map_1, vector_value_out, new_index__198);
            vector_return(vector_1, new_index__198, vector_value_out);
            vector_return(vector_2, new_index__198, vector_value_out_1);
            return 1;
          }

          // 210
          else {
            // dropping
            return device;
          } // !(false == ((out_of_space__198) & (0u == number_of_freed_flows__191)))

        }

        // 219
        // 223
        // 227
        // 231
        else {
          dchain_rejuvenate_index(dchain_1, map_value_out, now);
          uint8_t* vector_value_out = 0u;
          vector_borrow(vector_2, map_value_out, (void**)(&vector_value_out));
          vector_value_out[0u] = 10000000000ul - packet_length;
          vector_value_out[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
          vector_value_out[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
          vector_value_out[3u] = 84u;
          vector_value_out[4u] = 2u;
          vector_value_out[5u] = 0u;
          vector_value_out[6u] = 0u;
          vector_value_out[7u] = 0u;
          vector_value_out[8u] = now & 0xff;
          vector_value_out[9u] = (now >> 8) & 0xff;
          vector_value_out[10u] = (now >> 16) & 0xff;
          vector_value_out[11u] = (now >> 24) & 0xff;
          vector_value_out[12u] = (now >> 32) & 0xff;
          vector_value_out[13u] = (now >> 40) & 0xff;
          vector_value_out[14u] = (now >> 48) & 0xff;
          vector_value_out[15u] = (now >> 56) & 0xff;

          // 219
          // 223
          // 227
          if ((now - vector_value_out[8ul]) < 10000000000ul) {

            // 219
            // 223
            if (false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {

              // 219
              if (false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {
                vector_return(vector_2, map_value_out, vector_value_out);
                // dropping
                return device;
              }

              // 223
              else {
                vector_return(vector_2, map_value_out, vector_value_out);
                return 1;
              } // !(false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

            }

            // 227
            else {
              vector_return(vector_2, map_value_out, vector_value_out);
              return 1;
            } // !(false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

          }

          // 231
          else {
            vector_return(vector_2, map_value_out, vector_value_out);
            return 1;
          } // !((now - vector_value_out[8ul]) < 10000000000ul)

        } // !(0u == map_has_this_key__196)

      } // !device

    } // !(0u == ((0u == ((6u == ipv4_header_1->next_proto_id) | (17u == ipv4_header_1->next_proto_id))) | ((4294967262u + packet_length) < 4ul)))

  }

  // 233
  else {
    // dropping
    return device;
  } // !(0u == ((8u != ether_header_1->ether_type) | ((4294967282u + packet_length) < 20ul)))

}
