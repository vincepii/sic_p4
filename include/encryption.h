#include "utilities.h"

/** @file encryption.h
 * File containing encryption/decryption functions and data structures.
 *
 * */
#ifndef __encryption_h__
#define __encryption_h__

class Sym_Encryption{
	private:
		//ricordarsi di fare il distr. perchè il 
		//contesto va sempre riaggiornato e per ripulire la memoria

		//context
		EVP_CIPHER_CTX* ctx;


	public:
		//constructor
		Sym_Encryption();
		
		//destructor
		~Sym_Encryption();
		
		//encryption between a peer and kdc
		/**
		 * chiave simmetrica, ids e nonce, chiave asimm
		 */
		unsigned char* sym_encrypt(const unsigned char*, int, int, int, const unsigned char*);
		
		//decryption beetween a peer and kdc
		/*
		 * Chiave, cipher, parametri out (ids, nonce, chiave)
		 */
		void sym_decrypt(const unsigned char*, const unsigned char*, int*, int*, int*, unsigned char*);

};


#endif
