//
//  Socket.h
//  TestC01
//
//  Created by Liuming Qiu on 2016/11/28.
//  Copyright © 2016年 PPF. All rights reserved.
//

#ifndef Socket_h
#define Socket_h

#include <stdio.h>

typedef int SOCKET;

SOCKET connectTCP(const char *host,const char *service);
SOCKET connectUDP(const char *host,const char *service);

#endif /* Socket_h */
