#include <klee/klee.h>
#include "packet.h"
#include "ring.h"

#define CAP 512

int main() {
  {// Loop iteration begins.
    struct packet p;
    if (!ring_full())
      if (receive_packet(&p) && p.port != 9)
        ring_push(&p);
    if (!ring_empty() && can_send_packet()) {
      ring_pop(&p);
      assert(p.port != 9);
      send_packet(&p);
    }
  }// Loop iteration ends.
  return 0;
}
