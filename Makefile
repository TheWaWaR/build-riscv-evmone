TARGET := riscv64-unknown-elf
CC := $(TARGET)-gcc
CXX := $(TARGET)-g++
OBJCOPY := $(TARGET)-objcopy

CFLAGS_INTX := -Iintx/lib/intx -Iintx/include
CFLAGS_ETHASH := -Iethash/include -Iethash/lib/ethash -Iethash/lib/keccak -Iethash/lib/support
CFLAGS_EVMONE := -Ievmone/lib/evmone -Ievmone/include -Ievmone/evmc/include
CFLAGS := -O3 $(CFLAGS_EVMONE) $(CFLAGS_INTX) $(CFLAGS_ETHASH) -Wall
CXXFLAGS := $(CFLAGS) -std=c++1z
LDFLAGS := -fdata-sections -ffunction-sections -Wl,--gc-sections

ALL_OBJS := build/evmone.o build/analysis.o build/execution.o build/instructions.o build/div.o build/keccak.o build/keccakf800.o build/keccakf1600.o

BUILDER_DOCKER := nervos/ckb-riscv-gnu-toolchain@sha256:7b168b4b109a0f741078a71b7c4dddaf1d283a5244608f7851f5714fbad273ba

all: build/evmone

all-via-docker:
	mkdir -p build
	docker run --rm -v `pwd`:/code ${BUILDER_DOCKER} bash -c "cd /code && make"

build/evmone: main.c $(ALL_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ main.c $(ALL_OBJS)
	$(OBJCOPY) --strip-debug --strip-all $@
	riscv64-unknown-elf-run $@

build/evmone.o: evmone/lib/evmone/evmone.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $< -DPROJECT_VERSION=\"0.5.0-dev\"
build/analysis.o: evmone/lib/evmone/analysis.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<
build/execution.o: evmone/lib/evmone/execution.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<
build/instructions.o: evmone/lib/evmone/instructions.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<

build/keccak.o: ethash/lib/keccak/keccak.c build/keccakf800.o build/keccakf1600.o
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<
build/keccakf1600.o: ethash/lib/keccak/keccakf1600.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<
build/keccakf800.o: ethash/lib/keccak/keccakf800.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -c -o $@ $<

build/div.o: intx/lib/intx/div.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

clean:
	rm -rf build/*

clean-main:
	rm -rf build/evmone
