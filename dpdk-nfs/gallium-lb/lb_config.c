#include "lb_config.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "nf-log.h"
#include "nf-parse.h"
#include "nf-util.h"
#include "nf.h"

const uint32_t DEFAULT_FLOWS_CAPACITY = 65536;
const uint32_t DEFAULT_BACKENDS_CAPACITY = 64;

#define PARSE_ERROR(format, ...)                                               \
  nf_config_usage();                                                           \
  fprintf(stderr, format, ##__VA_ARGS__);                                      \
  exit(EXIT_FAILURE);

void nf_config_init(int argc, char **argv) {
  config.capacity = DEFAULT_FLOWS_CAPACITY;
  config.max_backends = DEFAULT_BACKENDS_CAPACITY;
  config.table_fname[0] = '\0'; // no static configuration

  uint16_t nb_devices = rte_eth_dev_count_avail();

  struct option long_options[] = { { "lan", required_argument, NULL, 'l' },
                                   { "wan", required_argument, NULL, 'w' },
                                   { "capacity", required_argument, NULL, 'c' },
                                   { "backends", required_argument, NULL, 'b' },
                                   { "config", required_argument, NULL, 'f' },
                                   { NULL, 0, NULL, 0 } };

  int opt;
  while ((opt = getopt_long(argc, argv, "l:w:c:b:f:", long_options, NULL)) !=
         EOF) {
    unsigned device;
    switch (opt) {
      case 'l':
        config.lan_device = nf_util_parse_int(optarg, "lan", 10, '\0');
        if (config.lan_device >= nb_devices) {
          PARSE_ERROR("LAN device does not exist.\n");
        }
        break;

      case 'w':
        config.wan_device = nf_util_parse_int(optarg, "wan", 10, '\0');
        if (config.wan_device >= nb_devices) {
          PARSE_ERROR("WAN device does not exist.\n");
        }
        break;

      case 'c':
        config.capacity = nf_util_parse_int(optarg, "capacity", 10, '\0');
        if (config.capacity <= 0) {
          PARSE_ERROR("Capacity must be strictly positive.\n");
        }
        break;

      case 'b':
        config.max_backends = nf_util_parse_int(optarg, "backends", 10, '\0');
        if (config.max_backends <= 0) {
          PARSE_ERROR(
              "Number of maximum backends must be strictly positive.\n");
        }
        break;

      case 'f':
        strncpy(config.table_fname, optarg, TABLE_FNAME_LEN - 1);
        config.table_fname[TABLE_FNAME_LEN - 1] = '\0';
        break;

      default:
        PARSE_ERROR("Unknown option.\n");
        break;
    }
  }

  // Reset getopt
  optind = 1;
}

void nf_config_usage(void) {
  NF_INFO("Usage:\n"
          "[DPDK EAL options] --\n"
          "\t--lan <device>: set device to be the LAN device\n"
          "\t--wan <device>: set device to be the external one.\n"
          "\t--capacity <n>: lb flows capacity"
          " (default: %" PRIu32 ")\n"
          "\t--backends <n>: lb max number of backends"
          " (default: %" PRIu32 ")\n"
          "\t--config <fname>: backends file.\n",
          DEFAULT_FLOWS_CAPACITY, DEFAULT_BACKENDS_CAPACITY);
}

void nf_config_print(void) {
  NF_INFO("\n--- Gallium Load Balancer Config ---\n");

  NF_INFO("LAN device: %" PRIu16, config.lan_device);
  NF_INFO("WAN device: %" PRIu16, config.wan_device);
  NF_INFO("Capacity: %" PRIu32, config.capacity);
  NF_INFO("Max backends: %" PRIu32, config.max_backends);
  NF_INFO("Backends file: %s", config.table_fname);

  NF_INFO("\n--- --- ------ ---\n");
}
