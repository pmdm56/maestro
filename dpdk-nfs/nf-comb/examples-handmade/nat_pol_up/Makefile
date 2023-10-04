NF_FILES := main.c nf_config.c state.c utils.c

NF_ARGS := --lan 1 \
           --rate $(or $(POLICER_RATE),1000000000) \
           --burst $(or $(POLICER_BURST),10000000000) \
           --capacity $(or $(CAPACITY),65536) \
		   --wan 1 \
           --expire $(or $(EXPIRATION_TIME),100000000) \
           --max-flows $(or $(CAPACITY),65536) \
           --eth-dest 0,$(or $(TESTER_MAC_EXTERNAL),01:23:45:67:89:00) \
           --eth-dest 1,$(or $(TESTER_MAC_INTERNAL),01:23:45:67:89:01)  \
           --extip $(or $(MB_IP_EXTERNAL),0.0.0.0) \
            --starting-port 0 
NF_LAYER := 4

include $(abspath $(dir $(lastword $(MAKEFILE_LIST))))/../../../Makefile
