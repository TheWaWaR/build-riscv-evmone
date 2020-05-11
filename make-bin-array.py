#!/usr/bin/env python3

import sys

content = open(sys.argv[1]).read().strip()
size = int(len(content)/2)
print('size = {}'.format(size))
for i in range(size):
    print('0x{}'.format(content[i*2:(i+1)*2]), end=', ')
