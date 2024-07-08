#!/bin/bash

installpath="/usr/local/include"
libpath="/usr/local/lib"

sudo rm -rf $installpath/bbt/pollevent
sudo rm -rf $libpath/libbbt_pollevent.so

echo "删除完毕"