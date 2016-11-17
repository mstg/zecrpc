//
// Created by mustafa on 10.11.16.
//

#include <memory.h>
#include "zecrpc.h"
#include "zecrpc_getinfo.h"

rpc_getinfo *zecrpc_getinfo() {
    json_t *resp = zecrpc_send_raw_command("getinfo", NULL);

    if (resp == NULL)
        return NULL;

    size_t ret_size = sizeof(rpc_getinfo);
    rpc_getinfo *ret = malloc(ret_size);

    json_t *error = json_object_get(resp, "error");
    const char *err = (char*)json_string_value(error);
    if (err != NULL) {
        ret_size += strlen(err);
        ret = realloc(ret, ret_size);
        ret->errors = strdup(err);
        zecrpc_free_response(resp);
        return ret;
    }

    json_t *result = json_object_get(resp, "result");

    json_t *errors = json_object_get(result, "errors");

    if (json_is_string(errors)) {
        err = (char*)json_string_value(errors);
        if (strcmp(err, "") != 0) {
            ret_size += strlen(err);
            ret = realloc(ret, ret_size);
            ret->errors = strdup(err);
        } else {
            ret->errors = NULL;
        }
    }


    json_t *toff = json_object_get(result, "timeoffset");
    const uint timeoff = (uint)json_integer_value(toff);
    ret->timeoffset = (uint)timeoff;

    json_t *ptx = json_object_get(result, "paytxfee");
    const double ptxfee = json_number_value(ptx);
    ret->paytxfee = (double)ptxfee;

    json_t *ver = json_object_get(result, "version");
    const uint64_t vers = (uint64_t)json_integer_value(ver);
    ret->version = (uint64_t)vers;

    json_t *pver = json_object_get(result, "protocolversion");
    const uint64_t pvers = (uint64_t)json_integer_value(pver);
    ret->protocolversion = (uint64_t)pvers;

    json_t *wver = json_object_get(result, "walletversion");
    const uint64_t wvers = (uint64_t)json_integer_value(wver);
    ret->walletversion = (uint64_t)wvers;

    json_t *bal = json_object_get(result, "balance");
    const double balnc = json_number_value(bal);
    ret->balance = (double)balnc;

    json_t *blk = json_object_get(result, "blocks");
    const uint blks = (uint)json_integer_value(blk);
    ret->blocks = (uint)blks;

    json_t *cn = json_object_get(result, "connections");
    const uint conn = (uint)json_integer_value(cn);
    ret->connections = (uint)conn;

    ret->proxy = NULL;

    json_t *pr = json_object_get(result, "proxy");

    if (json_is_string(pr)) {
        const char *proxy = (char*)json_string_value(pr);
        if (strcmp(proxy, "") != 0) {
            ret_size += strlen(proxy);
            ret = realloc(ret, ret_size);
            ret->proxy = strdup(proxy);
        }
    }


    json_t *diff = json_object_get(result, "difficulty");
    const double difficulty = (double)json_number_value(diff);
    ret->difficulty = difficulty;

    json_t *test = json_object_get(result, "testnet");
    const int testnet = (int)json_boolean_value(test);
    ret->testnet = testnet;

    json_t *keypo = json_object_get(result, "keypoololdest");
    const uint64_t keypoololdest = (uint64_t)json_integer_value(keypo);
    ret->keypoololdest = keypoololdest;

    json_t *keyps = json_object_get(result, "keypoolsize");
    const uint keypoolsize = (uint)json_integer_value(keyps);
    ret->keypoolsize = keypoolsize;

    json_t *relay = json_object_get(result, "relayfee");
    const double relayfee = (double)json_number_value(relay);
    ret->relayfee = relayfee;

    zecrpc_free_response(resp);

    return ret;
}

void zecrpc_getinfo_free(rpc_getinfo *f) {
    if (f->errors != NULL)
        free(f->errors);

    if (f->proxy != NULL)
        free(f->proxy);

    free(f);
}