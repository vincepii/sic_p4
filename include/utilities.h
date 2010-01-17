/** @file utilities.h
 * Contiene funzioni di utilità
 **/
#ifndef __utilities_h__
#define __utilities_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>
#include <pthread.h>
#include <string>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sys/stat.h>
#include <time.h>

using namespace std;

/** @def H_24
 * 24 ore in secondi
 * */
#define H_24 86400

/** @def P_KEY_LENGTH
 * Lunghezza in byte della chiave pubblica
 * */
#define P_KEY_LENGTH 272

/** @def BACKLOG
 * Coda di backlog per la listen()
 * */
#define BACKLOG 64

/** @def CAST_ADDR(x)
 * Cast dell'argomento x su struct sockaddr *
 */
#define CAST_ADDR(x) (struct sockaddr *)(x)

/** @fn void user_err (const char *s)
 * Riporta errore su std::out ed esce (exit(-1))
 *
 * @param[in] s Stringa che descrive l'errore
 */
void user_err (const char *s);

/** @fn void sys_err (const char *s)
 * Riporta un errore usando la perror() ad esce (exit(-1))
 *
 * @param[in] s Stringa che descrive l'errore
 */
void sys_err (const char *s);

/** @fn int init_sd(int port)
 * Inizializza socket per la connessione
 *
 * Esegue le operazioni:
 * - socket()
 * - bind()
 * - listen()
 *
 * @param[in] port La porta da usare per raggiungere il server
 */

int init_sd(int port);

/**
 * @fn int safe_read(int sd, void *arg, int len)
 * Esegue una lettura sicura (tutti i byte vengono letti)
 *
 * @param[in] sd Descrittore del socket
 * @param[out] *arg Buffer su cui scrivere
 * @param[in] len Numero di dati da leggere
 */
int safe_read(int sd, void *arg, int len);

/** @fn void printbyte (char b)
 * Stampa un byte in esadecimale
 *
 * @param[in] b Byte da stampare
 */
void printbyte (char b);

/** @fn void select_random_key (unsigned char* k, int b)
 * Crea una quantità random lunga b byte
 *
 * @param[out] *k Chiave random creata
 * @param[in] b Numero di byte da scrivere
 */
void select_random_key (unsigned char* k, int b);


/**
 * @fn void hsh(int a, int b, string hf, unsigned char** shared_key, int* hash_len)
 * Unisce i segreti dei peer con una hash per calcolare la chiave di sesssione
 *
 * @param[in] a Segreto del peer A
 * @param[in] b Segreto del peer B
 * @param[in] hf Funzione hash da usare
 * @param[out] *shared_key Chiave di sessione
 * @param[out] *hash_len Dimensione dell'output della funzione hash
 */
void hsh(int a, int b, string hf, unsigned char** shared_key, int* hash_len);

/**
 * Ritorna l'istante di ultima modifica del file
 */
time_t last_mod_time(const char* filename);

#endif
