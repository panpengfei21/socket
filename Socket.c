//
//  Socket.c
//  TestC01
//
//  Created by Liuming Qiu on 2016/11/28.
//  Copyright © 2016年 PPF. All rights reserved.
//

#include "Socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>


#define INVALID_SOCKET -1
#define SOCKET_ERROR -1



void errExit(const char *format,...){
    va_list args;
    va_start(args,format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}

SOCKET connectSock(const char*host, const char *service, const char *transport){
    
    struct hostent *phe     = NULL;
    struct servent *pse     = NULL;
    struct protoent * ppe   = NULL;
    struct sockaddr_in sin;
    int s, type;
    
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    
    // 把服务名解析成端口号 如(服务名"HTTP",端口号:80)
    pse = getservbyname(service, transport);
    if (pse) {
        sin.sin_port = pse->s_port;
    }else if ((sin.sin_port = htons(atoi(service))) == 0){
        errExit("can`t get \"%s\" service entry\n",service);
    }
    // htons 是把主机字节顺序转成网络字节顺序16进制的,htonl是32进制的
    // ntohs ntohl是相反
    //        sin.sin_port = htons(3001);
    
    // 把点分IP(123.58.180.121)或域名(study.163.com)转成32位的二进制地址
    phe = gethostbyname(host);
    if (phe) {
        memcpy(&sin.sin_addr, phe->h_addr_list[0], phe->h_length);
    }else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE){
        // net_addr()的功能是将一个点分十进制的IP转换成一个长整数型数
        errExit("can`t get \"%s\" host entry\n",host);
    }
    
    // 解析协议号
    if ((ppe = getprotobyname(transport)) == 0) {
        errExit("can`t get \"%s\" protocol entry\n",transport);
    }
    
    if (strcmp(transport, "udp") == 0) {
        type = SOCK_DGRAM;
    }else{
        type = SOCK_STREAM;
    }
    
    s = socket(PF_INET, type, ppe->p_proto);
    
    if (s == INVALID_SOCKET) {
        errExit("can`t creat socket:%d\n",s);
    }
    
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) {
        errExit("can`t connect to %s.%s\n",host,service);
    }
    return s;
}

SOCKET connectUDP(const char *host,const char *service){
    return connectSock(host, service, "udp");
}

SOCKET connectTCP(const char *host,const char *service){
    return connectSock(host, service, "tcp");
}
