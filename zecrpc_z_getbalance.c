//
// Created by mustafa on 13.11.16.
//

#include "zecrpc_z_getbalance.h"
#include "zecrpc.h"

double zecrpc_z_getbalance(char *addr) {
    json_t *params = json_array();
    json_array_append(params, json_string(addr));

    json_t *resp = zecrpc_send_raw_command("z_getbalance", params);

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