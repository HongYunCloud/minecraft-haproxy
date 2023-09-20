#include "haproxy/minecraft.h"

int parse_minecraft_handshake(struct sample *smp, struct minecraft_handshake* packet) {
  struct channel *chn;
  char *data, *data_end;
  uint64_t packet_size, packet_id;
  if (!smp->strm)
    goto not_minecraft_handshake;

  /* meaningless for HTX buffers */
  if (IS_HTX_STRM(smp->strm))
    goto not_minecraft_handshake;

  chn = ((smp->opt & SMP_OPT_DIR) == SMP_OPT_DIR_RES) ? &smp->strm->res : &smp->strm->req;

  data = (char *)ci_head(chn);
  data_end = data + ci_data(chn);

  if (data_end - data <= 0)
    goto too_short;

  if(decode_varint(&data, data_end, &packet_size) < 0)
    goto too_short;

  if(packet_size > 1024)
    goto not_minecraft_handshake;

  if (data + packet_size > data_end)
    goto too_short;
  data_end = data + packet_size;

  if(decode_varint(&data, data_end, &packet_id) < 0)
    goto not_minecraft_handshake;

  if(decode_varint(&data, data_end, &packet->protocol_version) < 0)
    goto not_minecraft_handshake;

  if(decode_varint(&data, data_end, &packet->server_address_size) < 0)
    goto not_minecraft_handshake;

  if(packet->server_address_size > data_end - data)
    goto not_minecraft_handshake;
  packet->server_address = data;
  data += packet->server_address_size;

  if(data_end - data < 2)
    goto not_minecraft_handshake;
  packet->server_port = (data[0] << 8) | data[1];
  data += 2;

  if(decode_varint(&data, data_end, &packet->next_state) < 0 || packet_id != 0)
    goto not_minecraft_handshake;

  return 0;

  too_short:
  return 1;

  not_minecraft_handshake:
  return 2;
}