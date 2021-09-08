#include "ovpn_ubus.h"

static const struct blobmsg_policy clientkill_policy[] = {
    [CLIENT_VALUE] = {.name = "client", .type = BLOBMSG_TYPE_STRING},
};

static const struct ubus_method ovpn_methods[] = {
    UBUS_METHOD_NOARG("get", clients_get),
    UBUS_METHOD("kill", client_kill, clientkill_policy)};

static int PORT = 0;
static char *IP_ADDR = NULL;

static int clients_get(struct ubus_context *ctx, struct ubus_object *obj,
                       struct ubus_request_data *req, const char *method,
                       struct blob_attr *msg)
{
    char *token, *token2;
    struct blob_buf b = {};
    bool read_client = false;
    char temp[128];
    void *list, *client;
    char *sav1 = NULL;
    char buff[2056];

    blob_buf_init(&b, 0);

    if (get_status(buff) != 0)  {
        puts("get_status failed\n");
        return 0;
    }

    token = strtok_r(buff, "\r", &sav1);
    list = blobmsg_open_array(&b, "connectedclients");

    /* loop through the string to extract all other tokens*/
    while (token != NULL)
    {
        /*line indicating that client list will start.*/
        if (strncmp(token + 1, "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since", 64) == 0)  {        
            read_client = true;
        }
        /*No more clients after this line*/
        else if (strncmp(token + 1, "ROUTING TABLE", 11) == 0)  {       
            read_client = false;
        }
        /* It's a client line. we need to parse it. */
        else if (read_client == true)   {
        
            strncpy(temp, token, 128);
            char *sav2 = NULL;
            token2 = strtok_r(temp, ",", &sav2);

            client = blobmsg_open_table(&b, token2 + 1);

            if (token2 != NULL) {
                blobmsg_add_string(&b, "name", token2+1);
                token2 = strtok_r(NULL, ",", &sav2);
            }
            if (token2 != NULL) {
                blobmsg_add_string(&b, "ipadress", token2);
                token2 = strtok_r(NULL, ",", &sav2);
            }
            if (token2 != NULL) {
                blobmsg_add_string(&b, "bytesrecv", token2);
                token2 = strtok_r(NULL, ",", &sav2);
            }
            if (token2 != NULL) {
                blobmsg_add_string(&b, "bytessent", token2);
                token2 = strtok_r(NULL, ",", &sav2);
            }
            if (token2 != NULL) {
                blobmsg_add_string(&b, "connectedsince", token2);
                token2 = strtok_r(NULL, ",", &sav2);
            }

            blobmsg_close_table(&b, client);
        }

        token = strtok_r(NULL, "\r", &sav1);
    }
    blobmsg_close_array(&b, list);
    ubus_send_reply(ctx, req, b.head);
    blob_buf_free(&b);

    return 0;
}

static int client_kill(struct ubus_context *ctx, struct ubus_object *obj,
                       struct ubus_request_data *req, const char *method,
                       struct blob_attr *msg)
{

    struct blob_attr *tb[__CLIENTKILL_MAX];
    struct blob_buf b = {};

    blobmsg_parse(clientkill_policy, __CLIENTKILL_MAX, tb, blob_data(msg), blob_len(msg));

    if (!tb[CLIENT_VALUE])
        return UBUS_STATUS_INVALID_ARGUMENT;

    /*Get the string we are gonna use.*/
    char *client = blobmsg_get_string(tb[CLIENT_VALUE]);

    /*Send the command*/
    blob_buf_init(&b, 0);
    int res = kill_client(client);

    /*Send the reply*/
    blobmsg_add_u32(&b, "result", res);
    ubus_send_reply(ctx, req, b.head);
    blob_buf_free(&b);

    return 0;
}

int get_status(char *result)
{
    int rc = telnet(IP_ADDR, PORT, "status\r\n", result);
    if (rc != 0)    {
        printf("Error from SendToTelnet.");
        return rc;
    }

    return 0;
}

int kill_client(char *name)
{
    char msg[1024] = {0};
    int bytesread = 0;
    char server_reply[2000];

    sprintf(msg, "kill %s\r\n", name);

    int rc = telnet(IP_ADDR, PORT, msg, server_reply);
    puts(server_reply);
    if (rc != 0)    {  
        printf("Error from SendToTelnet.");
        return rc;
    }
    return 0;
}

int ovpn_ubus_loop(char *name, char *ip, int port)
{
    IP_ADDR = ip;
    PORT = port;

    struct ubus_context *ctx;

    /*setup*/
    struct ubus_object_type ovpn_object_type =
        UBUS_OBJECT_TYPE(name, ovpn_methods);

    struct ubus_object ovpn_object = {
        .name = name,
        .type = &ovpn_object_type,
        .methods = ovpn_methods,
        .n_methods = ARRAY_SIZE(ovpn_methods),
    };

    uloop_init();

    ctx = ubus_connect(NULL);
    if (!ctx)   {  
        fprintf(stderr, "Failed to connect to ubus\n");
        return -1;
    }

    ubus_add_uloop(ctx);
    ubus_add_object(ctx, &ovpn_object);
    uloop_run();

    ubus_free(ctx);
    uloop_done();

    return 0;
}