#include "main.h"

void term(int signum)
{
   printf("Caught!\n");
   exit(0);
}

int main(int argc, char **argv)
{

	struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

	int count;

	for( count = 0; count < argc; count++ ){
		logwrite(argv[count]);
	}
	char fullname[128];
    sprintf(fullname,"ovpn.%s",argv[1]);

	char delim[] = " ";
	char *ptr = strtok(argv[2], delim);

	char* ip = strtok(argv[2], delim);
	char* portstr = ip + strlen(ip)+1;

	//printf( "  IP   [%s]\n", ip );

	int port = (int)strtol(portstr, (char **)NULL, 10);
	//printf( "  PORT: int: [%d]  s: [%s]\n", port, portstr );
	ovpn_ubus_loop(fullname, ip, port);
}

