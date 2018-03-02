#!/usr/bin/python
# -*- coding:utf-8 -*-
import os
import time
import subprocess

def safe_restart(safe_home):
    safed = safe_home + "safed"
    safe_cli = safe_home + "safe-cli"
    cmd_stop = safe_cli + " stop"
    cmd_start = safed + " &"

    ret = subprocess.call(cmd_stop, shell=True)
    if ret == 0:
        time.sleep(0.5);
    	ret = subprocess.call(cmd_start, shell=True)
	if ret == 0:
	    print "restart suc!!"
	else:
	    print "restart err!"
    else:
	print "stop err!"


def reset_item(fileName, item, value):
    print fileName
    #read
    with open(fileName, "r") as f:
        lines = f.readlines()
    #write
    with open(fileName, "w") as f_w:
        for line in lines:
            if item in line:
                line = item + "=" + value + "\n"
            f_w.write(line)



if __name__ == "__main__":
    safe_home = "/root/.safe/"
    #safe_home = "/home/angus/.safe/"
    reset_item(safe_home+"safe.conf", "rpcallowip", "127.0.0.1")
    safe_restart(safe_home)
