#ifndef _HAPROXY_MINECRAFT_H
#define _HAPROXY_MINECRAFT_H

#include <stdint.h>
#include <stddef.h>

#include <haproxy/sample.h>
#include <haproxy/stream.h>
#include <haproxy/channel.h>

struct minecraft_handshake {
  uint64_t protocol_version;
  char* server_address;
  uint64_t server_address_size;
  uint64_t server_port;
  uint64_t next_state;
};

int parse_minecraft_handshake(struct sample *smp, struct minecraft_handshake* packet);

#endif /* _HAPROXY_MINECRAFT_H */