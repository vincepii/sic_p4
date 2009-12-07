#include "../../include/utilities.h"
#include "../../include/encryption.h"
#include "../../include/comm_layer.h"

using namespace std;

int main (int argc, char* argv[])
{
	//-------------------------------------------------------------------------

	struct sockaddr_in addr;
	int server_port;
	int sd;
	string server_ip;

	if (argc < 2) user_err("usage: client <server_ip> <server_port>");

	server_ip = argv[1];
	server_port = atoi(argv[2]);
	if (server_ip == "127.0.0.1") server_ip = "localhost";

	sd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);
	inet_pton(AF_INET, server_ip.data(), &addr.sin_addr.s_addr);

	if (connect(sd, CAST_ADDR(&addr), sizeof(struct sockaddr_in)) < 0)
		sys_err("CL: connection error");

	//-------------------------------------------------------------------------
	//----------------   CIFRATURA   ------------------------------------------
	
	string cipher;
	//crea un messaggio e lo stampa
	int src=25;
	int dst=10;
	int nonce=50;
	//la chiave deve essere lunga sempre P_KEY_LENGTH, non di meno => altrimenti ho problemi di sicurezza
	const char* asym_key="chiave541516pubblica615145da_inviare_la_dimensione_deve_essere__";
	
	printf("\ntesto da inviare:\n");	
	printf("src: %d\n", src);
	printf("dst: %d\n", dst);
	printf("nonce: %d\n", nonce);
	printf("asym_key: %s\n\n", asym_key);
	
	printf("Per effettuare la prova è stata usata una chiave fissa (per semplicità nell'uso della stessa per kdc e apeer\n");
	
	//lo cifra e stampa il cifrato
	unsigned char sym_key[EVP_MAX_KEY_LENGTH];
	//select_random_key(sym_key, EVP_MAX_KEY_LENGTH);
	for (int i=0; i<EVP_MAX_KEY_LENGTH; i++)
		memcpy(&sym_key[i], "A", 1);
	
	printf("\n");
	
	Sym_Encryption encr_obj;
	
	cipher=encr_obj.sym_encrypt(sym_key, src, dst, nonce, (unsigned char*)asym_key);

	encr_obj.~Sym_Encryption();
	
	//----------------   INVIO MESSAGGIO   --------------------------------------
	
	Mess mes(25, 10, 0, (unsigned char*)cipher.c_str(), cipher.length());
	mes.send_mes(sd);

	return 0;
}
