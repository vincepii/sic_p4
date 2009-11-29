#include "../include/utilities.h"

/*
 * Processo multithreaded,
 */

using namespace std;

int main(int argc, char* argv[]){
	pthread_t tid;
	int port;

	if (argc<2) user_err("Server: usage: server <port>");
	port = atoi(argv[1]);

	int base_sd, curr_sd;
	struct sockaddr_in c_add;
	unsigned int addrlen;

	base_sd = init_sd(port);
	addrlen = sizeof(c_add);

	while(1) {
		curr_sd = accept(base_sd, CAST_ADDR(&c_add), &addrlen);
		if(curr_sd < 0) sys_err("Server: accept failed");
		if (pthread_create(&tid, 0, body, (void *) curr_sd) !=0)
			sys_err("Server: error creating thread for client");
	}
	return 0;
}
