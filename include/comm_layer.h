/**
 * @file comm_layer.h
 * Scambio messaggi su socket.
 */

#include "utilities.h"

#ifndef __mesgs_h__
#define __mesgs_h__

/**
 * Classe che realizza la struttura di un oggetto messaggio.
 *
 * Struttura generale di un oggetto messaggio, saranno usati solamente i
 * campi necessari allo specifico messaggio da inviare.
 */
class Mess {
	private:
		/** ID del mittente */
		int src_id;
		/** ID del destinatario */
		int dest_id;
		/** Nonce */
		int nonce;
		/** Initialization vector */
		string iv;
		/** Corpo del messaggio (tipicamente sarà un crittogramma) */
		string msg;
	public:
	
		/**
		 * Costruttore dell'oggetto Mess.
		 *
		 * @param[in] src_id ID del mittente.
		 * @param[in] dest_id ID del destinatario.
		 * @param[in] nonce Nonce.
		 * @param[in] iv Initialization vector.
		 * @param[in] cipher Crittogramma.
		 */
		Mess(int src_id, int dest_id, int nonce, string iv, string cipher);
		
		/**
		 * Invia un oggetto messaggio sul socket.
		 *
		 * Serializza l'oggetto Mess ed invia sul socket ogni suo campo.
		 *
		 * @param[in] socket Descrittore del socket.
		 */
		void send_mes(int socket);

		/**
		 * Riceve un oggetto messaggio da socket.
		 *
		 * Vengono ricevuti uno ad uno i campi dell'oggetto Mess, e viene
		 * compilato l'oggetto su cui è chiamato il metodo.
		 *
		 * @param[in] socket Descrittore del socket.
		 */
		int receive_mes(int socket);
		
		/**
		 * Stampa i campi dell'oggetto Mess.
		 */
		void print();
		
		/**
		 * Getter per il ciphertext.
		 *
		 * @return msg.
		 */
		string getCipher();

		/**
		 * Getter per iv.
		 *
		 * @return iv.
		 */
		string getIv();

		/**
		 * Getter per dest_id.
		 *
		 * @return dest_id.
		 */
		int getDest_id();

		/**
		 * Getter per nonce.
		 *
		 * @return nonce.
		 */
		int getNonce();

		/**
		 * Getter per src_id.
		 *
		 * @return src_id.
		 */
		int getSrc_id();
		
		/**
		 * Stampa il messaggio, formattando in esadecimale il campo msg.
		 */
		void print_hex();
};

#endif
