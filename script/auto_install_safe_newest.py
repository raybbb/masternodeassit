# -*- coding:utf-8 -*-
# author: @angus
# updatetime:2018年1月25日 22:08:00
# 功能：自动安装safed...

import os
import subprocess

def safe_stop():
    #cmd_stop = 'kill -9 `ps fuax|grep safed |grep -v grep| awk 'print $2'`'
    safe_pid = "ps fuax| grep safed| grep -v grep| awk '{print $2}'"
    cmd_stop = 'kill -9 ' + '`' + safe_pid + '`'
    ret = subprocess.call(cmd_stop, shell=True)
    if ret == 0:
	    print "stop suc!"
    else:
        print "stop err!"


def main():
    print '-------- start to install safe -----------'

    #stop process if it exists
    safe_stop()

    safe_home = '~/.safe'
    #'http://anwang.com/download/safe_newest_linux.tar.gz'
    ret = 0
    package_name = 'safe_newest_linux.tar.gz'
    base_dir = 'safe_newest'
    clean_file_cmd = 'rm -rf ' + base_dir + ' safe_v*'
    ret = subprocess.call(clean_file_cmd, shell=True)

    url_base = 'http://www.anwang.com/download/'
    package_url = url_base + package_name
    ##step 1: download package
    get_package_cmd = 'wget -N ' + package_url
    ret = subprocess.call(get_package_cmd, shell=True)
    if ret == 0:
        ##step 2: parse package
        parse_package_cmd = 'tar -xzvf ' + package_name + ' && mv safe_v* ' + base_dir
        print parse_package_cmd
        ret = subprocess.call(parse_package_cmd, shell=True)

        ##step 3: create workdir : ~/.safe
        find_safe_file = os.path.exists(safe_home)
        if find_safe_file:
            pass
        else:
            mkdir_cmd = 'mkdir ' + safe_home
            ret = subprocess.call(mkdir_cmd, shell=True)

        #step 4: copy files
        cp_file_to_home = 'cp ./safe.conf ' + safe_home
        ret = subprocess.call(cp_file_to_home, shell=True)

        cp_file_to_home = 'cp ./' + base_dir + '/safed ' + safe_home
        ret = subprocess.call(cp_file_to_home, shell=True)
        cp_file_to_home = 'cp ./' + base_dir + '/safe-cli ' + safe_home
        ret = subprocess.call(cp_file_to_home, shell=True)

        #step 5: change file's power
        change_file_power = 'chmod 755 ~/.safe/safed && chmod 755 ~/.safe/safe-cli'
        ret = subprocess.call(change_file_power, shell=True)

        #step 6: copy depend libs to system dir
        parse_dep = 'tar -xzvf ./' + base_dir + '/deps/deps-ubuntu14.04.tar.gz -C ./' + base_dir + '/deps/'
        ret = subprocess.call(parse_dep, shell=True)
        if ret == 0:
            #maybe need root password
            cp_dep_to_gen_root = 'cp -rf ./' + base_dir + '/deps/usr /'
            print cp_dep_to_gen_root
            ret = subprocess.call(cp_dep_to_gen_root, shell=True)
            exec_safed = '~/.safe/safed &'
            ret = subprocess.call(exec_safed, shell=True)
        print 'get package suc'
    else:
        print 'get package error'

    print '--------  successful to install safe !!!!  -----------'

    #exec_tail = 'tail -f ~/.safe/debug.log'
    #print exec_tail
    #ret = subprocess.call(exec_tail, shell=True)



if __name__ == '__main__':
    main()
