#include "encryption.h"
#include <sstream>
#include <string>

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
	return;
}

string Sym_Encryption::sym_encrypt(const unsigned char* sym_key,
		int src, int dst, int nonce, const unsigned char* asym_key)
{
	//unsigned char* ciphertext;
	int msg_len;
	//int ct_len;
	//int nc;			//numero byte effettivamente cifrati
	//int ct_ptr=0;	/*puntatore alla posizione di chipertext nella quale inserire i nuovi dati cifrati*/
	int pt_ptr=0;	/*puntatore alla posizione di plaintext 
					nella quale inserire i nuovi dati da cifrare*/
	unsigned char* plaintext;

	string s_cipher;

	msg_len = 3 * sizeof(int) + P_KEY_LENGTH;
	plaintext=(unsigned char *)malloc(msg_len);
	
	memcpy(&plaintext[pt_ptr], &src, sizeof(int));
	pt_ptr+=sizeof(int);
	memcpy(&plaintext[pt_ptr], &dst, sizeof(int));
	pt_ptr+=sizeof(int);
	memcpy(&plaintext[pt_ptr], &nonce, sizeof(int));
	pt_ptr+=sizeof(int);
	memcpy(&plaintext[pt_ptr], asym_key, P_KEY_LENGTH);

	//cifratura
	
	s_cipher = generic_encrypt((unsigned char*)sym_key, plaintext, msg_len);

//	EVP_EncryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);
//
//	ct_len=msg_len+EVP_CIPHER_CTX_block_size(this->ctx);
//	ciphertext=(unsigned char*)malloc(ct_len);
//
//	ct_ptr=0;
//	EVP_EncryptUpdate(this->ctx, &ciphertext[ct_ptr], &nc, plaintext, msg_len);
//	ct_ptr+=nc;
//
//	EVP_EncryptFinal(this->ctx, &ciphertext[ct_ptr], &nc);
//	ct_ptr+=nc;
//
//	s_cipher.insert(0,	(char*)ciphertext, ct_ptr);
	
//	printf("Ciphertext: \n");
//	for (unsigned int i=0; i<s_cipher.length(); i++)
//		printbyte(s_cipher.at(i));

	free(plaintext);
	//free(ciphertext);
	return s_cipher;
}

void Sym_Encryption::sym_decrypt(const unsigned char* sym_key, const string ciphertext, 
	int* src, int* dst, int* nonce, string& asym_key)
{
	
	//unsigned char* plaintext;
	int msg_len;
	//int nd;			//numero byte effettivamente decifrati
	int pt_ptr=0;	/*puntatore alla posizione di plaintext 
					nella quale inserire i nuovi dati decifrati*/
	
	//assegno una zona di memoria al plaintext
	msg_len = 3 * sizeof(int) + P_KEY_LENGTH + 8;
	//plaintext=(unsigned char*)malloc(msg_len);
	//bzero(plaintext, msg_len);
	string plain;

	//decifro
	//plaintext = (unsigned char*)generic_decrypt((unsigned char *)sym_key,
			//(unsigned char*) ciphertext.data(), msg_len).data();
	plain = generic_decrypt((unsigned char *)sym_key,
			(unsigned char*) ciphertext.data(), msg_len);
//	EVP_DecryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);
//
//	pt_ptr=0;
//	EVP_DecryptUpdate(this->ctx, &plaintext[pt_ptr], &nd, ((unsigned char*)ciphertext.data()), ciphertext.length());
//	pt_ptr+=nd;
//
//	EVP_DecryptFinal(this->ctx, &plaintext[pt_ptr], &nd);
//	pt_ptr+=nd;

	pt_ptr = 0;
	plain.copy((char*)src, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	plain.copy((char*)dst, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	plain.copy((char*)nonce, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	asym_key = plain.substr(pt_ptr, P_KEY_LENGTH);

//	pt_ptr=0;	//reinizializzo per lettura numero byte giusti dei vari campi
//	memcpy(src, &plaintext[pt_ptr], sizeof(int));
//	pt_ptr+=sizeof(int);
//
//	memcpy(dst, &plaintext[pt_ptr], sizeof(int));
//	pt_ptr+=sizeof(int);
//
//	memcpy(nonce, &plaintext[pt_ptr], sizeof(int));
//	pt_ptr+=sizeof(int);
//
//	asym_key.assign((const char*)&plaintext[pt_ptr], P_KEY_LENGTH);

	//free(plaintext);
	return;
}

string Sym_Encryption::generic_encrypt(unsigned char* k,
		unsigned char* msg, int msg_ll)
{
	unsigned char* ciphertext;
	int ct_len;
	int nc;
	string str;

	//EVP_CIPHER_CTX* ctx = (EVP_CIPHER_CTX *)malloc(sizeof(EVP_CIPHER_CTX));
	//EVP_CIPHER_CTX_init(ctx);

	EVP_EncryptInit(this->ctx, EVP_des_ecb(), k, NULL);

	ct_len = msg_ll + EVP_CIPHER_CTX_block_size(this->ctx);
	ciphertext = (unsigned char *)malloc(ct_len);

	/* cifratura in un colpo */
	EVP_EncryptUpdate(this->ctx, ciphertext, &nc, msg, msg_ll);
	/*contesto, crittogramma (memoria opportunamente allocata), parametro out
	 * del numero di byte scritti, messaggio da cifrare, lunghezza del plaintext*/

	EVP_EncryptFinal(this->ctx, &ciphertext[nc], &nc);

//	cout << "Ciphertext: " << endl;
//	for (int i = 0; i < ct_len; i++){
//		printbyte(ciphertext[i]);
//	}
//	cout << endl;

	str.assign((const char *)ciphertext, ct_len);
	return str;
}

string Sym_Encryption::generic_decrypt(unsigned char* k,
		unsigned char* cipher, int msg_ll)
{
	unsigned char* plaintext;
	int nc;
	string str;

	//EVP_CIPHER_CTX* ctx = (EVP_CIPHER_CTX *)malloc(sizeof(EVP_CIPHER_CTX));

	plaintext = (unsigned char*)malloc(msg_ll);
	bzero(plaintext, msg_ll);

	//EVP_CIPHER_CTX_init(ctx);
	EVP_DecryptInit(this->ctx, EVP_des_ecb(), k, NULL);

//	cout << "Ciphertext:" << endl;
//
//	for (int i = 0; i < length; i++){
//		printbyte(r[i]);
//	}
//	cout << endl << endl;

	EVP_DecryptUpdate(this->ctx, plaintext, &nc, cipher, msg_ll);
	EVP_DecryptFinal(this->ctx, plaintext, &nc);
	//cout << "Plaintext:" << endl << plaintext << endl;
	str.assign((const char *)plaintext, msg_ll);

	return str;
}
