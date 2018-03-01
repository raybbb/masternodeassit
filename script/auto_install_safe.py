# -*- coding:utf-8 -*-
# author: @angus
# updatetime:2018年1月25日 22:08:00
# 功能：自动安装safed...

import os
import subprocess


def main():
    print '-------- start to install safe -----------'

    safe_home = '~/.safe'
    find_safe_file = os.path.exists(safe_home)
    if find_safe_file:
        print 'SAFE already exists! return..'
        return

    #'http://anwang.com/download/safe_v1.0.1_linux.tar.gz'
    ret = 0
    package_name = 'safe_v1.0.1_linux.tar.gz'
    url_base = 'http://anwang.com/download/'
    package_url = url_base + package_name
    ##step 1: download package
    get_package_cmd = 'wget ' + package_url
    ret = subprocess.call(get_package_cmd, shell=True)
    if ret == 0:
        ##step 2: parse package
        parse_package_cmd = 'tar -xzvf ' + package_name
        print parse_package_cmd
        ret = subprocess.call(parse_package_cmd, shell=True)

        ##step 3: create workdir : ~/.safe
        mkdir_cmd = 'mkdir ' + safe_home
        ret = subprocess.call(mkdir_cmd, shell=True)

        #step 4: copy files
        cp_file_to_home = 'cp ./safe.conf ' + safe_home
        ret = subprocess.call(cp_file_to_home, shell=True)

        cp_file_to_home = 'cp ./safe_v1.0.1/safed ' + safe_home
        ret = subprocess.call(cp_file_to_home, shell=True)
        cp_file_to_home = 'cp ./safe_v1.0.1/safe-cli ' + safe_home
        ret = subprocess.call(cp_file_to_home, shell=True)

        #step 5: change file's power
        change_file_power = 'chmod 777 ~/.safe/safed && chmod 777 ~/.safe/safe-cli'
        ret = subprocess.call(change_file_power, shell=True)

        #step 6: copy depend libs to system dir
        parse_dep = 'tar -xzvf ./safe_v1.0.1/deps/deps-ubuntu14.04.tar.gz -C ./safe_v1.0.1/deps/'
        ret = subprocess.call(parse_dep, shell=True)
        if ret == 0:
            #maybe need root password
            #cp_dep_to_gen_root = 'cp -rf ./safe_v1.0.1/deps/usr ./'
            cp_dep_to_gen_root = 'cp -rf ./safe_v1.0.1/deps/usr /'
            print cp_dep_to_gen_root
            ret = subprocess.call(cp_dep_to_gen_root, shell=True)
            exec_safed = '~/.safe/safed &'
            ret = subprocess.call(exec_safed, shell=True)
    else:
        print 'get package error'

    print '--------  successful to install safe !!!!  -----------'

    #exec_tail = 'tail -f ~/.safe/debug.log'
    #print exec_tail
    #ret = subprocess.call(exec_tail, shell=True)



if __name__ == '__main__':
    main()
