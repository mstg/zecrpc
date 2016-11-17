//
// Created by mustafa on 12.11.16.
//

#ifndef ZECGTK_ZECRPC_Z_GETTOTALBALANCE_H
#define ZECGTK_ZECRPC_Z_GETTOTALBALANCE_H

typedef struct rpc_totalbalance {
    double transparent;
    double private;
    double total;
} rpc_totalbalance;

rpc_totalbalance *zecrpc_z_gettotalbalance();
void zecrpc_z_gettotalbalance_free(rpc_totalbalance *ret);

#endif //ZECGTK_ZECRPC_Z_GETTOTALBALANCE_H
