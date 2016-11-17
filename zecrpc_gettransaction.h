//
// Created by mustafa on 11.11.16.
//

#ifndef ZECGTK_ZECRPC_GETTRANSACTION_H
#define ZECGTK_ZECRPC_GETTRANSACTION_H

#include <stdint.h>

typedef struct rpc_transaction {
    double amount;
    double fee;
    int confirmations;
    char *txid;
    uint64_t time;
    uint64_t timereceived;
    char *toaddr;
    char *fromaddr;
} rpc_transaction;

rpc_transaction *zecrpc_gettransaction(const char *txid);
void zecrpc_gettransaction_free(rpc_transaction *r);

#endif //ZECGTK_ZECRPC_GETTRANSACTION_H
