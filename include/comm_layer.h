#include "utilities.h"

#ifndef __mesgs_h__
#define __mesgs_h__

class Mess {
	private:
		int src_id;
		int dest_id;
		int nonce;
		unsigned char* cipher;
		int cipher_ll;
	public:
		//Quando viene creato un messaggio devono essere specificati
		//tutti gli argomenti
		Mess(int src_id, int dest_id, int nonce = 0,
				unsigned char* cipher = NULL, int chipher_ll = 0);
		void send_mes(int socket);
		int receive_mes(int socket);
		void print();
		unsigned char* getCipher();
		int getCipher_ll();
		int getDest_id();
		int getNonce();
		int getSrc_id();
		~Mess(){ delete[] this->cipher; };
};

#endif
