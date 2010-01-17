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
		int src, int dst, int nonce, const unsigned char* asym_key, string iv)
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
	
	s_cipher = generic_encrypt((unsigned char*)sym_key, plaintext, msg_len, iv);

	free(plaintext);
	return s_cipher;
}

void Sym_Encryption::sym_decrypt(const unsigned char* sym_key, const string ciphertext, 
	int* src, int* dst, int* nonce, string& asym_key, string iv)
{
	int msg_len;
	int pt_ptr = 0;
	string plain;
	
	msg_len = 3 * sizeof(int) + P_KEY_LENGTH + 8;

	plain = generic_decrypt((unsigned char *)sym_key,
			(unsigned char*) ciphertext.data(), msg_len, iv);

	pt_ptr = 0;
	plain.copy((char*)src, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	plain.copy((char*)dst, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	plain.copy((char*)nonce, sizeof(int), pt_ptr);
	pt_ptr += sizeof(int);

	asym_key = plain.substr(pt_ptr, P_KEY_LENGTH);

	return;
}

string Sym_Encryption::generic_encrypt(unsigned char* k,
		unsigned char* msg, int msg_ll, string iv)
{
	unsigned char* ciphertext;
	int ct_len;
	int nc;
	string str;

	EVP_EncryptInit(this->ctx, EVP_aes_128_cbc(), k, (unsigned char*)iv.data());

	ct_len = msg_ll + EVP_CIPHER_CTX_block_size(this->ctx);
	ciphertext = (unsigned char *)malloc(ct_len);

	EVP_EncryptUpdate(this->ctx, ciphertext, &nc, msg, msg_ll);
	EVP_EncryptFinal(this->ctx, &ciphertext[nc], &nc);

	str.assign((const char *)ciphertext, ct_len);
	free (ciphertext);

	return str;
}

string Sym_Encryption::generic_decrypt(unsigned char* k,
		unsigned char* cipher, int msg_ll, string iv)
{
	unsigned char* plaintext;
	int nc;
	string str;

	plaintext = (unsigned char*)malloc(msg_ll);
	bzero(plaintext, msg_ll);

	EVP_DecryptInit(this->ctx, EVP_aes_128_cbc(), k, (unsigned char*)iv.data());
	EVP_DecryptUpdate(this->ctx, plaintext, &nc, cipher, msg_ll);
	EVP_DecryptFinal(this->ctx, plaintext, &nc);

	str.assign((const char *)plaintext, msg_ll);
	free (plaintext);
	return str;
}
