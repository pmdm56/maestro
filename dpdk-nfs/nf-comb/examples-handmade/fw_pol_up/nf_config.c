#include "nf_config.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nf-log.h"
#include "nf-parse.h"
#include "nf-util.h"

const uint16_t DEFAULT_LAN = 1;
const uint16_t DEFAULT_WAN = 0;
const uint64_t DEFAULT_RATE = 1000000; // 1MB/s
const uint64_t DEFAULT_BURST = 100000; // 100kB
const uint32_t DEFAULT_CAPACITY = 128; // IPs

#define PARSE_ERROR(format, ...)                                               \
  nf_config_usage();                                                           \
  fprintf(stderr, format, ##__VA_ARGS__);                                      \
  exit(EXIT_FAILURE);

void nf_config_init(int argc, char **argv) {
  // Set the default values
  config.lan_device = DEFAULT_LAN;
  config.wan_device = DEFAULT_WAN;
  config.rate = DEFAULT_RATE;             // B/s
  config.burst = DEFAULT_BURST;           // B
  config.dyn_capacity = DEFAULT_CAPACITY; // MAC addresses

  unsigned nb_devices = rte_eth_dev_count_avail();

  struct option long_options[] = { { "lan", required_argument, NULL, 'l' },
                                   { "wan", required_argument, NULL, 'w' },
                                   { "rate", required_argument, NULL, 'r' },
                                   { "burst", required_argument, NULL, 'b' },
                                   { "capacity", required_argument, NULL, 'c' },
                                   { "eth-dest", required_argument, NULL, 'm' },
                                   { "expire", required_argument, NULL, 't' },
                                   { "max-flows", required_argument, NULL,
                                     'f' },
                                   { NULL, 0, NULL, 0 } };

  config.device_macs = calloc(nb_devices, sizeof(struct rte_ether_addr));
  config.endpoint_macs = calloc(nb_devices, sizeof(struct rte_ether_addr));

  // Set the devices' own MACs
  for (uint16_t device = 0; device < nb_devices; device++) {
    rte_eth_macaddr_get(device, &(config.device_macs[device]));
  }

  int opt;
  while ((opt = getopt_long(argc, argv, "l:w:r:b:c:m:t:f:", long_options,
                            NULL)) != EOF) {
    unsigned device;
    switch (opt) {
      case 'l':
        config.lan_device = nf_util_parse_int(optarg, "lan", 10, '\0');
        if (config.lan_device < 0 || config.lan_device >= nb_devices) {
          PARSE_ERROR("Invalid LAN device.\n");
        }
        break;

      case 'w':
        config.wan_device = nf_util_parse_int(optarg, "wan", 10, '\0');
        if (config.wan_device < 0 || config.wan_device >= nb_devices) {
          PARSE_ERROR("Invalid WAN device.\n");
        }
        break;

      case 'r':
        config.rate = nf_util_parse_int(optarg, "rate", 10, '\0');
        if (config.rate == 0) {
          PARSE_ERROR("Policer rate must be strictly positive.\n");
        }
        break;

      case 'b':
        config.burst = nf_util_parse_int(optarg, "burst", 10, '\0');
        if (config.burst == 0) {
          PARSE_ERROR("Policer burst size must be strictly positive.\n");
        }
        break;

      case 'c':
        config.dyn_capacity = nf_util_parse_int(optarg, "capacity", 10, '\0');
        if (config.dyn_capacity <= 0) {
          PARSE_ERROR("Flow table size must be strictly positive.\n");
        }
        break;
      case 'm':
        device = nf_util_parse_int(optarg, "eth-dest device", 10, ',');
        if (device >= nb_devices) {
          PARSE_ERROR("eth-dest: device %d >= nb_devices (%d)\n", device,
                      nb_devices);
        }

        optarg += 2;
        if (!nf_parse_etheraddr(optarg, &(config.endpoint_macs[device]))) {
          PARSE_ERROR("Invalid MAC address: %s\n", optarg);
        }
        break;

      case 't':
        config.expiration_time =
            nf_util_parse_int(optarg, "exp-time", 10, '\0');
        if (config.expiration_time == 0) {
          PARSE_ERROR("Expiration time must be strictly positive.\n");
        }
        break;

      case 'f':
        config.max_flows = nf_util_parse_int(optarg, "max-flows", 10, '\0');
        if (config.max_flows <= 0) {
          PARSE_ERROR("Flow table size must be strictly positive.\n");
        }
        break;

      default:
        PARSE_ERROR("Unknown option %c", opt);
    }
  }

  // Reset getopt
  optind = 1;
}

void nf_config_usage(void) {
  // //NF_INFO("Usage:\n"
  //         "[DPDK EAL options] --\n"
  //         "\t--lan <device>: LAN device,"
  //         " default: %" PRIu16 ".\n"
  //         "\t--wan <device>: WAN device,"
  //         " default: %" PRIu16 ".\n"
  //         "\t--rate <rate>: policer rate in bytes/s,"
  //         " default: %" PRIu64 ".\n"
  //         "\t--burst <size>: policer burst size in bytes,"
  //         " default: %" PRIu64 ".\n"
  //         "\t--capacity <n>: policer table capacity,"
  //         " default: %" PRIu32 ".\n",
  //         "\t--eth-dest <device>,<mac>: MAC address of the endpoint linked to "
  //         "a device.\n"
  //         "\t--expire <time>: flow expiration time (us).\n"
  //         "\t--max-flows <n>: flow table capacity.\n",
  //         DEFAULT_LAN, DEFAULT_WAN, DEFAULT_RATE, DEFAULT_BURST,
  //         DEFAULT_CAPACITY);
}

void nf_config_print(void) {
  //NF_INFO("\n--- FW_POL Config ---\n");

  //NF_INFO("LAN Device: %" PRIu16, config.lan_device);
  //NF_INFO("WAN Device: %" PRIu16, config.wan_device);
  //NF_INFO("Rate: %" PRIu64, config.rate);
  //NF_INFO("Burst: %" PRIu64, config.burst);
  //NF_INFO("Capacity: %" PRIu16, config.dyn_capacity);

  uint16_t nb_devices = rte_eth_dev_count_avail();
  for (uint16_t dev = 0; dev < nb_devices; dev++) {
    char *dev_mac_str = nf_mac_to_str(&(config.device_macs[dev]));
    char *end_mac_str = nf_mac_to_str(&(config.endpoint_macs[dev]));

    //NF_INFO("Device %" PRIu16 " own-mac: %s, end-mac: %s", dev, dev_mac_str,end_mac_str);

    free(dev_mac_str);
    free(end_mac_str);
  }

  //NF_INFO("Expiration time: %" PRIu32 "us", config.expiration_time);
  //NF_INFO("Max flows: %" PRIu32, config.max_flows);

  //NF_INFO("\n--- ------ ------ ---\n");
}
