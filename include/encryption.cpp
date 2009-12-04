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

char* Sym_Encryption::sym_encrypt(int src, int dst, int nonce, const char* key){
	unsigned char* chipertext;
	int msg_len;
	int ct_len;
	int nc;			//numero byte effettivamente cifrati
	int ct_ptr=0;	/*puntatore alla posizione di chipertext 
					nella quale inserire i nuovi dati cifrati*/
	
	msg_len = sizeof(int) * 3 + strlen(key);
	ct_len=msg_len+EVP_CIPHER_CTX_block_size(this->ctx);
	chipertext=(unsigned char*)malloc(ct_len);
	
	EVP_EncryptUpdate(ctx, &chipertext[ct_ptr], &nc, (unsigned char*)src, sizeof(int));
	ct_ptr+=nc;
		
	EVP_EncryptUpdate(ctx, &chipertext[ct_ptr], &nc, (unsigned char*)dst, sizeof(int));
	ct_ptr+=nc;
	
	EVP_EncryptUpdate(ctx, &chipertext[ct_ptr], &nc, (unsigned char *)nonce, sizeof(int));
	ct_ptr+=nc;
	
	EVP_EncryptUpdate(ctx, &chipertext[ct_ptr], &nc, (unsigned char *)key, strlen(key));
	ct_ptr+=nc;
	
	
	return 0;
}
