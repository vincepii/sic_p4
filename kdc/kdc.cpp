//#include "../include/utilities.h"
//#include "../include/comm_layer.h"
#include "kdc.h"

bool search_public_key(int peer, string& pub_key){
	unsigned char key[EVP_MAX_KEY_LENGTH];	//shared key
	int actual_peer;						//peer letto dal file
	unsigned char pk_file[1024];			/*path del file contenente la 
											chiave pubblica del peer*/
	FILE* KRep;								//Key Repository file
	FILE* pub_file;							//file contenente la public key
	unsigned char* buffer;					//contiene contenuto file publ key
	int length;								//lunghezza file public key
	
	//apre il file
	if (!(KRep = fopen("./KeyRepository", "r")))
		sys_err("Non esiste alcun Key Repository!");
	
	//cerca l'entry associata al peer e legge la chiave condivisa da esso e dal KDC
	do{
		if(feof(KRep)) return false;
		fscanf(KRep, "%d", &actual_peer);
		fscanf(KRep, "%s", key);
		fscanf(KRep, "%s", pk_file);
	}while(actual_peer!=peer);
	
	//estraggo la public key del peer
	if(!(pub_file = fopen((const char*)pk_file, "r")))
		sys_err("Non esiste alcun file contenente la chiave pubblica richiesta!");
	
	fseek (pub_file, 0, SEEK_END); 
	buffer=(unsigned char*)malloc((length=ftell(pub_file)));
	rewind(pub_file);
	
	fread (buffer, sizeof(unsigned char), length, pub_file); 
	
	//restituisce la chiave pubblica
	pub_key.insert(0, (char*)buffer, length);
	
	fclose(pub_file);
	
	free(buffer);

	//chiude il file
	fclose(KRep);
	
	return true;
}


bool search_shared_key(int peer, string& sh_key){
	unsigned char key[EVP_MAX_KEY_LENGTH];	//shared key
	int actual_peer;						//peer letto dal file
	unsigned char pk_file[1024];			/*path del file contenente la 
											chiave pubblica del peer*/
	FILE* KRep;								//Key Repository file
	
	//apre il file
	if (!(KRep = fopen("./KeyRepository", "r")))
		sys_err("Non esiste alcun Key Repository!");
	
	//cerca l'entry associata al peer e legge la chiave condivisa da esso e dal KDC
	do{
		if(feof(KRep)) return false;
		fscanf(KRep, "%d", &actual_peer);
		fscanf(KRep, "%s", key);
		fscanf(KRep, "%s", pk_file);
	}while(actual_peer!=peer);

	//restituisci la chiave
	sh_key.insert(0, (char*)key, EVP_MAX_KEY_LENGTH);

	//chiude il file
	fclose(KRep);
	
	return true;
}


/*
 * Processo multithreaded, attende una richiesta sulla accept e crea un
 * thread gestore
 */

using namespace std;

void* body(void* arg){
	int sd = (int) arg;
	int src, dst, nonce;
	string shared_key;
	string public_key;
	string cipher;

	//ricezione messaggio (sono del tipo A, B, Na)
	Mess M2_M4(0,0,0,0,0);
	M2_M4.receive_mes(sd);
	M2_M4.print();
	
	//estrae mittente, destinatario e nonce
	src=M2_M4.getSrc_id();
	dst=M2_M4.getDest_id();
	nonce=M2_M4.getNonce();
	
	//ricava dal KeyRepository la chiave condivisa con A
	if(!search_shared_key(src, shared_key))
		sys_err("Errore ricerca chiave condivisa tra peer e KDC!");
		
	//ricava dal KeyRepository la chiave pubblica di B
	if(!search_shared_key(dst, public_key))
		sys_err("Errore ricerca chiave pubblica di un peer!");
		
	//cifra un messaggio contenente A, B, Na, eB usando la shared_key	
	Sym_Encryption encr_obj;
	cipher=encr_obj.sym_encrypt((unsigned char*)shared_key.c_str(), src, dst, nonce, (unsigned char*)public_key.c_str());
	encr_obj.~Sym_Encryption();
	
	//invia un messaggio contenente A, B e il cipher {A, B, Na, eB} cifrato con la shared_key
	Mess M3_M5(src, dst, nonce, (unsigned char*)cipher.c_str(), cipher.length());
	M3_M5.send_mes(sd);
	M3_M5.print_hex();
	
	/**
	 * Operazioni KDC:
	 * 1. Riceve M2 (o M4)
	 * 2. Legge il primo id e recupera la chiave simmetrica corrispondente
	 * 3. Legge il secondo id e recupera la chiave pubblica corrispondente
	 * 4. Legge il nonce che dovrà usare in M3 ( o M5)
	 * 5. Usa la chiave simmetrica letta al punto 2 per cifrare
	 * 6. Invia il messaggio di risposta
	 */

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

