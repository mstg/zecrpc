//
// Created by mustafa on 10.11.16.
//

#ifndef ZECGTK_ZECRPC_LISTTRANSACTIONS_H
#define ZECGTK_ZECRPC_LISTTRANSACTIONS_H

#include <stdint.h>

typedef struct rpc_ltransaction {
    char *address;
    char *category;
    double amount;
    int vout;
    int confirmations;
    int generated;
    char *blockhash;
    int blockindex;
    uint64_t blocktime;
    char *txid;
    uint64_t time;
    uint64_t timereceived;
} rpc_ltransaction;

typedef struct rpc_listtransactions {
    rpc_ltransaction **transactions;
    size_t length;
} rpc_listtransactions;

rpc_listtransactions *zecrpc_listtransactions(int count);
void zecrpc_listtransactions_free(rpc_listtransactions *r);

#endif //ZECGTK_ZECRPC_LISTTRANSACTIONS_H
