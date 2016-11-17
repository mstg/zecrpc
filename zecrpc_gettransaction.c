//
// Created by mustafa on 11.11.16.
//

#include <memory.h>
#include "zecrpc_gettransaction.h"
#include "zecrpc.h"

rpc_transaction *zecrpc_gettransaction(const char *txid) {
    json_t *params = json_array();
    json_array_append(params, json_string(txid));

    json_t *resp = zecrpc_send_raw_command("gettransaction", params);

    if (resp == NULL)
        return NULL;

    size_t ret1_size = sizeof(rpc_transaction);
    rpc_transaction *ret1 = malloc(ret1_size);

    ret1_size += strlen(txid);
    ret1 = realloc(ret1, ret1_size);
    ret1->txid = (char*)strdup(txid);

    ret1->toaddr = NULL;
    ret1->fromaddr = NULL;

    json_t *error = json_object_get(resp, "error");
    const char *err = (char*)json_string_value(error);
    if (err != NULL) {
        zecrpc_free_response(resp);
        return NULL;
    }

    json_t *res = json_object_get(resp, "result");

    json_t *fe = json_object_get(res, "fee");
    double fee = (double)json_number_value(fe);
    ret1->fee = -fee;

    json_t *co = json_object_get(res, "confirmations");
    int conf = (int)json_integer_value(co);

    if (conf < 0)
        conf = 0;

    ret1->confirmations = conf;

    json_t *t = json_object_get(res, "time");
    uint64_t time = (uint64_t)json_integer_value(t);
    ret1->time = time;

    json_t *tr = json_object_get(res, "timereceived");
    uint64_t timereceived = (uint64_t)json_integer_value(tr);
    ret1->timereceived = timereceived;

    json_t *details = json_object_get(res, "details");
    if (details == NULL || !json_is_array(details)) {
        zecrpc_free_response(resp);
        return ret1;
    }

    size_t index;
    json_t *val;
    json_array_foreach(details, index, val) {
        json_t *cat = json_object_get(val, "category");
        const char *categ = json_string_value(cat);

        if (strcmp(categ, "send") == 0) {
            json_t *sa = json_object_get(val, "address");

            if (json_is_string(sa)) {
                const char *saddr = json_string_value(sa);
                ret1_size += strlen(saddr);
                ret1 = realloc(ret1, ret1_size);
                ret1->fromaddr = (char*)strdup(saddr);
            }

            json_t *amo = json_object_get(val, "amount");
            if (json_is_number(amo)) {
                double amount = (double)json_number_value(amo);
                ret1->amount = amount;
            }
        } else if (strcmp(categ, "receive") == 0) {
            json_t *ta = json_object_get(val, "address");

            if (json_is_string(ta)) {
                const char *taddr = json_string_value(ta);
                ret1_size += strlen(taddr);
                ret1 = realloc(ret1, ret1_size);
                ret1->toaddr = (char*)strdup(taddr);
            }

            json_t *amo = json_object_get(val, "amount");
            if (json_is_number(amo)) {
                double amount = (double)json_number_value(amo);
                ret1->amount = amount;
            }
        }
    }

    zecrpc_free_response(resp);

    return ret1;
}

void zecrpc_gettransaction_free(rpc_transaction *r) {
    free(r);
}