.PHONY:all clean

all:ubus_openvpn

ubus_openvpn: main.c ovpn_ubus.c telnet.c logger.c
	$(CC) -o $@ $^ -lblobmsg_json -lubox -lubus

clean:
	rm -rf ubus_openvpn
