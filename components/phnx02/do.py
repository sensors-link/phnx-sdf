#!/usr/bin/env python
# -*- coding: utf-8 -*-  

from __future__ import division, print_function
import sys,os
import subprocess

command = sys.argv[1] if len(sys.argv) > 1 else "UNKNOWN"

if command == "flash" :
    os.system("TASKKILL /F /IM openocd.exe 2>NUL")
    binFile = sys.argv[2]
    binFile = binFile.replace("\\","/") # 简单的Window路径到POSIX路径转换
    run_args = ["openocd", "-f", "interface/cmsis-dap.cfg", "-f", "board/frdm-kl25z.cfg",
            "-c", "program " + binFile + " reset exit"]
    ret = subprocess.call(run_args, env=os.environ)

elif command == "gdbserver" :
    os.system("TASKKILL /F /IM openocd.exe 2>NUL")
    os.system("START openocd -f \"interface/cmsis-dap.cfg\" -f \"board/frdm-kl25z.cfg\"")

elif command == "monitor" :
    serialPort = sys.argv[2]
    baudRate = sys.argv[3] if len(sys.argv) > 3 else "115200"
    os.system("START putty -sercfg " + baudRate + ",8,n,1,N -serial " + serialPort)

else :
    print('''Bad Command!
Usage: 
    do.py flash binfile
    do.py gdbserver
    do.py monitor
''')

