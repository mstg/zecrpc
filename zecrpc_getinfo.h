//
// Created by mustafa on 10.11.16.
//

#ifndef ZECGTK_ZECRPC_GETINFO_H
#define ZECGTK_ZECRPC_GETINFO_H

#include <jansson.h>
#include <stdint.h>

typedef struct rpc_getinfo {
    uint64_t version;
    uint64_t protocolversion;
    uint64_t walletversion;
    double balance;
    uint blocks;
    uint timeoffset;
    uint connections;
    char *proxy;
    double difficulty;
    int testnet;
    uint64_t keypoololdest;
    uint keypoolsize;
    double paytxfee;
    double relayfee;
    char *errors;
} rpc_getinfo;

rpc_getinfo *zecrpc_getinfo();
void zecrpc_getinfo_free(rpc_getinfo *f);

#endif //ZECGTK_ZECRPC_GETINFO_H
