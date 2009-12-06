#include "encryption.h"
#include <sstream>

Sym_Encryption::Sym_Encryption()
{
	//context initialization
	this->ctx=(EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(this->ctx);
	return;
}


Sym_Encryption::~Sym_Encryption()
{
	if (EVP_CIPHER_CTX_cleanup(this->ctx)==0)
		sys_err("Context deallocation error!");
	printf("in distr\n");
	return;
}

unsigned char* Sym_Encryption::sym_encrypt(const unsigned char* sym_key,
		int src, int dst, int nonce, const unsigned char* asym_key)
{
	unsigned char* ciphertext;
	int msg_len;
	int ct_len;
	int nc;			//numero byte effettivamente cifrati
	int ct_ptr=0;	/*puntatore alla posizione di chipertext 
					nella quale inserire i nuovi dati cifrati*/
	//int intTostr_len=0;
	
	//conversione interi in caratteri
	string s;
	stringstream out;
	out << src;
	out << dst;
	out << nonce;
	out << asym_key;
	s = out.str();
	msg_len=strlen((const char*)s.c_str());

//	msg_len = intTostr_len + P_KEY_LENGTH;

	EVP_EncryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);

	ct_len=msg_len+EVP_CIPHER_CTX_block_size(this->ctx);
	ciphertext=(unsigned char*)malloc(ct_len);

	
	
cout<<endl<<"prova valore 1: "<<s<<"."<<endl;
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, (unsigned char *)s.c_str(), msg_len);
	ct_ptr+=nc;

/*	out << dst;
	s = out.str();
cout<<endl<<"prova valore 2: "<<s;
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, (unsigned char*)s.c_str(), sizeof(int));
	ct_ptr+=nc;
	
	out << nonce;
	s = out.str();
cout<<endl<<"prova valore 3: "<<s<<endl;
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, (unsigned char *)s.c_str(), sizeof(int));
	ct_ptr+=nc;

	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, asym_key, strlen((const char*)asym_key));
	ct_ptr+=nc;*/
	
	EVP_EncryptFinal(this->ctx, &ciphertext[ct_ptr], &nc);
	ct_ptr+=nc;
printf("msg_len: %d --- ct_len: %d --- ct_ptr: %d\n", msg_len,ct_len, ct_ptr);
	//********* tolta perchè da sempre qualcosa in meno della dim max. A meno che non becchi il caso
	//********* in cui sono proprio identici. Quindi da sempre errore anche se non c'è!!
	/*
	printf("ct_ptr: %d, ct_len: %d\n", ct_ptr, ct_len);
	if (ct_ptr!=ct_len)
		sys_err("Symmetric encryption error!");*/
	
	printf("CCiphertext: \n");
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
	int pt_ptr=0;	/*puntatore alla posizione di plaintext 
					nella quale inserire i nuovi dati decifrati*/
	
	msg_len = strlen((const char*)ciphertext);
	plaintext=(unsigned char*)malloc(msg_len);
	bzero(plaintext, msg_len);
	
	EVP_DecryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);
	
	EVP_DecryptUpdate(this->ctx, &plaintext[pt_ptr], &nd, ciphertext, strlen((const char*)ciphertext));
	pt_ptr+=nd;

	//if(nd!=(int)strlen((const char*)ciphertext))
	//	sys_err("Symmetric decryption error!");
	
	EVP_DecryptFinal(this->ctx, &plaintext[pt_ptr], &nd);
	pt_ptr+=nd;
	printf("pt_ptr: %d\n", pt_ptr);
	
	
	printf("PPlaintext: %s\n", plaintext);
	printf("PPlaintext senza padding: ");
	for (int i=0; i<pt_ptr; i++)
		cout<<plaintext[i];
	printf("\n");
	
	pt_ptr=0;	//reinizializzo per lettura numero byte giusti dei vari campi
	
	
	memcpy((void*)src, (const void*)plaintext[pt_ptr], sizeof(int));
	printf("ora si\n");
	pt_ptr+=sizeof(int);
	
	memcpy((void*)dst, (const void*)plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	memcpy((void*)nonce, (const void*)plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
		
	memcpy((void*)asym_key, (const void*)plaintext[pt_ptr], P_KEY_LENGTH);
	
	free(plaintext);
	
	return;
}
