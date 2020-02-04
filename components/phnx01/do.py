#!/usr/bin/env python
# -*- coding: utf-8 -*-  

from __future__ import division, print_function
import sys,os
import subprocess

print(sys.argv)
command = sys.argv[1] if len(sys.argv) > 1 else "UNKNOWN"

if command == "flash" :
    binFile = sys.argv[2]
    binFile = binFile.replace("\\","/") # 简单的Window路径到POSIX路径转换
    run_args = ["openocd", "-f", "interface/cmsis-dap.cfg", "-f", "board/frdm-kl25z.cfg",
            "-c", "program " + binFile + " reset exit"]
    ret = subprocess.call(run_args, env=os.environ)

elif command == "gdbserver" :
    run_args = ["openocd", "-f", "interface/cmsis-dap.cfg", "-f", "board/frdm-kl25z.cfg"]
    ret = subprocess.Popen(run_args, env=os.environ)

elif command == "monitor" :
    serialPort = sys.argv[2]
    baudRate = sys.argv[3] if len(sys.argv) > 3 else "115200"
    run_args = ["putty", "-sercfg", baudRate + ",8,n,1,N", "-serial", serialPort]
    ret = subprocess.Popen(run_args, env=os.environ)
    
else :
    print('''Bad Command!
Usage: 
    do.py flash binfile
    do.py gdbserver
    do.py monitor
''')

