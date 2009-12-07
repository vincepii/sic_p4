#include "../../include/utilities.h"
#include "../../include/comm_layer.h"
#include "../../include/asym_enc.h"

/*
 * Processo multithreaded, attende una richiesta sulla accept e crea un
 * thread gestore
 */

using namespace std;

#define PRIV_KEY_FILE "kdcpriv.pem"

void* body(void* arg){
	int sd = (int) arg;

	unsigned char* cipher;
	int cipher_ll;

	//ricezione primo messaggio
	Mess m1(0,0,0,0,0);
	m1.receive_mes(sd);
	//m1.print_hex();
	cipher_ll = m1.getCipher_ll();
	cipher = m1.getCipher();

	As_enc adec1("Niente", PRIV_KEY_FILE);
	adec1.asym_decr(cipher, cipher_ll);
	adec1.print_plain(3);

	//ricezione secondo messaggio
	Mess m2(0,0,0,0,0);
	m2.receive_mes(sd);
	cipher_ll = m2.getCipher_ll();
	cipher = m2.getCipher();

	As_enc adec2("", PRIV_KEY_FILE);
	adec2.asym_decr(cipher, cipher_ll);
	adec2.print_plain(4);

	//ricezione terzo messaggio
	Mess m3(0,0,0,0,0);
	m3.receive_mes(sd);
	cipher_ll = m3.getCipher_ll();
	cipher = m3.getCipher();

	As_enc adec3("Niente", PRIV_KEY_FILE);
	adec3.asym_decr(cipher, cipher_ll);
	adec3.print_plain(4);

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
	return 0;
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
