//
// Created by mustafa on 12.11.16.
//

#include <malloc.h>
#include "zecrpc_z_gettotalbalance.h"
#include "zecrpc.h"

rpc_totalbalance *zecrpc_z_gettotalbalance() {
    rpc_totalbalance *ret = malloc(sizeof(rpc_totalbalance));
    ret->private = 0.0;
    ret->transparent = 0.0;
    ret->total = 0.0;

    json_t *resp = zecrpc_send_raw_command("z_gettotalbalance", NULL);

    if (resp == NULL)
        return NULL;

    json_t *err = json_object_get(resp, "error");

    if (json_is_object(err)) {
        zecrpc_free_response(resp);
        return ret;
    }

    json_t *res = json_object_get(resp, "result");

    if (!json_is_object(res)) {
        zecrpc_free_response(resp);
        return ret;
    }

    json_t *tr = json_object_get(res, "transparent");
    const char *_transparent = json_string_value(tr);
    double transparent = strtod(_transparent, 0);
    ret->transparent = transparent;

    json_t *pr = json_object_get(res, "private");
    const char *_private = json_string_value(pr);
    double private = strtod(_private, 0);
    ret->private = private;

    json_t *to = json_object_get(res, "total");
    const char *_total = json_string_value(to);
    double total = strtod(_total, 0);
    ret->total = total;

    zecrpc_free_response(resp);

    return ret;
}

void zecrpc_z_gettotalbalance_free(rpc_totalbalance *ret) {
    free(ret);
}