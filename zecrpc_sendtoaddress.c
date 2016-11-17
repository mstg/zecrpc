//
// Created by mustafa on 11.11.16.
//

#include <memory.h>
#include "zecrpc_sendtoaddress.h"
#include "zecrpc.h"

char *zecrpc_sendtoaddress(char *address, double amount) {
    json_t *params = json_array();
    json_array_append(params, json_string(address));
    json_array_append(params, json_real(amount));

    json_t *r = zecrpc_send_raw_command("sendtoaddress", params);

    if (r == NULL)
        return NULL;

    json_t *err = json_object_get(r, "error");
    if(json_is_object(err)) {
        json_t *erra = json_object_get(err, "message");

        char *msg;
        if (json_is_string(erra))
            msg = strdup((char*)json_string_value(erra));
        else
            msg = "An unexpected error happened";

        char *retmsg = strdup(msg);

        zecrpc_free_response(r);
        return retmsg;
    }

    json_t *res = json_object_get(r, "result");
    char *retmsg = strdup(json_string_value(res));

    zecrpc_free_response(r);

    return retmsg;
}