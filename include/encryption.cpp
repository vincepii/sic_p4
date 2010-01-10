#include "encryption.h"
#include <sstream>
#include <string>

Sym_Encryption::Sym_Encryption()
{
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
	int msg_len;
	int pt_ptr = 0;
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
<<<<<<< HEAD:include/encryption.cpp

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
cout << "Byte cifrati: " << ct_ptr << endl;	
	s_cipher.insert(0,	(char*)ciphertext, ct_ptr);
=======
>>>>>>> 07568abe2a3c8c1890e1016358a894fc95157b7f:include/encryption.cpp
	
	s_cipher = generic_encrypt((unsigned char*)sym_key, plaintext, msg_len);

	free(plaintext);
	return s_cipher;
}

void Sym_Encryption::sym_decrypt(const unsigned char* sym_key, const string ciphertext, 
	int* src, int* dst, int* nonce, string& asym_key)
{
	int msg_len;
	int pt_ptr = 0;
	string plain;
	
	msg_len = 3 * sizeof(int) + P_KEY_LENGTH + 8;

<<<<<<< HEAD:include/encryption.cpp
	//decifro
	EVP_DecryptInit(this->ctx, EVP_des_ecb(), sym_key, NULL);
	
//ct_len = msg_len + EVP_CIPHER_CTX_block_size(this->ctx);
//cipher=(unsigned char*)malloc(ct_len);
//bzero(cipher, ct_len);
//cipher=(unsigned char*)ciphertext.c_str();

	pt_ptr=0;
//while(pt_ptr <= ciphertext.length()){
	EVP_DecryptUpdate(this->ctx, &plaintext[pt_ptr], &nd, ((unsigned char*)ciphertext.data()), ciphertext.length());
//EVP_DecryptUpdate(this->ctx, &plaintext[pt_ptr], &nd, (unsigned char*)(ciphertext.substr(pt_ptr, ciphertext.length()-pt_ptr)).c_str(), ciphertext.length());
//EVP_DecryptUpdate(this->ctx, &plaintext[pt_ptr], &nd, &cipher[pt_ptr], ct_len);
	pt_ptr+=nd;
//}

//pt_ptr = 0;
	EVP_DecryptFinal(this->ctx, &plaintext[pt_ptr], &nd);
	pt_ptr+=nd;
cout << "Byte decifrati: " << pt_ptr << endl;


	pt_ptr=0;	//reinizializzo per lettura numero byte giusti dei vari campi	
	memcpy(src, &plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	memcpy(dst, &plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
	memcpy(nonce, &plaintext[pt_ptr], sizeof(int));
	pt_ptr+=sizeof(int);
	
//	asym_key.insert(0, &((const char)plaintext[pt_ptr]), P_KEY_LENGTH);
	asym_key.assign((const char*)&plaintext[pt_ptr], P_KEY_LENGTH);
=======
	plain = generic_decrypt((unsigned char *)sym_key,
			(unsigned char*) ciphertext.data(), msg_len);

	pt_ptr = 0;
	plain.copy((char*)src, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	plain.copy((char*)dst, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	plain.copy((char*)nonce, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	asym_key = plain.substr(pt_ptr, P_KEY_LENGTH);
>>>>>>> 07568abe2a3c8c1890e1016358a894fc95157b7f:include/encryption.cpp

	return;
}

string Sym_Encryption::generic_encrypt(unsigned char* k,
		unsigned char* msg, int msg_ll)
{
	unsigned char* ciphertext;
	int ct_len;
	int nc;
	string str;

	EVP_EncryptInit(this->ctx, EVP_des_cbc(), k, NULL);

	ct_len = msg_ll + EVP_CIPHER_CTX_block_size(this->ctx);
	ciphertext = (unsigned char *)malloc(ct_len);

	EVP_EncryptUpdate(this->ctx, ciphertext, &nc, msg, msg_ll);
	EVP_EncryptFinal(this->ctx, &ciphertext[nc], &nc);

	str.assign((const char *)ciphertext, ct_len);
	free (ciphertext);

	return str;
}

string Sym_Encryption::generic_decrypt(unsigned char* k,
		unsigned char* cipher, int msg_ll)
{
	unsigned char* plaintext;
	int nc;
	string str;

	plaintext = (unsigned char*)malloc(msg_ll);
	bzero(plaintext, msg_ll);

	EVP_DecryptInit(this->ctx, EVP_des_cbc(), k, NULL);
	EVP_DecryptUpdate(this->ctx, plaintext, &nc, cipher, msg_ll);
	EVP_DecryptFinal(this->ctx, plaintext, &nc);

	str.assign((const char *)plaintext, msg_ll);
	free (plaintext);
	return str;
}
