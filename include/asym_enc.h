#include "utilities.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <fstream>

#ifndef __ASYM_ENC_H__
#define __ASYM_ENC_H__
/*
 * --------------------------RSA-----------------------------------------------
 * RSA_public_encrypt() -- ritornano numero di byte cifrati, padding incluso
 * RSA_private_decrypt() -- o -1 per errore
 * Ricordarsi RSA_free()
 *
 * int RSA_public_encrypt(int flen, unsigned char *from,
 * 					unsigned char to,RSA *rsa, int padding);
 * per la dimensione del buffer che contiene la cifratura, usare RSA_size(RSAobj)
 *
 * int RSA_private_decrypt(int flen, unsigned char *from,
 * 					unsigned char to,RSA *rsa, int padding);
 * Modalità di padding da usare: RSA_PKCS1_OAEP_PADDING
 *
 * -------------------------RSA in EVP-----------------------------------------
 * Oggetto EVP_PKEY, contenitore di oggetto RSA (contenitore di chiave RSA)
 * EVP_PKEY_new: crea oggetto EVP_PKEY
 * EVP_PKEY_free: distrugge oggetto EVP_PKEY <--------------------
 * EVP_PKEY_assign_RSA(EVP_PKEY, RSA), assegna la chiave RSA al PKEY
 *
 * Effettua la cifratura dei dati usando una chiave simmetrica e poi
 * usa la chiave pubblica della controparte per cifrare la chiave simmetrica.
 *
 *
 *-----------------------------------------------------------------------------
 *
 * NOTA: i peer devono possedere i file con la proprio chiave pubblica e
 * privata, la KDC deve possedere i file con la chiave privata dei due peer.
 * Generazione della chiave privata:
 * $ openssl genrsa -out privkey.pem
 * Generazione della chiave pubblica dalla chiave privata:
 * $ openssl rsa -in privkey.pem -pubout -out pubkey.pem
 *
 * Usare RSA *PEM_read_RSAPrivateKey e EVP_PKEY *PEM_read_RSAPublicKey per prelevare
 * le chiavi da file e compilare l'oggetto RSA
 *
 * man rsa
 * man 3 rsa
 *
 * */

using namespace std;

class As_enc {
private:
	string pubkey_file;
	string privkey_file;
	//cifrario creato da public_encrypt
	unsigned char* cipher;
	//plaintext ottenuto da private_decrypt
	unsigned char* plain;
public:
	As_enc(string pubfile, string privfile);
	int asym_enc(int src_id, int dst_id, int nonce1);
	int asym_enc(int src_id, int dst_id, int nonce1, int nonce2);
	int asym_dec(unsigned char* from, int number_of_int_carried);
	/**
	 * Legge gli interi contenuti nel plaintext decifrato
	 */
	void print_plain(int number_of_int_carried);
	~As_enc();
};

#endif
