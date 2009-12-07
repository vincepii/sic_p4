#include "../../include/utilities.h"
#include "../../include/encryption.h"
#include "../../include/comm_layer.h"

/*
 * Processo multithreaded, attende una richiesta sulla accept e crea un
 * thread gestore
 */

using namespace std;

void* body(void* arg){
	int sd = (int) arg;
	int src, dst, nonce;
	string AK;
	string cipher;

	unsigned char sym_key[EVP_MAX_KEY_LENGTH];
	//select_random_key(sym_key, EVP_MAX_KEY_LENGTH);
	for (int i=0; i<EVP_MAX_KEY_LENGTH; i++)
		memcpy(&sym_key[i], "A", 1);

	//ricezione messaggio
	Mess m1(0,0,0,0,0);
	m1.receive_mes(sd);
	m1.print_hex();
	
	src=m1.getSrc_id();
	dst=m1.getDest_id();
	nonce=m1.getNonce();
	cipher.insert(0, (char*)m1.getCipher(), m1.getCipher_ll());
	
	Sym_Encryption decr_obj;
	
	decr_obj.sym_decrypt(sym_key, cipher, &src, &dst, &nonce, AK);
	
	decr_obj.~Sym_Encryption();

	printf("\n\ntesto decifrato: \n");
	printf("src: %d\n", src);
	printf("dst: %d\n", dst);
	printf("nonce: %d\n", nonce);
	cout<<"asym_key: "<<AK<<endl;


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
