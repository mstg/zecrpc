//
// Created by mustafa on 10.11.16.
//

#include <memory.h>
#include "zecrpc_listtransactions.h"
#include "zecrpc.h"

rpc_listtransactions *zecrpc_listtransactions(int count) {
    json_t *params = json_array();
    json_array_append(params, json_string(""));
    json_array_append(params, json_integer(count));

    json_t *resp = zecrpc_send_raw_command("listtransactions", params);

    if (resp == NULL)
        return NULL;

    size_t ret1_size = sizeof(rpc_listtransactions);
    rpc_listtransactions *ret1 = malloc(ret1_size);
    ret1->transactions = malloc(sizeof(rpc_ltransaction));
    ret1->length = 0;

    json_t *error = json_object_get(resp, "error");
    const char *err = (char*)json_string_value(error);
    if (err != NULL) {
        zecrpc_free_response(resp);
        return NULL;
    }

    json_t *res = json_object_get(resp, "result");

    size_t index;
    json_t *result;

    json_array_foreach(res, index, result) {
        size_t ret_size = sizeof(rpc_ltransaction);
        rpc_ltransaction *ret = malloc(ret_size);

        json_t *addr = json_object_get(result, "address");
        ret->address = NULL;
        if (json_is_string(addr)) {
            const char *address = json_string_value(addr);
            ret_size += strlen(address);
            ret = realloc(ret, ret_size);
            ret->address = (char *) strdup(address);
        }

        json_t *cat = json_object_get(result, "category");
        const char *category = json_string_value(cat);
        ret_size += strlen(category);
        ret = realloc(ret, ret_size);
        ret->category = (char*)strdup(category);

        json_t *amo = json_object_get(result, "amount");
        double amount = json_number_value(amo);
        ret->amount = amount;

        json_t *vo = json_object_get(result, "vout");
        int vout = (int)json_integer_value(vo);
        ret->vout = vout;

        json_t *conf = json_object_get(result, "confirmations");
        int confirmations = (int)json_integer_value(conf);

        if (confirmations < 0)
            confirmations = 0;

        ret->confirmations = confirmations;

        ret->generated = 0;
        ret->blockhash = NULL;
        ret->blockindex = 0;
        ret->blocktime = 0;

        json_t *blkh = json_object_get(result, "blockhash");
        if (json_is_string(blkh)) {
            const char *blockh = json_string_value(blkh);
            ret_size += strlen(blockh);
            ret = realloc(ret, ret_size);
            ret->blockhash = (char*)strdup(blockh);

            json_t *gen = json_object_get(result, "generated");
            double generated = json_boolean_value(gen);
            ret->generated = (int)generated;

            json_t *blki = json_object_get(result, "blockindex");
            int blocki = (int)json_integer_value(blki);
            ret->blockindex = blocki;

            json_t *blkt = json_object_get(result, "blocktime");
            uint64_t blockt = (uint64_t)json_integer_value(blkt);
            ret->blocktime = blockt;
        }

        json_t *tx = json_object_get(result, "txid");
        const char *txid = json_string_value(tx);
        ret_size += strlen(txid);
        ret = realloc(ret, ret_size);
        ret->txid = (char*)strdup(txid);

        json_t *t = json_object_get(result, "time");
        uint64_t time = (uint64_t)json_integer_value(t);
        ret->time = time;

        json_t *tr = json_object_get(result, "timereceived");
        uint64_t timereceived = (uint64_t)json_integer_value(tr);
        ret->timereceived = timereceived;

        ret1_size += ret_size;

        ret1->transactions = realloc(ret1->transactions, ret1_size);
        ret1->transactions[index] = ret;
        ret1->length++;
    }

    zecrpc_free_response(resp);

    return ret1;
}

void zecrpc_listtransactions_free(rpc_listtransactions *r) {
    free(r->transactions);
    free(r);
}