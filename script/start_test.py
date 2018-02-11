# -*- coding:utf-8 -*-
# author: @angus
# updatetime:2018年1月25日 22:08:00
# 功能：自动安装safed...

import subprocess


def main():
    print '-------- start to install safe -----------'
    safe_home = '~/.safe'
    mkdir_cmd = 'mkdir ' + safe_home
    ret = subprocess.call(mkdir_cmd, shell=True)



if __name__ == '__main__':
    main()
