/** @file utilities.h
 * File containing utility functions.
 *
 * This file contains utility functions.
 * */
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

using namespace std;

/** @def P_KEY_LENGTH
 * Public key length in bytes.
 * */
#define P_KEY_LENGTH 272

/** @def BACKLOG
 * Backlog queue for listen() function.
 * */
#define BACKLOG 64

/** @def CAST_ADDR(x)
 * Macro used to cast the argument x to struct sockaddr *
 */
#define CAST_ADDR(x) (struct sockaddr *)(x)

/** @fn void user_err (const char *s)
 * Reports an error using std::out and quits (exit(-1))
 *
 * @param[in] s The string that describes the error
 */
void user_err (const char *s);

/** @fn void sys_err (const char *s)
 * Reports an error using perror() and quits (exit(-1))
 *
 * @param[in] s The string that describes the error
 */
void sys_err (const char *s);

/** @fn int init_sd(int port)
 * Initializes a socket for a server connection.
 *
 * Perform socket operations:
 * - socket()
 * - bind()
 * - listen()
 *
 * @param[in] port The port used to reach the server
 */

int init_sd(int port);

/**
 * @fn int safe_read(int sd, void *arg, int len)
 * Ensures all bytes have been read.
 *
 * It performs a loop until all data length has been read.
 * @param[in] sd The socket descriptor.
 * @param[out] *arg The buffer to write to.
 * @param[in] len Number of bytes to be read.
 */
int safe_read(int sd, void *arg, int len);

void printbyte (char b);

void select_random_key (unsigned char* k, int b);

void hsh(int a, int b, string hf,unsigned char** sk, int* len);

string generic_encrypt(unsigned char* k, unsigned char* msg, int msg_ll);

string generic_decrypt(unsigned char* k, unsigned char* cipher, int msg_ll);

#endif
