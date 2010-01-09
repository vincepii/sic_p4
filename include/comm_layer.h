#include "utilities.h"

#ifndef __mesgs_h__
#define __mesgs_h__

class Mess {
	private:
		int src_id;
		int dest_id;
		int nonce;
		string msg;
	public:
		Mess(int src_id, int dest_id, int nonce, string cipher);
		void send_mes(int socket);
		int receive_mes(int socket);
		void print();
		string getCipher();
		int getDest_id();
		int getNonce();
		int getSrc_id();
		void print_hex();
};

#endif
