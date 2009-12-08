//#include "../include/utilities.h"
//#include "../include/comm_layer.h"
#include "kdc.h"


void search_shared_key(int peer, string& sh_key){
	//unsigned char sh_key[EVP_MAX_KEY_LENGTH];		//shared key
	char peer_id[10];								/*caratteri che formano il peer_id
													con 10 riesco a coprire il range di valori
													per l'int (32 bit)*/
	int actual_peer;								//peer letto dal file
	fstream KRep;										//key repository file
	
	
	//apre il file
	KRep.open("./KeyRepository", ios::in|ios::binary);

	
	//cerca l'entry associata al peer e legge la chiave condivisa da esso e dal KDC
	
	/*do{
		int i=0;
		while(KRep.fgets(peer_id[i], 10, KRep)){
			if(peer_id[i]==';') break;
			i++;	
		}
		peer_id[i]="\0";
		actual_peer=atoi(peer_id);
	}while(actual_peer!=peer);//passa alla prox riga*/
	do{
		KRep.getline(peer_id,10,';');
		actual_peer=atoi(peer_id);
		if(actual_peer==peer) break;
		KRep.getline(
	}while(1);
	
		
	
	//chiude il file
	KRep.close();
	
	return;
}


/*
 * Processo multithreaded, attende una richiesta sulla accept e crea un
 * thread gestore
 */

using namespace std;

void* body(void* arg){
	int sd = (int) arg;
	int src, dst, nonce;

	//ricezione messaggio (sono del tipo A, B, Na)
	Mess m1(0,0,0,0,0);
	m1.receive_mes(sd);
	m1.print();
	
	//estrae mittente, destinatario e nonce
	src=m1.getSrc_id();
	dst=m1.getDest_id();
	nonce=m1.getNonce();
	
	//ricava dal KeyRepository la chiave condivisa con A
	


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
