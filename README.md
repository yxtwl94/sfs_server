# A Stupid for Study Server


[![Build Status](https://travis-ci.org/yxtwl94/sfs_server.svg?branch=master)](https://travis-ci.org/yxtwl94/sfs_server)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)

A C++ server project which used to study,contains most IO modells(BIO NIO reactor etc...)
sfs means stupid for study

* 开发平台：x86-64 Linux 18.04 LTS
* 开发语言：C++ 11

## BIO modell

blocking i/o modell use TCP/IP

同步阻塞模型实现，该模型缺乏弹性，仅仅在客户端数量较少情况下使用。

当客户端连接过多会导致每个客户端需要为其分配单独线程，造成系统资源不足，线程切换效率低下。

### 服务端

通过socket创建，然后监听连接。当连接到达后为其分配非阻塞handeler线程以实现多用户连接

handler线程处理用户输入数据并返回相同数据。

数据输入以buff[BUFFSIZE]实现

### 客户端

通过socket创建，然后发送请求。可同时生成约3000个客户端。

客户端利用transmit函数发送信息。
