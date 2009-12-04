#include "../include/utilities.h"

/*
 * Processo multithreaded, attende una richiesta sulla accept e crea un
 * thread gestore
 */

using namespace std;

void* body(void* arg){
	int sd = (int) arg;

	//thread pronto a gestire la comunicazione sul socket sd
	//segue pseudocodice:
	/*
	 * ricevi messaggio del tipo A, B, Na
	 * Trova la chiave condivisa con A da file
	 * Trova la chiave pubblica di B da file
	 * Cifra, usando Kat, la quantità {A, B, Na, eb}
	 * Invia sul socket il messaggio: A, B, {A, B, Na, eb}Kat
	 */

	//fine thread gestore
}

int main(int argc, char* argv[]){
	pthread_t tid;
	int port;

	if (argc<2) user_err("KDC: usage: kdc <port>");
	port = atoi(argv[1]);

	int base_sd, curr_sd;
	struct sockaddr_in c_add;
	unsigned int addrlen;

	base_sd = init_sd(port);
	addrlen = sizeof(c_add);

	while(1) {
		curr_sd = accept(base_sd, CAST_ADDR(&c_add), &addrlen);
		if(curr_sd < 0) sys_err("KDC: accept failed");
		if (pthread_create(&tid, 0, body, (void *) curr_sd) !=0)
			sys_err("KDC: error creating thread for client");
	}
	return 0;
}
