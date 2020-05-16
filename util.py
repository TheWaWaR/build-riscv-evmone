#!/usr/bin/env python3

import sys

null = None
tx_data = {
  "mock_info": {
    "cell_deps": [
      {
        "cell_dep": {
          "dep_type": "code",
          "out_point": {
            "index": "0x0",
            "tx_hash": "0x000000000000000000000000000000000000000000000000000000000000ff01"
          }
        },
        "data": "./build/evmone",
        "output": {
          "capacity": "0xdddf8475800",
          "lock": {
            "args": "0x0000000000000000000000000000000000000000",
            "code_hash": "0x0000000000000000000000000000000000000000000000000000000000000000",
            "hash_type": "type"
          },
          "type": null
        }
      }
    ],
    "header_deps": [],
    "inputs": [
      {
        "data": "0x61626364",
        "input": {
          "previous_output": {
            "index": "0x0",
            "tx_hash": "0x000000000000000000000000000000000000000000000000000000000000ff02"
          },
          "since": "0x0"
        },
        "output": {
          "capacity": "0x6fc23ac00",
          "lock": {
            "args": "0x0000000000000000000000000000000000000000",
            "code_hash": "0xeb53da848f78b541b0b0e881ac0bf7666998eb8bd4028e148a856889c4334c16",
            "hash_type": "data"
          },
          "type": null
        }
      }
    ]
  },
  "tx": {
    "cell_deps": [
      {
        "dep_type": "code",
        "out_point": {
          "index": "0x0",
          "tx_hash": "0x000000000000000000000000000000000000000000000000000000000000ff01"
        }
      }
    ],
    "header_deps": [],
    "inputs": [
      {
        "previous_output": {
          "index": "0x0",
          "tx_hash": "0x000000000000000000000000000000000000000000000000000000000000ff02"
        },
        "since": "0x0"
      }
    ],
    "outputs": [],
    "outputs_data": [],
    "version": "0x0",
    "witnesses": []
  }
}

def make_bin_array(path):
    content = open(path).read().strip()
    size = int(len(content)/2)
    print('size = {}'.format(size))
    for i in range(size):
        print('0x{}'.format(content[i*2:(i+1)*2]), end=', ')
    print('')

def make_cli_arg(path):
    import struct
    from binascii import hexlify
    content = open(path).read().strip()
    size = int(len(content)/2)
    output = ['00' * 65]
    output.append('{}'.format(hexlify(struct.pack('<i', size)).decode('utf-8')))
    output.append(content)
    # input data size
    output.append('{}'.format(hexlify(struct.pack('<i', 0)).decode('utf-8')))
    for i in range(len(output)):
        print((i, output[i]))
    print(''.join(output) + '\n')

    total_size = 65 + 4 + size + 4
    print('program size: {}'.format(hexlify(struct.pack('<i', total_size)).decode('utf-8')))


def run(path):
    import tempfile
    import os
    import json
    import binascii
    import subprocess
    with tempfile.NamedTemporaryFile() as fp:
        cmd_hash = 'ckb-cli util blake2b --binary-path {}'.format(path)
        print(cmd_hash)
        data_hash = subprocess.check_output(cmd_hash, shell=True).strip().decode('utf-8')
        print('data hash: {}'.format(data_hash))
        tx_data['mock_info']['inputs'][0]['output']['lock']['code_hash'] = data_hash

        with open(path, 'rb') as binary_f:
            content = binary_f.read()
            print('data size: {}'.format(len(content)))
            data_string = '0x{}'.format(binascii.hexlify(content).decode('utf-8'))
            tx_data['mock_info']['cell_deps'][0]['data'] = data_string
        json_output = json.dumps(tx_data, indent=2)
        fp.write(json_output.encode('utf-8'))
        fp.flush()
        fp.seek(0)
        cmd_verify = 'ckb-cli mock-tx verify --tx-file {}'.format(fp.name)
        print(cmd_verify)
        os.system(cmd_verify)

USAGE = '''USAGE:
  ./util.py make-array Xxx.sol.bin
  ./util.py make-args Xxx.sol.bin
  ./util.py run'''
if len(sys.argv) < 2:
    print(USAGE)
elif sys.argv[1] == 'make-array':
    make_bin_array(sys.argv[2])
elif sys.argv[1] == 'make-args':
    make_cli_arg(sys.argv[2])
elif sys.argv[1] == 'run':
    binary_path = sys.argv[2] if len(sys.argv) > 2 else './build/evmone'
    run(binary_path)
else:
    print('Invalid subcommand: {}'.format(sys.argv[1]))
    print(USAGE)
    exit(-1)
