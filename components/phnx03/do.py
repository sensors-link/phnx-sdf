#!/usr/bin/env python
# -*- coding: utf-8 -*-  

from __future__ import division, print_function
import sys,os
import subprocess

command = sys.argv[1] if len(sys.argv) > 1 else "UNKNOWN"

if command == "flash" :
    os.system("TASKKILL /F /IM openocd.exe 2>NUL")
    binFile = sys.argv[2]
    binFile = binFile.replace("bin","elf")
    binFile = binFile.replace("\\","/") # 简单的Window路径到POSIX路径转换
    run_args = ["openocd", "-f", "interface/ftdi/sipeed-rv-debugger.cfg", "-f", "target/gd32vf103.cfg",
        "-c", "adapter_khz 8000",
        "-c", "init; halt;",
        "-c", "flash protect 0 0 last off; program {" + binFile + "}; mww 0xe004200c 0x4b5a6978; mww 0xe0042008 0x01; resume; exit 0;"
        # "-c", "flash protect 0 0 last off; program {" + binFile + "} verify; mww 0xe004200c 0x4b5a6978; mww 0xe0042008 0x01; resume; exit 0;"
        # "-c", "program {" + binFile + "} ;resume; exit 0;"
    ]
    ret = subprocess.call(run_args, env=os.environ)

elif command == "gdbserver" :
    os.system("TASKKILL /F /IM openocd.exe 2>NUL")
    os.system("START openocd -f \"interface/ftdi/sipeed-rv-debugger.cfg\" -f \"target/gd32vf103.cfg\" -c \"adapter_khz 8000\"")
    # os.system("openocd -f \"interface/ftdi/sipeed-rv-debugger.cfg\" -f \"target/gd32vf103.cfg\" -c \"adapter_khz 8000\"")

elif command == "monitor" :
    serialPort = sys.argv[2]
    baudRate = sys.argv[3] if len(sys.argv) > 3 else "57600"
    os.system("START putty -sercfg " + baudRate + ",8,n,1,N -serial " + serialPort)

else :
    print('''Bad Command!
Usage: 
    do.py flash binfile
    do.py gdbserver
    do.py monitor
''')

