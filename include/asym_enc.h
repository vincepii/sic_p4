#include "utilities.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <fstream>

#ifndef __ASYM_ENC_H__
#define __ASYM_ENC_H__
/*
 * Generazione della chiave privata:
 * $ openssl genrsa -out privkey.pem
 * Generazione della chiave pubblica dalla chiave privata:
 * $ openssl rsa -in privkey.pem -pubout -out pubkey.pem
 *
 * Usare RSA *PEM_read_RSAPrivateKey e EVP_PKEY *PEM_read_RSAPublicKey per prelevare
 * le chiavi da file e compilare l'oggetto RSA
 *
 * */

using namespace std;


/* As_enc:
   Classe che realizza la struttura di un oggetto per gestire la cifratura asimmetrica.
*/
class As_enc {
private:
	//file chiave pubblica
	string pubkey_file;
	
	//file chiave privata
	string privkey_file;
	
	//cifrario creato da public_encrypt
	string cipher;
	
	//plaintext ottenuto da private_decrypt
	string plain;
public:

	//costruttore per definire quali file contengono le chiavi (pub/priv) per la 
	//cifratura asimmetrica usare
	As_enc(string pubfile, string privfile);
	
	//funzioni di cifratura (overloading per supportare i diversi tipi di input)
	int asym_encr(int src_id, int dst_id, int nonce1);
	int asym_encr(int src_id, int dst_id, int nonce1, int nonce2);
	
	//funzione di decifratura (prende in input il ciphertext)
	int asym_decr(string from);
	
	//funzioni per stampa/estrazione dei contenuti
	void print_plain(int number_of_int_carried);
	string getCipher();
	string getPlain();
	void extract_integers(int* a, int* b, int* c);
	void extract_integers(int* a, int* b, int* c, int* d);
};

#endif
