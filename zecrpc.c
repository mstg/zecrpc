//
// Created by mustafa on 09.11.16.
//

#include "zecrpc.h"
#include <curl/curl.h>
#include <memory.h>

static CURL *curl = NULL;
static struct curl_slist *headers = NULL;

typedef struct response {
    size_t size;
    char* data;
} response;

size_t write_data(void *ptr, size_t size, size_t nmemb, response *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        printf("zecrpc error: %s", "Failed to allocate memory.");
        return 0;
    }

    memcpy((data->data + index), ptr, n);

    data->data[data->size] = '\0';

    return size * nmemb;
}

void zecrpc_init(const char *host, const char *port, const char *username, const char *password) {
    if (host == NULL || port == NULL || username == NULL || password == NULL)
        return;

    curl = curl_easy_init();

    if (headers == NULL)
        headers = curl_slist_append(headers, "content-type: text/plain;");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    char *url = malloc(2048);
    snprintf(url, 2048, "http://%s:%s/", host, port);

    curl_easy_setopt(curl, CURLOPT_URL, url);

    free(url);

    char *up = malloc(2048);
    snprintf(up, 2048, "%s:%s", username, password);

    curl_easy_setopt(curl, CURLOPT_USERPWD, up);

    free(up);

    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
}

void zecrpc_free_response(json_t *resp) {
    json_decref(resp);
}

json_t *zecrpc_parse_response(const char *input) {
    printf("%s", input);
    if (strcmp(input, "") == 0) {
        return NULL;
    }

    json_t *root;
    json_error_t err;

    root = json_loads(input, 0, &err);

    if (!root) {
        printf("zecrpc error: %s", err.text);
        return NULL;
    }

    if (json_is_array(root)) {
        printf("zecrpc error: %s", "Response contains an array for json root object");
        json_decref(root);
        return NULL;
    }

    return root;
}

json_t *zecrpc_send_raw_command(const char *method, json_t *params) {
    if (zecrpc_initiated() != 1) {
        printf("zecrpc error: %s", "RPC not initiated");
        return NULL;
    }

    char *pdata;

    json_t *root = json_object();

    json_object_set_new(root, "params", params);
    json_object_set_new(root, "id", json_string("zecgtk"));
    json_object_set_new(root, "jsonrpc", json_string("2.0"));
    json_object_set_new(root, "method", json_string(method));

    pdata = json_dumps(root, 0);

    json_decref(params);
    json_decref(root);

    response data;
    data.size = 0;
    data.data = malloc(4096);

    data.data[0] = '\0';

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(pdata));
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pdata);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    curl_easy_perform(curl);

    json_t *parsed = zecrpc_parse_response(data.data);

    free(data.data);

    return parsed;
}

int zecrpc_test() {
    return zecrpc_send_raw_command("getinfo", NULL) ? 1 : 0;
}

int zecrpc_initiated() {
    return curl == NULL ? 0 : 1;
}

void zecrpc_destruct() {
    curl_free(curl);
}