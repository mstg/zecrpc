//
// Created by mustafa on 09.11.16.
//

#ifndef ZECGTK_ZECRPC_H
#define ZECGTK_ZECRPC_H

#include <jansson.h>

void zecrpc_init(const char *host, const char *port, const char *username, const char *password);
void zecrpc_destruct();

int  zecrpc_test();
int zecrpc_initiated();
json_t *zecrpc_send_raw_command(const char *method, json_t *params);
void zecrpc_free_response(json_t *resp);

#endif //ZECGTK_ZECRPC_H
