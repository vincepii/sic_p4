#include "encryption.h"

Sym_Encryption::Sym_Encryption(){
	//context initialization
	this->ctx=(EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(this->ctx);
	
	return;
}


Sym_Encryption::~Sym_Encryption(){
	if (EVP_CIPHER_CTX_cleanup(this->ctx)==0)
		sys_err("Context deallocation error!");

	return;
}

char* Sym_Encryption::sym_encrypt(int src, int dst, int nonce, const char* asym_key){
	char* chipertext;
	int msg_len;
	int ct_len;
	int nc;			//numero byte effettivamente cifrati
	int ct_ptr=0;	/*puntatore alla posizione di chipertext 
					nella quale inserire i nuovi dati cifrati*/
	char sym_key[EVP_MAX_KEY_LENGTH];
	
	msg_len = sizeof(int) * 3 + P_KEY_LENGTH;
	ct_len=msg_len+EVP_CIPHER_CTX_block_size(this->ctx);
	chipertext=(char*)malloc(ct_len);

	select_random_key(sym_key, EVP_MAX_KEY_LENGTH);
	
	EVP_Encrypt_Init(this->ctx, EVP_des_ecb(), sym_key, NULL);
	
	EVP_EncryptUpdate(this->ctx, &chipertext[ct_ptr], &nc, (unsigned char*)src, sizeof(int));
	ct_ptr+=nc;
		
	EVP_EncryptUpdate(this->ctx, &chipertext[ct_ptr], &nc, (unsigned char*)dst, sizeof(int));
	ct_ptr+=nc;
	
	EVP_EncryptUpdate(this->ctx, &chipertext[ct_ptr], &nc, (unsigned char *)nonce, sizeof(int));
	ct_ptr+=nc;
	
	EVP_EncryptUpdate(this->ctx, &chipertext[ct_ptr], &nc, (unsigned char *)asym_key, strlen(asym_key));
	ct_ptr+=nc;
	
	if (ct_ptr!=msg_len)
		sys_err("Symmetric encryption error!");
		
	EVP_EncryptFinal(this->ctx, &ciphertext[ct_ptr], &nc);
	
	printf("Ciphertext: %s\n", ciphertext);
	
	return ciphertext;
}

void Sym_Encryption::sym_decrypt(const char* ciphertext, int src, int dst, int nonce, char* asym_key){
	char* plaintext;
	int msg_len;
	
	msg_len = sizeof(int) * 3 + P_KEY_LENGTH;
	plaintext=(char*)malloc(msg_len);
	bzero(plaintext, ct_len);
	
	
	

	return;
}
