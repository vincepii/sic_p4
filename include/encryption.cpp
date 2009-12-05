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

unsigned char* Sym_Encryption::sym_encrypt(const unsigned char* sym_key, int src, int dst, int nonce, const unsigned char* asym_key){
	unsigned char* ciphertext;
	int msg_len;
	int ct_len;
	int nc;			//numero byte effettivamente cifrati
	int ct_ptr=0;	/*puntatore alla posizione di chipertext 
					nella quale inserire i nuovi dati cifrati*/
	
	msg_len = sizeof(int) * 3 + P_KEY_LENGTH;
	ct_len=msg_len+EVP_CIPHER_CTX_block_size(this->ctx);
	ciphertext=(unsigned char*)malloc(ct_len);
	
	EVP_EncryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);
	
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, (unsigned char*)src, sizeof(int));
	ct_ptr+=nc;
		
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, (unsigned char*)dst, sizeof(int));
	ct_ptr+=nc;
	
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, (unsigned char *)nonce, sizeof(int));
	ct_ptr+=nc;
	
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, asym_key, strlen((const char*)asym_key));
	ct_ptr+=nc;
	
	if (ct_ptr!=msg_len)
		sys_err("Symmetric encryption error!");
		
	EVP_EncryptFinal(this->ctx, &ciphertext[ct_ptr], &nc);
	
	printf("Ciphertext: ");
	for (unsigned int i=0; i<strlen((const char*)ciphertext); i++)
		printbyte(ciphertext[i]);
	
	printf("\n");
	///la free??????
	return ciphertext;
}

void Sym_Encryption::sym_decrypt(const unsigned char* sym_key, const unsigned char* ciphertext, int* src, int* dst, int* nonce, unsigned char* asym_key){
	unsigned char* plaintext;
	int msg_len;
	int nd;			//numero byte effettivamente decifrati
	int pt_ptr=0;
	
	msg_len = sizeof(int) * 3 + P_KEY_LENGTH;
	plaintext=(unsigned char*)malloc(msg_len);
	bzero(plaintext, msg_len);
	
	EVP_DecryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);
	
	EVP_DecryptUpdate(this->ctx, plaintext, &nd, ciphertext, strlen((const char*)ciphertext));

	if(nd!=(int)strlen((const char*)ciphertext))
		sys_err("Symmetric decryption error!");
	
	EVP_DecryptFinal(this->ctx, plaintext, &nd);
	
	printf("Plaintext: %s\n", plaintext);
	
	memcpy((void*)src, (const void*)plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	memcpy((void*)dst, (const void*)plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	memcpy((void*)nonce, (const void*)plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
		
	memcpy((void*)asym_key, (const void*)plaintext[pt_ptr], P_KEY_LENGTH);
	
	free(plaintext);
	
	return;
}