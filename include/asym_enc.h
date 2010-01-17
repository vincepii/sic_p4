/**
 * @file asym_enc.h
 * Realizzazione della cifratura asimmetrica
 */

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
 */

using namespace std;

/**
 * Classe che implementa la cifratura asimmetrica
 */
class As_enc {
private:
	/** nome file contenente la chiave pubblica */
	string pubkey_file;
	
	/** nome file contenente la chiave privata */
	string privkey_file;
	
	/** crittogramma creato da public_encrypt */
	string cipher;
	
	/** plaintext ottenuto da private_decrypt */
	string plain;
public:
	/**
	 * Crea l'oggetto As_enc
	 *
	 * Inizializzazione dell'oggetto passando i nomi dei file
	 * contenenti la chiave pubblica e privata
	 *
	 * @param[in] pubfile Nome del file contenente la chiave pubblica
	 * @param[in] privfile Nome del file contenente la chiave privata
	 */
	As_enc(string pubfile, string privfile);
	
	/**
	 * Cifra i paramentri passati in argomento
	 *
	 * Usata per il messaggio M6, che ha questo formato
	 *
	 * @param[in] src_id ID del mittente
	 * @param[in] dst_id ID del destinatario
	 * @param[in] nonce1 Nonce
	 */
	int asym_encr(int src_id, int dst_id, int nonce1);

	/**
	 * Cifra i paramentri passati in argomento
	 *
	 * Usata per i messaggi M7 e M8, che hanno questo formato
	 *
	 * @param[in] src_id ID del mittente
	 * @param[in] dst_id ID del destinatario
	 * @param[in] nonce1 Nonce
	 * @param[in] nonce2 Nonce
	 */
	int asym_encr(int src_id, int dst_id, int nonce1, int nonce2);
	
	/**
	 * Decifra il crittogramma passato come argomento
	 *
	 * @param from Crittogramma da decifrare
	 */
	int asym_decr(string from);
	
	/**
	 * Stampa il contenuto del plaintext
	 */
	void print_plain(int number_of_int_carried);

	/**
	 * Getter
	 *
	 * @return Crittogramma contenuto nell'oggetto As_enc
	 */
	string getCipher();

	/**
	 * Getter
	 *
	 * @return Plaintext contenuto nell'oggetto As_enc
	 */
	string getPlain();

	/**
	 * Estrae dal plaintext gli interi contenuti (il plaintext è una stringa)
	 *
	 * @param[out] *a Primo intero
	 * @param[out] *b Secondo intero
	 * @param[out] *c Terzo intero
	 */
	void extract_integers(int* a, int* b, int* c);

	/**
	 * Estrae dal plaintext gli interi contenuti (il plaintext è una stringa)
	 *
	 * @param[out] *a Primo intero
	 * @param[out] *b Secondo intero
	 * @param[out] *c Terzo intero
	 * @param[out] *d Quarto intero
	 */
	void extract_integers(int* a, int* b, int* c, int* d);
};

#endif
