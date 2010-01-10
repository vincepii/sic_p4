#include "utilities.h"

/** @file encryption.h
 * File containing encryption/decryption functions and data structures.
 *
 * */
#ifndef __encryption_h__
#define __encryption_h__

class Sym_Encryption{
	private:
		//context
		EVP_CIPHER_CTX* ctx;

	public:
		//constructor
		Sym_Encryption();
		
		//destructor
		~Sym_Encryption();
		
		/**
		 * chiave simmetrica, ids e nonce, chiave asimm
		 */
		string sym_encrypt(const unsigned char* k, int A, int B,
				int n, const unsigned char* pub);
		
		/**
		 * Chiave, cipher, parametri out (ids, nonce, chiave)
		 */
		void sym_decrypt(const unsigned char* k, const string cptxt, int* ID1,
				int* ID2, int* nonce, string& pub);

		string generic_encrypt(unsigned char* k, unsigned char* msg, int msg_ll);
		string generic_decrypt(unsigned char* k, unsigned char* cipher, int msg_ll);

};


#endif
