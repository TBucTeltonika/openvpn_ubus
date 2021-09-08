#ifndef OVPN_UBUS_H
#define OVPN_UBUS_H
#include <libubox/blobmsg_json.h>
#include <libubus.h>

enum {
	CLIENT_VALUE,
	__CLIENTKILL_MAX
};

/* Methods */
static int clients_get(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

static int client_kill(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);


int get_status(char *result);
int kill_client(char* name);
int ovpn_ubus_loop(char* name, char* ip, int port);
#endif