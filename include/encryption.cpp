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

string Sym_Encryption::sym_encrypt(const unsigned char* sym_key,
		int src, int dst, int nonce, const unsigned char* asym_key)
{
	unsigned char* ciphertext;
	int msg_len;
	int ct_len;
	int nc;			//numero byte effettivamente cifrati
	int ct_ptr=0;	/*puntatore alla posizione di chipertext 
					nella quale inserire i nuovi dati cifrati*/
	int pt_ptr=0;	/*puntatore alla posizione di plaintext 
					nella quale inserire i nuovi dati da cifrare*/
	unsigned char* plaintext;
	string s;

	//assegno una zona di memoria al plaintext e lo riempo con i dati che lo compongono
	msg_len = 3* sizeof(int) + P_KEY_LENGTH;
	plaintext=(unsigned char*)malloc(msg_len);
	
	memcpy(&plaintext[pt_ptr], &src, sizeof(int));
	pt_ptr+=sizeof(int);
	memcpy(&plaintext[pt_ptr], &dst, sizeof(int));
	pt_ptr+=sizeof(int);
	memcpy(&plaintext[pt_ptr], &nonce, sizeof(int));
	pt_ptr+=sizeof(int);
	memcpy(&plaintext[pt_ptr], asym_key, P_KEY_LENGTH);

	//cifratura
	EVP_EncryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);

	//assegno una zona di memoria al ciphertext
	ct_len=msg_len+EVP_CIPHER_CTX_block_size(this->ctx);
	ciphertext=(unsigned char*)malloc(ct_len);

	ct_ptr=0;
	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, plaintext, msg_len);
	ct_ptr+=nc;

	EVP_EncryptFinal(this->ctx, &ciphertext[ct_ptr], &nc);
	ct_ptr+=nc;
	
	s.insert(0,	(char*)ciphertext, ct_ptr);
	printf("dime s: %d\n\n",s.length());
	
printf("msg_len: %d --- ct_len: %d --- ct_ptr: %d\n", msg_len, ct_len, ct_ptr);
	//********* tolta perchè da sempre qualcosa in meno della dim max. A meno che non becchi il caso
	//********* in cui sono proprio identici. Quindi da sempre errore anche se non c'è!!
	/*
	printf("ct_ptr: %d, ct_len: %d\n", ct_ptr, ct_len);
	if (ct_ptr!=ct_len)
		sys_err("Symmetric encryption error!");*/
	
	printf("CCiphertext: \n");
	for (unsigned int i=0; i<s.length(); i++)
		printbyte(s.at(i));
	free(plaintext);
	///la free??????
	return s;
}

void Sym_Encryption::sym_decrypt(const unsigned char* sym_key, const string ciphertext, 
	int* src, int* dst, int* nonce, string& asym_key)
{
	
	unsigned char* plaintext;
	int msg_len;
	int nd;			//numero byte effettivamente decifrati
	int pt_ptr=0;	/*puntatore alla posizione di plaintext 
					nella quale inserire i nuovi dati decifrati*/
	
	printf("cifrato a destinazione: \n");
	for (unsigned int i=0; i<ciphertext.length(); i++)
		printbyte(ciphertext.at(i));
	printf("\n CON ct_len: %d\n",ciphertext.length());

//-----
	msg_len = 3* sizeof(int) + P_KEY_LENGTH;
	plaintext=(unsigned char*)malloc(msg_len);
	bzero(plaintext, msg_len);
	
	unsigned char apg[ciphertext.length()];
	memcpy(apg, ciphertext.c_str(),ciphertext.length());
	//for (unsigned int i=0; i<ciphertext.length(); i++)
	//	printbyte(apg[i]);
	
	EVP_DecryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);
	
	pt_ptr=0;
	EVP_DecryptUpdate(this->ctx, &plaintext[pt_ptr], &nd, apg, ciphertext.length());
	pt_ptr+=nd;
	
	EVP_DecryptFinal(this->ctx, &plaintext[pt_ptr], &nd);
	pt_ptr+=nd;
printf("msg_len: %d --- ct_len: %d --- pt_ptr: %d\n", msg_len, ciphertext.length(), pt_ptr);

//---

/*	pt_ptr=0;	//reinizializzo per lettura numero byte giusti dei vari campi
	
	memcpy(src, &plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	memcpy(dst, &plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	memcpy(nonce, &plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	asym_key.insert(0, (char*)&plaintext[pt_ptr], P_KEY_LENGTH);*/
	
	//free(plaintext);
	
	return;
}
