#include "../../include/encryption.h"


int main(){
	unsigned char* cipher;
	//crea un messaggio e lo stampa
	int src=10;
	int dst=25;
	int nonce=65;
	const char* asym_key="chiave541516pubblica615145da_inviare";
	
	printf("src: %d\n", src);
	printf("dst: %d\n", dst);
	printf("nonce: %d\n", nonce);
	printf("asym_key: %s\n", asym_key);
	
	//lo cifra e stampa il cifrato
	unsigned char* sym_key;
	select_random_key(sym_key, EVP_MAX_KEY_LENGTH);
	
	Sym_Encryption encr_obj;
	
	cipher=encr_obj.sym_encrypt(sym_key, src, dst, nonce, (unsigned char*)asym_key);
	printf("funge\n");
	encr_obj.~Sym_Encryption();
/*
	printf("cifrato: %s\n", cipher);
	
	//decifra e stampa il decifrato
	src=0; dst=0; nonce=0; 	//inizializzo variabili perchè poi ci va il decifrato
	
	Sym_Encryption decr_obj;
	decr_obj.sym_decrypt(sym_key, cipher, &src, &dst, &nonce, (unsigned char*)asym_key);
	decr_obj.~Sym_Encryption();

	printf("testo decifrato: \n");
	printf("src: %d\n", src);
	printf("dst: %d\n", dst);
	printf("nonce: %d\n", nonce);
	printf("asym_key: %s\n", asym_key);
	*/
	return 0;
}
