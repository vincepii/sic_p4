#include "utilities.h"

#ifndef __mesgs_h__
#define __mesgs_h__

/*
	Mess:
	
	Classe che realizza la struttura di un oggetto messaggio da scambiare tra peer e con il kdc.
*/
class Mess {
	private:
		//mittente/destinatario indivisuati da id
		int src_id;
		int dest_id;
		
		//autoesplicativi
		int nonce;
		string msg;
	public:
	
		//costruttore per creare un messaggio con i parametri passati
		Mess(int src_id, int dest_id, int nonce, string cipher);
		
		//invia un messaggio sul socket passato come argomento
		void send_mes(int socket);

		//riceve un messaggio dal socket passato come argomento
		int receive_mes(int socket);
		
		//stampa messaggio
		void print();
		
		//estrazione campi messaggio
		string getCipher();
		int getDest_id();
		int getNonce();
		int getSrc_id();
		
		//stampa in esadecimale il cifrato contenuto
		void print_hex();
};

#endif
