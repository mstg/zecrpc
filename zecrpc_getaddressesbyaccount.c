//
// Created by mustafa on 10.11.16.
//

#include <memory.h>
#include "zecrpc_getaddressesbyaccount.h"
#include "zecrpc.h"

rpc_addresses *zecrpc_getaddressesbyaccount() {
    json_t *params = json_array();
    json_array_append(params, json_string(""));

    json_t *resp = zecrpc_send_raw_command("getaddressesbyaccount", params);

    if (resp == NULL)
        return NULL;

    json_t *error = json_object_get(resp, "error");
    const char *err = (char*)json_string_value(error);
    if (err != NULL) {
        zecrpc_free_response(resp);
        return NULL;
    }

    json_t *result = json_object_get(resp, "result");

    size_t index;
    json_t *value;

    rpc_addresses *ret = malloc(sizeof(rpc_addresses));
    ret->addresses = malloc(1024);
    size_t size = 1024;
    size_t addrsize = 1024;
    ret->length = 0;

    json_array_foreach(result, index, value) {
        const char *addr = strdup(json_string_value(value));
        size += strlen(addr);
        addrsize += strlen(addr);
        ret = realloc(ret, size);
        ret->addresses = realloc(ret->addresses, addrsize);
        ret->addresses[ret->length] = (char*)addr;

        ret->length++;
    }

    zecrpc_free_response(resp);

    return ret;
}

void zecrpc_getaddressesbyaccount_free(rpc_addresses *ret) {
    free(ret->addresses);
    free(ret);
}