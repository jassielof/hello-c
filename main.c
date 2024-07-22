// simple build: 'gcc -o hello main.c -lmicrohttpd'
// optimized build: 'gcc -O3 -march=native -flto -ffast-math -s -o hello_world_server main.c -lmicrohttpd'


#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define PORT 8888
#define ENDPOINT "/hello"
#define RESPONSE_BODY "<html><body>Hello, World!</body></html>"

static bool is_hello_endpoint(const char *url)
{
    return strcmp(url, ENDPOINT) == 0;
}

static struct MHD_Response *create_hello_response(void)
{
    return MHD_create_response_from_buffer(
        strlen(RESPONSE_BODY),
        (void *)RESPONSE_BODY,
        MHD_RESPMEM_PERSISTENT);
}

static enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                                      const char *url, const char *method,
                                      const char *version, const char *upload_data,
                                      size_t *upload_data_size, void **con_cls)
{
    (void)cls;
    (void)method;
    (void)version;
    (void)upload_data;
    (void)upload_data_size;
    (void)con_cls;

    if (!is_hello_endpoint(url))
    {
        return MHD_NO;
    }

    struct MHD_Response *response = create_hello_response();
    enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

static struct MHD_Daemon *start_server(void)
{
    return MHD_start_daemon(
        MHD_USE_INTERNAL_POLLING_THREAD, PORT,
        NULL, NULL,
        &handle_request, NULL,
        MHD_OPTION_END);
}

int main(void)
{
    struct MHD_Daemon *daemon = start_server();
    if (daemon == NULL)
    {
        return 1;
    }

    printf("Server running on http://localhost:%d%s\n", PORT, ENDPOINT);
    getchar();

    MHD_stop_daemon(daemon);
    return 0;
}
