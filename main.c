
#include <evmone/evmone.h>
#include <evmc/evmc.h>
#include <stdio.h>

struct evmc_host_context {
  evmc_address address;
  evmc_bytes32 key;
  evmc_bytes32 value;
};

struct evmc_tx_context get_tx_context(struct evmc_host_context* context) {
  struct evmc_tx_context ctx;
  return ctx;
}
evmc_bytes32 get_storage(struct evmc_host_context* context,
                         const evmc_address* address,
                         const evmc_bytes32* key) {
  evmc_bytes32 value;
  return value;
}

enum evmc_storage_status set_storage(struct evmc_host_context* context,
                                     const evmc_address* address,
                                     const evmc_bytes32* key,
                                     const evmc_bytes32* value) {
  context->address = *address;
  context->key = *key;
  context->value = *value;
  return EVMC_STORAGE_ADDED;
}

void println_hex(const char *prefix, const uint8_t *data, size_t size) {
  printf("%s => 0x", prefix);
  for(size_t i = 0; i < size; i++)
    printf("%02x", data[i]);
  printf("\n");
}

/// NOTE: This program must compile use g++ since evmone implemented with c++17
int main() {
  struct evmc_vm *vm = evmc_create_evmone();
  struct evmc_host_interface interface = { NULL, get_storage, set_storage, NULL, NULL, NULL, NULL, NULL, NULL, get_tx_context, NULL, NULL};
  struct evmc_host_context context;

  struct evmc_message msg;
  msg.kind = EVMC_CREATE;
  msg.destination.bytes[19] = 4;  // Call Identify precompile at address 0x4.
  msg.sender.bytes[18] = 3;
  msg.depth = 0;
  msg.flags = 0;
  msg.input_data = NULL;
  msg.input_size = 0;
  msg.gas = 1000000;

  uint8_t code[] = {0x60, 0x80, 0x60, 0x40, 0x52, 0x5b, 0x60, 0x7b, 0x60, 0x00, 0x60, 0x00, 0x50, 0x81, 0x90, 0x90, 0x90, 0x55, 0x50, 0x5b, 0x61, 0x00, 0x18, 0x56, 0x5b, 0x60, 0xdb, 0x80, 0x61, 0x00, 0x26, 0x60, 0x00, 0x39, 0x60, 0x00, 0xf3, 0xfe, 0x60, 0x80, 0x60, 0x40, 0x52, 0x60, 0x04, 0x36, 0x10, 0x60, 0x29, 0x57, 0x60, 0x00, 0x35, 0x60, 0xe0, 0x1c, 0x80, 0x63, 0x60, 0xfe, 0x47, 0xb1, 0x14, 0x60, 0x2f, 0x57, 0x80, 0x63, 0x6d, 0x4c, 0xe6, 0x3c, 0x14, 0x60, 0x5b, 0x57, 0x60, 0x29, 0x56, 0x5b, 0x60, 0x00, 0x60, 0x00, 0xfd, 0x5b, 0x60, 0x59, 0x60, 0x04, 0x80, 0x36, 0x03, 0x60, 0x20, 0x81, 0x10, 0x15, 0x60, 0x44, 0x57, 0x60, 0x00, 0x60, 0x00, 0xfd, 0x5b, 0x81, 0x01, 0x90, 0x80, 0x80, 0x35, 0x90, 0x60, 0x20, 0x01, 0x90, 0x92, 0x91, 0x90, 0x50, 0x50, 0x50, 0x60, 0x84, 0x56, 0x5b, 0x00, 0x5b, 0x34, 0x80, 0x15, 0x60, 0x67, 0x57, 0x60, 0x00, 0x60, 0x00, 0xfd, 0x5b, 0x50, 0x60, 0x6e, 0x60, 0x94, 0x56, 0x5b, 0x60, 0x40, 0x51, 0x80, 0x82, 0x81, 0x52, 0x60, 0x20, 0x01, 0x91, 0x50, 0x50, 0x60, 0x40, 0x51, 0x80, 0x91, 0x03, 0x90, 0xf3, 0x5b, 0x80, 0x60, 0x00, 0x60, 0x00, 0x50, 0x81, 0x90, 0x90, 0x90, 0x55, 0x50, 0x5b, 0x50, 0x56, 0x5b, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x50, 0x54, 0x90, 0x50, 0x60, 0xa2, 0x56, 0x5b, 0x90, 0x56, 0xfe, 0xa2, 0x64, 0x69, 0x70, 0x66, 0x73, 0x58, 0x22, 0x12, 0x20, 0x4e, 0x58, 0x80, 0x4e, 0x37, 0x5d, 0x4a, 0x73, 0x2a, 0x7b, 0x67, 0xcc, 0xe8, 0xd8, 0xff, 0xa9, 0x04, 0xfa, 0x53, 0x4d, 0x45, 0x55, 0xe6, 0x55, 0xa4, 0x33, 0xce, 0x0a, 0x5e, 0x0d, 0x33, 0x9f, 0x64, 0x73, 0x6f, 0x6c, 0x63, 0x43, 0x00, 0x06, 0x06, 0x00, 0x33};

  printf("msg.gas: %ld\n", msg.gas);
  struct evmc_result res = vm->execute(vm, &interface, &context, EVMC_MAX_REVISION, &msg, code, sizeof(code));
  printf("status_code: %d\n", res.status_code);
  printf("gas_left: %ld, gas_cost: %ld\n", res.gas_left, msg.gas - res.gas_left);
  printf("output_size: %ld\n", res.output_size);
  println_hex("output_data", res.output_data, res.output_size);

  println_hex("context.address", context.address.bytes, sizeof(context.address.bytes));
  println_hex("context.key    ", context.key.bytes, sizeof(context.key.bytes));
  println_hex("context.value  ", context.value.bytes, sizeof(context.value.bytes));

  return (int)res.status_code;
}