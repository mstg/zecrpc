//
// Created by mustafa on 10.11.16.
//

#include "zecrpc_getbalance.h"
#include "zecrpc.h"

double zecrpc_getbalance() {
    json_t *resp = zecrpc_send_raw_command("getbalance", NULL);

    if (resp == NULL)
        return 0;

    json_t *error = json_object_get(resp, "error");
    const char *err = (char*)json_string_value(error);
    if (err != NULL) {
        zecrpc_free_response(resp);
        return -1;
    }

    json_t *result = json_object_get(resp, "result");
    const double balance = json_number_value(result);

    zecrpc_free_response(resp);

    return balance;
}