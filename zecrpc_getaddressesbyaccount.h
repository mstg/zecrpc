//
// Created by mustafa on 10.11.16.
//

#ifndef ZECGTK_ZECRPC_GETADDRESSESBYACCOUNT_H
#define ZECGTK_ZECRPC_GETADDRESSESBYACCOUNT_H

typedef struct rpc_addresses {
    char **addresses;
    size_t length;
} rpc_addresses;

rpc_addresses *zecrpc_getaddressesbyaccount();
void zecrpc_getaddressesbyaccount_free(rpc_addresses *ret);

#endif //ZECGTK_ZECRPC_GETADDRESSESBYACCOUNT_H
