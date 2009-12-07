#include "utilities.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>

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
 * Usare RSA *PEM_read_RSAPrivateKey e EVP_PKEY *PEM_read_PUBKEY per prelevare
 * le chiavi da file e compilare l'oggetto RSA
 *
 * man rsa
 * man 3 rsa
 *
 * */

class As_enc {
private:
	RSA rsa_obj;
public:
	As_enc();
	string asym_enc(int src_id, int dst_id, char nonce1, char nonce2 = 0);
};

#endif
