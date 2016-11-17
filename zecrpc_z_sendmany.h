//
// Created by mustafa on 12.11.16.
//

#ifndef ZECGTK_ZECRPC_Z_SENDMANY_H
#define ZECGTK_ZECRPC_Z_SENDMANY_H

typedef struct rpc_sendmany {
    char **addresses;
    double *amount;
    int length;
    size_t addr_size;
    size_t amount_size;
} rpc_sendmany;

rpc_sendmany *zecrpc_z_sendmany_new();
void zecrpc_z_sendmany_add(rpc_sendmany *s, char *addr, double amount);

// If amount is 0 all entries with addr will be removed
void zecrpc_z_sendmany_remove(rpc_sendmany *s, char *addr, double amount);

char *zecrpc_z_sendmany(rpc_sendmany *s, char *fromaddr);
void zecrpc_z_sendmany_free(rpc_sendmany *s);

#endif //ZECGTK_ZECRPC_Z_SENDMANY_H
