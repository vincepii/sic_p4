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
		string sym_encrypt(const unsigned char*, int, int, int, const unsigned char*);
		
		/**
		 * Chiave, cipher, parametri out (ids, nonce, chiave)
		 */
		void sym_decrypt(const unsigned char*, const string, int*, int*, int*, string&);

};


#endif
