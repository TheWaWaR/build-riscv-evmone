
#include <evmone/evmone.h>
#include <evmc/evmc.h>
#include <evmc/evmc.hpp>

int main() {
  // struct evmc_vm *vm = evmc_create_evmone();
  auto vm = evmc::VM{evmc_create_evmone()};
}
