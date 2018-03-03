#!/usr/bin/python
# -*- coding:utf-8 -*-
import subprocess

#kill safed process
def safe_stop():
    #cmd_stop = 'kill -9 `ps fuax|grep safed |grep -v grep| awk 'print $2'`'
    safe_pid = "ps fuax| grep safed| grep -v grep| awk '{print $2}'"
    cmd_stop = 'kill -9 ' + '`' + safe_pid + '`'
    ret = subprocess.call(cmd_stop, shell=True)
    if ret == 0:
	    print "stop suc!"
    else:
        print "stop err!"

#rm dir
def clean_dir(dir):
    cmd = 'rm -rf ' + dir
    ret = subprocess.call(cmd, shell=True)

    if ret == 0:
	    print "clean suc!"
    else:
        print "clean err!"


if __name__ == "__main__":
    safe_stop()
    safe_home = "/root/.safe/"
    clean_dir(safe_home)
