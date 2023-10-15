
bool FlowId_eq(void* a, void* b) ;
uint32_t FlowId_hash(void* obj) ;
void FlowId_allocate(void* obj) ;
void null_init(void* obj) ;
bool ip_addr_eq(void* a, void* b) ;
uint32_t ip_addr_hash(void* obj) ;
void ip_addr_allocate(void* obj) ;
void DynamicValue_allocate(void* obj) ;
struct Map* map;
struct Vector* vector;
struct Vector* vector_1;
struct DoubleChain* dchain;
struct Map* map_1;
struct Vector* vector_2;
struct DoubleChain* dchain_1;
struct Vector* vector_3;

bool nf_init() {
  int map_allocation_succeeded__0 = map_allocate(FlowId_eq, FlowId_hash, 65536u, &map);

  // 16
  // 17
  // 18
  // 19
  // 20
  // 21
  // 22
  // 23
  if (map_allocation_succeeded__0) {
    int vector_alloc_success__2 = vector_allocate(16u, 65536u, FlowId_allocate, &vector);

    // 16
    // 17
    // 18
    // 19
    // 20
    // 21
    // 22
    if (vector_alloc_success__2) {
      int vector_alloc_success__4 = vector_allocate(4u, 65536u, null_init, &vector_1);

      // 16
      // 17
      // 18
      // 19
      // 20
      // 21
      if (vector_alloc_success__4) {
        int is_dchain_allocated__6 = dchain_allocate(65536u, &dchain);

        // 16
        // 17
        // 18
        // 19
        // 20
        if (is_dchain_allocated__6) {
          int map_allocation_succeeded__8 = map_allocate(ip_addr_eq, ip_addr_hash, 65536u, &map_1);

          // 16
          // 17
          // 18
          // 19
          if (map_allocation_succeeded__8) {
            int vector_alloc_success__10 = vector_allocate(4u, 65536u, ip_addr_allocate, &vector_2);

            // 16
            // 17
            // 18
            if (vector_alloc_success__10) {
              int is_dchain_allocated__12 = dchain_allocate(65536u, &dchain_1);

              // 16
              // 17
              if (is_dchain_allocated__12) {
                int vector_alloc_success__14 = vector_allocate(16u, 65536u, DynamicValue_allocate, &vector_3);

                // 16
                if (vector_alloc_success__14) {
                  return 1;
                }

                // 17
                else {
                  return 0;
                } // !vector_alloc_success__14

              }

              // 18
              else {
                return 0;
              } // !is_dchain_allocated__12

            }

            // 19
            else {
              return 0;
            } // !vector_alloc_success__10

          }

          // 20
          else {
            return 0;
          } // !map_allocation_succeeded__8

        }

        // 21
        else {
          return 0;
        } // !is_dchain_allocated__6

      }

      // 22
      else {
        return 0;
      } // !vector_alloc_success__4

    }

    // 23
    else {
      return 0;
    } // !vector_alloc_success__2

  }

  // 24
  else {
    return 0;
  } // !map_allocation_succeeded__0

}

int nf_process(uint16_t device, uint8_t* packet, uint16_t packet_length, int64_t now) {
  int number_of_freed_flows__27 = expire_items_single_map(dchain, vector, map, now - 100000000000ul);
  struct rte_ether_hdr* ether_header_1 = (struct rte_ether_hdr*)(packet);

  // 40
  // 48
  // 71
  // 75
  // 85
  // 90
  // 95
  // 100
  // 114
  // 118
  // 128
  // 133
  // 138
  // 143
  // 158
  // 162
  // 172
  // 177
  // 182
  // 187
  // 192
  // 204
  // 207
  // 216
  // 220
  // 224
  // 228
  if (0u == ((8u != ether_header_1->ether_type) | ((4294967282u + packet_length) < 20ul))) {
    struct rte_ipv4_hdr* ipv4_header_1 = (struct rte_ipv4_hdr*)(packet + 14u);

    // 40
    // 48
    // 71
    // 75
    // 85
    // 90
    // 95
    // 100
    // 114
    // 118
    // 128
    // 133
    // 138
    // 143
    // 158
    // 162
    // 172
    // 177
    // 182
    // 187
    if (0u == ((0u == ((6u == ipv4_header_1->next_proto_id) | (17u == ipv4_header_1->next_proto_id))) | ((4294967262u + packet_length) < 4ul))) {
      struct tcpudp_hdr* tcpudp_header_1 = (struct tcpudp_hdr*)(packet + (14u + 20u));

      // 40
      // 48
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
        int map_has_this_key__34 = map_get(map, &map_key, &map_value_out);

        // 40
        if (0u == map_has_this_key__34) {
          int number_of_freed_flows__36 = expire_items_single_map(dchain_1, vector_2, map_1, now - 10000000000ul);
          // dropping
          return device;
        }

        // 48
        else {
          uint8_t* vector_value_out = 0u;
          vector_borrow(vector_1, map_value_out, (void**)(&vector_value_out));
          vector_return(vector_1, map_value_out, vector_value_out);
          dchain_rejuvenate_index(dchain, map_value_out, now);
          int number_of_freed_flows__44 = expire_items_single_map(dchain_1, vector_2, map_1, now - 10000000000ul);
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
        } // !(0u == map_has_this_key__34)

      }

      // 71
      // 75
      // 85
      // 90
      // 95
      // 100
      // 114
      // 118
      // 128
      // 133
      // 138
      // 143
      // 158
      // 162
      // 172
      // 177
      // 182
      // 187
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
        int map_has_this_key__49 = map_get(map, &map_key, &map_value_out);

        // 71
        // 75
        // 85
        // 90
        // 95
        // 100
        // 114
        // 118
        // 128
        // 133
        // 138
        // 143
        if (0u == map_has_this_key__49) {
          uint32_t new_index__51;
          int out_of_space__51 = !dchain_allocate_new_index(dchain, &new_index__51, now);

          // 71
          // 75
          // 85
          // 90
          // 95
          // 100
          if (false == ((out_of_space__51) & (0u == number_of_freed_flows__27))) {
            uint8_t* vector_value_out = 0u;
            vector_borrow(vector, new_index__51, (void**)(&vector_value_out));
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
            map_put(map, vector_value_out, new_index__51);
            vector_return(vector, new_index__51, vector_value_out);
            uint8_t* vector_value_out_1 = 0u;
            vector_borrow(vector_1, new_index__51, (void**)(&vector_value_out_1));
            vector_value_out_1[0u] = 0u;
            vector_value_out_1[1u] = 0u;
            vector_value_out_1[2u] = 0u;
            vector_value_out_1[3u] = 0u;
            vector_return(vector_1, new_index__51, vector_value_out_1);
            int number_of_freed_flows__58 = expire_items_single_map(dchain_1, vector_2, map_1, now - 10000000000ul);
            uint8_t map_key_1[4];
            map_key_1[0u] = ipv4_header_1->dst_addr & 0xff;
            map_key_1[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
            map_key_1[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
            map_key_1[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
            int map_value_out_1;
            int map_has_this_key__59 = map_get(map_1, &map_key_1, &map_value_out_1);

            // 71
            // 75
            if (0u == map_has_this_key__59) {
              uint32_t new_index__61;
              int out_of_space__61 = !dchain_allocate_new_index(dchain_1, &new_index__61, now);

              // 71
              if (false == ((out_of_space__61) & (0u == number_of_freed_flows__58))) {
                uint8_t* vector_value_out_2 = 0u;
                vector_borrow(vector_2, new_index__61, (void**)(&vector_value_out_2));
                vector_value_out_2[0u] = ipv4_header_1->dst_addr & 0xff;
                vector_value_out_2[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
                vector_value_out_2[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
                vector_value_out_2[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
                uint8_t* vector_value_out_3 = 0u;
                vector_borrow(vector_3, new_index__61, (void**)(&vector_value_out_3));
                vector_value_out_3[0u] = 10000000000ul - packet_length;
                vector_value_out_3[1u] = ((10000000000ul - packet_length) >> 8ul) & 0xff;
                vector_value_out_3[2u] = ((10000000000ul - packet_length) >> 16ul) & 0xff;
                vector_value_out_3[3u] = 84u;
                vector_value_out_3[4u] = 2u;
                vector_value_out_3[5u] = 0u;
                vector_value_out_3[6u] = 0u;
                vector_value_out_3[7u] = 0u;
                vector_value_out_3[8u] = now & 0xff;
                vector_value_out_3[9u] = (now >> 8) & 0xff;
                vector_value_out_3[10u] = (now >> 16) & 0xff;
                vector_value_out_3[11u] = (now >> 24) & 0xff;
                vector_value_out_3[12u] = (now >> 32) & 0xff;
                vector_value_out_3[13u] = (now >> 40) & 0xff;
                vector_value_out_3[14u] = (now >> 48) & 0xff;
                vector_value_out_3[15u] = (now >> 56) & 0xff;
                map_put(map_1, vector_value_out_2, new_index__61);
                vector_return(vector_2, new_index__61, vector_value_out_2);
                vector_return(vector_3, new_index__61, vector_value_out_3);
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

              // 75
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
              } // !(false == ((out_of_space__61) & (0u == number_of_freed_flows__58)))

            }

            // 85
            // 90
            // 95
            // 100
            else {
              dchain_rejuvenate_index(dchain_1, map_value_out_1, now);
              uint8_t* vector_value_out_2 = 0u;
              vector_borrow(vector_3, map_value_out_1, (void**)(&vector_value_out_2));
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

              // 85
              // 90
              // 95
              if ((now - vector_value_out_2[8ul]) < 10000000000ul) {

                // 85
                // 90
                if (false == (10000000000ul < (vector_value_out_2[0ul] + ((1000000000ul * (now - vector_value_out_2[8ul])) / 1000000000ul)))) {

                  // 85
                  if (false == (packet_length < (vector_value_out_2[0ul] + ((1000000000ul * (now - vector_value_out_2[8ul])) / 1000000000ul)))) {
                    vector_return(vector_3, map_value_out_1, vector_value_out_2);
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

                  // 90
                  else {
                    vector_return(vector_3, map_value_out_1, vector_value_out_2);
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
                  } // !(false == (packet_length < (vector_value_out_2[0ul] + ((1000000000ul * (now - vector_value_out_2[8ul])) / 1000000000ul))))

                }

                // 95
                else {
                  vector_return(vector_3, map_value_out_1, vector_value_out_2);
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
                } // !(false == (10000000000ul < (vector_value_out_2[0ul] + ((1000000000ul * (now - vector_value_out_2[8ul])) / 1000000000ul))))

              }

              // 100
              else {
                vector_return(vector_3, map_value_out_1, vector_value_out_2);
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
              } // !((now - vector_value_out_2[8ul]) < 10000000000ul)

            } // !(0u == map_has_this_key__59)

          }

          // 114
          // 118
          // 128
          // 133
          // 138
          // 143
          else {
            int number_of_freed_flows__101 = expire_items_single_map(dchain_1, vector_2, map_1, now - 10000000000ul);
            uint8_t map_key_1[4];
            map_key_1[0u] = ipv4_header_1->dst_addr & 0xff;
            map_key_1[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
            map_key_1[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
            map_key_1[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
            int map_value_out_1;
            int map_has_this_key__102 = map_get(map_1, &map_key_1, &map_value_out_1);

            // 114
            // 118
            if (0u == map_has_this_key__102) {
              uint32_t new_index__104;
              int out_of_space__104 = !dchain_allocate_new_index(dchain_1, &new_index__104, now);

              // 114
              if (false == ((out_of_space__104) & (0u == number_of_freed_flows__101))) {
                uint8_t* vector_value_out = 0u;
                vector_borrow(vector_2, new_index__104, (void**)(&vector_value_out));
                vector_value_out[0u] = ipv4_header_1->dst_addr & 0xff;
                vector_value_out[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
                vector_value_out[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
                vector_value_out[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
                uint8_t* vector_value_out_1 = 0u;
                vector_borrow(vector_3, new_index__104, (void**)(&vector_value_out_1));
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
                map_put(map_1, vector_value_out, new_index__104);
                vector_return(vector_2, new_index__104, vector_value_out);
                vector_return(vector_3, new_index__104, vector_value_out_1);
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

              // 118
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
              } // !(false == ((out_of_space__104) & (0u == number_of_freed_flows__101)))

            }

            // 128
            // 133
            // 138
            // 143
            else {
              dchain_rejuvenate_index(dchain_1, map_value_out_1, now);
              uint8_t* vector_value_out = 0u;
              vector_borrow(vector_3, map_value_out_1, (void**)(&vector_value_out));
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

              // 128
              // 133
              // 138
              if ((now - vector_value_out[8ul]) < 10000000000ul) {

                // 128
                // 133
                if (false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {

                  // 128
                  if (false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {
                    vector_return(vector_3, map_value_out_1, vector_value_out);
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

                  // 133
                  else {
                    vector_return(vector_3, map_value_out_1, vector_value_out);
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

                // 138
                else {
                  vector_return(vector_3, map_value_out_1, vector_value_out);
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

              // 143
              else {
                vector_return(vector_3, map_value_out_1, vector_value_out);
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

            } // !(0u == map_has_this_key__102)

          } // !(false == ((out_of_space__51) & (0u == number_of_freed_flows__27)))

        }

        // 158
        // 162
        // 172
        // 177
        // 182
        // 187
        else {
          dchain_rejuvenate_index(dchain, map_value_out, now);
          int number_of_freed_flows__145 = expire_items_single_map(dchain_1, vector_2, map_1, now - 10000000000ul);
          uint8_t map_key_1[4];
          map_key_1[0u] = ipv4_header_1->dst_addr & 0xff;
          map_key_1[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
          map_key_1[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
          map_key_1[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
          int map_value_out_1;
          int map_has_this_key__146 = map_get(map_1, &map_key_1, &map_value_out_1);

          // 158
          // 162
          if (0u == map_has_this_key__146) {
            uint32_t new_index__148;
            int out_of_space__148 = !dchain_allocate_new_index(dchain_1, &new_index__148, now);

            // 158
            if (false == ((out_of_space__148) & (0u == number_of_freed_flows__145))) {
              uint8_t* vector_value_out = 0u;
              vector_borrow(vector_2, new_index__148, (void**)(&vector_value_out));
              vector_value_out[0u] = ipv4_header_1->dst_addr & 0xff;
              vector_value_out[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
              vector_value_out[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
              vector_value_out[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
              uint8_t* vector_value_out_1 = 0u;
              vector_borrow(vector_3, new_index__148, (void**)(&vector_value_out_1));
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
              map_put(map_1, vector_value_out, new_index__148);
              vector_return(vector_2, new_index__148, vector_value_out);
              vector_return(vector_3, new_index__148, vector_value_out_1);
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

            // 162
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
            } // !(false == ((out_of_space__148) & (0u == number_of_freed_flows__145)))

          }

          // 172
          // 177
          // 182
          // 187
          else {
            dchain_rejuvenate_index(dchain_1, map_value_out_1, now);
            uint8_t* vector_value_out = 0u;
            vector_borrow(vector_3, map_value_out_1, (void**)(&vector_value_out));
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

            // 172
            // 177
            // 182
            if ((now - vector_value_out[8ul]) < 10000000000ul) {

              // 172
              // 177
              if (false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {

                // 172
                if (false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {
                  vector_return(vector_3, map_value_out_1, vector_value_out);
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

                // 177
                else {
                  vector_return(vector_3, map_value_out_1, vector_value_out);
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

              // 182
              else {
                vector_return(vector_3, map_value_out_1, vector_value_out);
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

            // 187
            else {
              vector_return(vector_3, map_value_out_1, vector_value_out);
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

          } // !(0u == map_has_this_key__146)

        } // !(0u == map_has_this_key__49)

      } // !device

    }

    // 192
    // 204
    // 207
    // 216
    // 220
    // 224
    // 228
    else {
      int number_of_freed_flows__188 = expire_items_single_map(dchain_1, vector_2, map_1, now - 10000000000ul);

      // 192
      if (device) {
        // dropping
        return device;
      }

      // 204
      // 207
      // 216
      // 220
      // 224
      // 228
      else {
        uint8_t map_key[4];
        map_key[0u] = ipv4_header_1->dst_addr & 0xff;
        map_key[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
        map_key[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
        map_key[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
        int map_value_out;
        int map_has_this_key__193 = map_get(map_1, &map_key, &map_value_out);

        // 204
        // 207
        if (0u == map_has_this_key__193) {
          uint32_t new_index__195;
          int out_of_space__195 = !dchain_allocate_new_index(dchain_1, &new_index__195, now);

          // 204
          if (false == ((out_of_space__195) & (0u == number_of_freed_flows__188))) {
            uint8_t* vector_value_out = 0u;
            vector_borrow(vector_2, new_index__195, (void**)(&vector_value_out));
            vector_value_out[0u] = ipv4_header_1->dst_addr & 0xff;
            vector_value_out[1u] = (ipv4_header_1->dst_addr >> 8) & 0xff;
            vector_value_out[2u] = (ipv4_header_1->dst_addr >> 16) & 0xff;
            vector_value_out[3u] = (ipv4_header_1->dst_addr >> 24) & 0xff;
            uint8_t* vector_value_out_1 = 0u;
            vector_borrow(vector_3, new_index__195, (void**)(&vector_value_out_1));
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
            map_put(map_1, vector_value_out, new_index__195);
            vector_return(vector_2, new_index__195, vector_value_out);
            vector_return(vector_3, new_index__195, vector_value_out_1);
            // dropping
            return device;
          }

          // 207
          else {
            // dropping
            return device;
          } // !(false == ((out_of_space__195) & (0u == number_of_freed_flows__188)))

        }

        // 216
        // 220
        // 224
        // 228
        else {
          dchain_rejuvenate_index(dchain_1, map_value_out, now);
          uint8_t* vector_value_out = 0u;
          vector_borrow(vector_3, map_value_out, (void**)(&vector_value_out));
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

          // 216
          // 220
          // 224
          if ((now - vector_value_out[8ul]) < 10000000000ul) {

            // 216
            // 220
            if (false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {

              // 216
              if (false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul)))) {
                vector_return(vector_3, map_value_out, vector_value_out);
                // dropping
                return device;
              }

              // 220
              else {
                vector_return(vector_3, map_value_out, vector_value_out);
                // dropping
                return device;
              } // !(false == (packet_length < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

            }

            // 224
            else {
              vector_return(vector_3, map_value_out, vector_value_out);
              // dropping
              return device;
            } // !(false == (10000000000ul < (vector_value_out[0ul] + ((1000000000ul * (now - vector_value_out[8ul])) / 1000000000ul))))

          }

          // 228
          else {
            vector_return(vector_3, map_value_out, vector_value_out);
            // dropping
            return device;
          } // !((now - vector_value_out[8ul]) < 10000000000ul)

        } // !(0u == map_has_this_key__193)

      } // !device

    } // !(0u == ((0u == ((6u == ipv4_header_1->next_proto_id) | (17u == ipv4_header_1->next_proto_id))) | ((4294967262u + packet_length) < 4ul)))

  }

  // 230
  else {
    // dropping
    return device;
  } // !(0u == ((8u != ether_header_1->ether_type) | ((4294967282u + packet_length) < 20ul)))

}
