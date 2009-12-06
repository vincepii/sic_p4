#include "../../include/encryption.h"


int main(){
	unsigned char* cipher;
	unsigned char AK[P_KEY_LENGTH];
	//crea un messaggio e lo stampa
	int src=10;
	int dst=25;
	int nonce=65;
	//la chiave deve essere lunga sempre P_KEY_LENGTH, non di meno => altrimenti ho problemi di sicurezza
	const char* asym_key="chiave541516pubblica615145da_inviare_la_dimensione_deve_essere__";
	//unsigned char asym_key2[P_KEY_LENGTH];
	
	printf("src: %d\n", src);
	printf("dst: %d\n", dst);
	printf("nonce: %d\n", nonce);
	printf("asym_key: %s\n", asym_key);
	
	//lo cifra e stampa il cifrato
	unsigned char sym_key[EVP_MAX_KEY_LENGTH];
	select_random_key(sym_key, EVP_MAX_KEY_LENGTH);
	
	Sym_Encryption encr_obj;
	
	cipher=encr_obj.sym_encrypt(sym_key, src, dst, nonce, (unsigned char*)asym_key);

	encr_obj.~Sym_Encryption();
		
	printf("cifrato: \n");
	for (unsigned int i=0; i<strlen((const char*)cipher); i++)
		printbyte(cipher[i]);

	printf("\n");
	
	//decifra e stampa il decifrato
	src=0; dst=0; nonce=0; 	//inizializzo variabili perchè poi ci va il decifrato
	
	Sym_Encryption decr_obj;
	decr_obj.sym_decrypt(sym_key, cipher, &src, &dst, &nonce, AK);
	//decr_obj.~Sym_Encryption();

	printf("testo decifrato: \n");
	printf("src: %d\n", src);
	printf("dst: %d\n", dst);
	printf("nonce: %d\n", nonce);
	//printf("asym_key: %s\n", AK);
	cout<<strlen((const char*)AK)<<endl;
	for(unsigned int i=0; i<strlen((const char*)AK); i++)
		cout<<AK[i]<<".";
	cout<<endl;
	
	return 0;
}
