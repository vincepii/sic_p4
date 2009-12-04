#include "utilities.h"

#ifndef __mesgs_h__
#define __mesgs_h__

class Message {
	private:
		int src_id;
		int dest_id;
		int nonce;
		unsigned char* cipher;
		int cipher_ll;
	public:
		//Quando viene creato un messaggio devono essere specificati
		//tutti gli argomenti
		Message(int src_id, int dest_id, int nonce = 0,
				unsigned char* cipher = NULL, int chipher_ll = 0);
		int send_mes(int socket);
		int receive_mes(int socket);
};

#endif
