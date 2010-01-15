#include "utilities.h"

/** @file encryption.h
 * File containing encryption/decryption functions and data structures.
 *
 * */
#ifndef __encryption_h__
#define __encryption_h__

/*
	Sym_Encryption:
	
	Classe che permette di creare oggetti per gestire la cifratira simmetrica.
*/

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
		 * Funzione di cifratura che inizializza i parametri da usare
		 * quando la struttura del messaggio da cifrare usa i campi definiti come argomenti.
		 *
		 * chiave simmetrica, ids e nonce, chiave asimm
		 */
		string sym_encrypt(const unsigned char* k, int A, int B,
				int n, const unsigned char* pub);
		
		/**
		 * Funzione di decifratura che preleva i dati nel messaggio
		 * quando la struttura di quest'ultimo usa i campi definiti come argomenti.
		 *
		 * Chiave, cipher, parametri out (ids, nonce, chiave)
		 */
		void sym_decrypt(const unsigned char* k, const string cptxt, int* ID1,
				int* ID2, int* nonce, string& pub);

		//funzioni che realizzano il corpo della cifratura/decifratura
		string generic_encrypt(unsigned char* k, unsigned char* msg, int msg_ll);
		string generic_decrypt(unsigned char* k, unsigned char* cipher, int msg_ll);

};


#endif
