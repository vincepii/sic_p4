#include "../include/utilities.h"
#include "../include/encryption.h"
#include "../include/comm_layer.h"

using namespace std;

/**
 * Operazioni KDC:
 * 1. Riceve M2 (o M4)
 * 2. Legge il primo id e recupera la chiave simmetrica corrispondente
 * 3. Legge il secondo id e recupera la chiave pubblica corrispondente
 * 4. Legge il nonce che dovrà usare in M3 ( o M5)
 * 5. Usa la chiave simmetrica letta al punto 2 per cifrare
 * 6. Invia il messaggio di risposta
 */

bool search_shared_key(int peer, string& sh_key);
bool search_public_key(int peer, string& pub_key);

void* body(void* arg){
	int sd = (int) arg;		//socket
	int src;
	int dst;
	int nonce;
	string shared_key;
	string public_key;
	string cipher;
	string ivstr;
	unsigned char iv[EVP_MAX_IV_LENGTH];

	cout << "[KDC]: Thread started..." << endl;

	//ricezione messaggio del tipo A, B, Na
	Mess M2_M4(0,0,0,"","");
	M2_M4.receive_mes(sd);

	cout << "[KDC]: ricevuto M2/M4" << endl;

	src = M2_M4.getSrc_id();
	dst = M2_M4.getDest_id();
	nonce = M2_M4.getNonce();

	//cerca la chiave condivisa con A
	if(!search_shared_key(src, shared_key))
		sys_err("Errore ricerca chiave condivisa tra peer e KDC!");

	//cerca la chiave pubblica di B
	if(!search_public_key(dst, public_key))
		sys_err("Errore ricerca chiave pubblica di un peer!");

	//crea crittogramma contenente A, B, Na, eB usando la shared_key
	RAND_bytes(iv, EVP_MAX_IV_LENGTH);
	ivstr.assign((const char *)iv, EVP_MAX_IV_LENGTH);
	Sym_Encryption encr_obj;
	cipher=encr_obj.sym_encrypt((unsigned char*)shared_key.data(), src, dst,
			nonce, (unsigned char*)public_key.data(), ivstr);
	encr_obj.~Sym_Encryption();

	//invia un messaggio contenente A, B, {A, B, Na, eB}Kat
	Mess M3_M5(src, dst, 0, ivstr, cipher);
	M3_M5.send_mes(sd);

	cout << "[KDC]: inviato M3/M5" << endl;

	close(sd);
	return 0;
}

/*
 * Processo multithreaded, attende una richiesta sulla accept e crea un
 * thread gestore
 */

int main(int argc, char* argv[]){
	pthread_t tid;
	int port;

	if (argc<2) user_err("KDC: usage: kdc <port>");
	port = atoi(argv[1]);

	//base_sd è il socket usato dal kdc per attendere richieste; 
	//curr_sd è quello usato da un thread gestore per servire la richiesta
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

bool search_public_key(int peer, string& pub_key){
	unsigned char key[2*EVP_MAX_KEY_LENGTH+1];	//shared key
	int actual_peer;						//peer letto dal file
	unsigned char pk_file[256];			/*path del file contenente la
											chiave pubblica del peer*/
	FILE* KRep;								//Key Repository file
	FILE* pub_file;							//file contenente la public key
	unsigned char* buffer;					//contiene contenuto file pub key
	int length;								//lunghezza file public key
	
	if (!(KRep = fopen("./KeyRepository", "r")))
		sys_err("Non esiste alcun Key Repository!");
	
	//cerca l'entry associata al peer e legge la chiave condivisa
	do{
		if(feof(KRep)) return false;
		fscanf(KRep, "%d", &actual_peer);
		fscanf(KRep, "%s", key);
		fscanf(KRep, "%s", pk_file);
	}while(actual_peer!=peer);
	
	//estrae la public key del peer
	if(!(pub_file = fopen((const char*)pk_file, "r")))
		sys_err("Non esiste alcun file contenente la chiave pubblica richiesta!");
	
	fseek (pub_file, 0, SEEK_END); 
	buffer=(unsigned char *)malloc(length=ftell(pub_file));
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
	unsigned char key[2*EVP_MAX_KEY_LENGTH+1];	//shared key
	int actual_peer;						//peer letto dal file
	unsigned char pk_file[256];			/*path del file contenente la
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
