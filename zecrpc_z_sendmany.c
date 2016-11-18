//
// Created by mustafa on 12.11.16.
//

#include <malloc.h>
#include <memory.h>
#include "zecrpc_z_sendmany.h"
#include "zecrpc.h"

rpc_sendmany *zecrpc_z_sendmany_new() {
    rpc_sendmany *ret = malloc(sizeof(rpc_sendmany));
    ret->addr_size = sizeof(char*);
    ret->addresses = malloc(ret->addr_size);
    ret->amount_size = sizeof(double);
    ret->amount = malloc(ret->amount_size);
    ret->length = 0;

    return ret;
}

void zecrpc_z_sendmany_add(rpc_sendmany *s, char *addr, double amount) {
    s->addr_size += strlen(addr) + sizeof(char*);
    s->amount_size += sizeof(amount);

    s->addresses = realloc(s->addresses, s->addr_size);
    s->addresses[s->length] = strdup(addr);

    s->amount = realloc(s->amount, s->amount_size);
    s->amount[s->length] = amount;

    s->length++;
}

// If amount is 0 all entries with addr will be removed
void zecrpc_z_sendmany_remove(rpc_sendmany *s, char *addr, double amount) {
    for (int i = 0; i < s->length; i++) {
        if (strcmp(s->addresses[i], addr)) {
            if (s->amount[i] == amount || amount == 0) {
                s->addr_size -= strlen(addr);
                s->amount_size -= sizeof(amount);

                s->addresses[s->length] = NULL;
                s->addresses = realloc(s->addresses, s->addr_size);

                s->amount[s->length] = 0;
                s->amount = realloc(s->amount, s->amount_size);

                s->length--;
            }
        }
    }
}

char *zecrpc_z_sendmany(rpc_sendmany *s, char *fromaddr) {
    json_t *root = json_array();
    json_array_append(root, json_string(fromaddr));

    json_t *in = json_array();
    json_array_append(root, in);

    for (int i = 0; i < s->length; i++) {
        char *t = s->addresses[i];
        json_t *obj = json_object();
        json_object_set(obj, "address", json_string(strdup(s->addresses[i])));
        json_object_set(obj, "amount", json_real(s->amount[i]));
        json_array_append(in, obj);
    }

    printf("%s\n", json_dumps(root, 0));

    json_t *resp = zecrpc_send_raw_command("z_sendmany", root);

    if (resp == NULL)
        return NULL;

    json_t *error = json_object_get(resp, "error");
    if (json_is_object(error)) {
        char *err = strdup(json_string_value(json_object_get(error, "message")));
        if (err != NULL) {
            zecrpc_free_response(resp);
            return err;
        }
    }

    json_t *result = json_object_get(resp, "result");
    char *opid = strdup(json_string_value(result));
    zecrpc_free_response(resp);

    return opid;
}

void zecrpc_z_sendmany_free(rpc_sendmany *s) {
    free(s->addresses);
    free(s->amount);
    free(s);
}
