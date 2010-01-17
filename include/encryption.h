/**
 * @file encryption.h
 * Contiene le funzioni e le strutture dati per la (de)cifratura simmetrica.
 */
#include "utilities.h"
#ifndef __encryption_h__
#define __encryption_h__

/**
 * Classe per la gestione della (de)cifratura simmetrica.
 */
class Sym_Encryption{
	private:
		/** Contesto */
		EVP_CIPHER_CTX* ctx;

	public:
		/**
		 * Costruttore, inizializza il contesto.
		 */
		Sym_Encryption();
		
		/**
		 * Distruttore, invoca la cleanup sul contesto.
		 */
		~Sym_Encryption();
		
		/**
		 * Crea un crittogramma cifrando i paramatri passati in argomento.
		 *
		 * @param[in] *k Chiave simmetrica per cifrare.
		 * @param[in] A ID dell'entità A.
		 * @param[in] B ID dell'entità B.
		 * @param[in] n nonce.
		 * @param[in] *pub Chiave asimmetrica da fornire.
		 * @param[in] iv Initialization Vector per cbc.
		 * @return Crittogramma prodotto dall'operazione di cifratura.
		 * @see generic_encrypt().
		 */
		string sym_encrypt(const unsigned char* k, int A, int B,
				int n, const unsigned char* pub, string iv);
		
		/**
		 * Decifra un crittogramma ed estrae il contenuto sui parametri di output.
		 *
		 * @param[in] k Chiave simmetrica per decifrare.
		 * @param[in] cptxt Crittogramma da decifrare.
		 * @param[out] *ID1 Conterrà l'id dell'utente 1 decifrato.
		 * @param[out] *ID2 Conterrà l'id dell'utente 2 decifrato.
		 * @param[out] *nonce Conterrà il nonce decifrato.
		 * @param[out] &pub Conterrà la chiave pubblica decifrata.
		 * @param[in] iv Initialization Vector per cbc.
		 * @see generic_decrypt().
		 */
		void sym_decrypt(const unsigned char* k, const string cptxt, int* ID1,
				int* ID2, int* nonce, string& pub, string iv);

		/**
		 * Realizza la funzione di cifratura di base.
		 *
		 * @param[in] *k Chiave simmetrica.
		 * @param[in] *msg Messaggio da cifrare.
		 * @param[in] msg_ll Lunghezza del plaintext.
		 * @param[in] iv Initialization Vector per cbc.
		 * @return Ciphertext ottenuto con la cifratura.
		 */
		string generic_encrypt(unsigned char* k, unsigned char* msg,
				int msg_ll, string iv);

		/**
		 * Realizza la funzione di decifratura di base.
		 *
		 * @param[in] *k Chiave simmetrica.
		 * @param[in] *cipher Crittogramma da decifrare.
		 * @param[in] msg_ll Lunghezza del plaintext.
		 * @param[in] iv Initialization Vector per cbc.
		 * @return Plaintext ottunuto con la decifratura.
		 */
		string generic_decrypt(unsigned char* k, unsigned char* cipher,
				int msg_ll, string iv);

};


#endif
