#include "../include/utilities.h"
#include "../include/comm_layer.h"
#include "../include/asym_enc.h"
#include "../include/encryption.h"
#include <fstream>

using namespace std;

#define B_ID 2
#define SYM_KEY_FILE "KBT"
#define PRIV_KEY_FILE "privkey.pem"
#define A_PUB_KEY_FILE "A_pubkey.pem"

int main (int argc, char* argv[])
{
	//-------------------------------------------------------------------------
	//PREPARAZIONE SOCKET PER COLLEGAMENTO CON KDC

	struct sockaddr_in addr;
	int server_port;
	int kdc_socket;
	string server_ip;

	if (argc < 3) user_err("usage: bpeer <server_ip> <server_port> <local port>");

	server_ip = argv[1];
	server_port = atoi(argv[2]);
	if (server_ip == "127.0.0.1") server_ip = "localhost";

	//tolto qui

	//-------------------------------------------------------------------------
	//PREPARAZIONE SOCKET PER COLLEGAMENTO CON A

	int port;

	port = atoi(argv[3]);

	int rec_socket, curr_sd;
	struct sockaddr_in c_add;
	unsigned int addrlen;

	rec_socket = init_sd(port);
	addrlen = sizeof(c_add);

	curr_sd = accept(rec_socket, CAST_ADDR(&c_add), &addrlen);
	if(curr_sd < 0) sys_err("B: accept failed");

	//-------------------------------------------------------------------------

	/*
	 * Operazioni di B:
	 * 1. Riceve M1 da A
	 * 2. Contatta la kdc per avere la chiave pubblica di A (M4)
	 * 3. Riceve M5 e salva la chiave pubblica di A
	 * 4. Riceve M6
	 * 5. Invia M7
	 * 6. Riceve M8
	 */

	int A = 0;
	int B = 0;
	int Nb = 0;
	int check1 = 0;
	int check2 = 0;
	int check3 = 0;
	int check4 = 0;
	string cipher;
	string sym_key;
	string A_asym_key;
	ifstream kfile;
	ofstream as_k_file;
	string as_cipher;
	unsigned char* shared_key = NULL;
	int as_a_nonce = 0;
	int as_b_nonce = 0;
	
	cout << "[B]: Running..." << endl;

	//ricezione di M1 e controllo sul mio id
	Mess M1(0,0,0,"");
	M1.receive_mes(curr_sd);
	A = M1.getSrc_id();
	B = M1.getDest_id();

	cout << "[B]: ricevuto M1" << endl;
	if (B != B_ID){
		// A non vuole comunicare con B
		cout << "[B]: ricevuto M1 con dest_id " << B << endl;
		return -1;
	}
	
	//controllo se il peer ha il file della chiave pubblica del peer che lo contatta
	//da più di 24 ore. In tal caso deve richiederlo, altrimenti la chiave è ancora valida
	//e può usare quella che ha.

	time_t file_time=0;									//last modified date of the file 
														//(in seconds from 1970)
														
	//get last modified date only if the file exists
	as_k_file.open(A_PUB_KEY_FILE, ios::in | ios::binary);
	if (as_k_file.is_open()){
		file_time = last_mod_time(A_PUB_KEY_FILE);			//get last modified date
	}

	//get local time
	time_t actual_time;									//actual time in seconds from 1970
	time(&actual_time);									//get actual time
	
	//se file_time==0 perchè non ho il file non stampare la data (non significativa)
	if (file_time==0)
		printf("***NON POSSEGGO CHIAVE PUBBLICA DI A***\n");
	else
		printf("DATA RILASCIO CHIAVE PUBBLICA DI a: %s\n", ctime(&file_time));
	printf("ORA/DATA ATTUALE: %s\n", ctime(&actual_time));

	srand( time (NULL)  + 10);

	//comparison between dates
	if (!as_k_file.is_open() || (actual_time>(file_time+H_24))){
		printf("***RECUPERO CHIAVE PUBBLICA DI A***\n");
	
		kdc_socket = socket(AF_INET, SOCK_STREAM, 0);
		bzero(&addr, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(server_port);
		inet_pton(AF_INET, server_ip.data(), &addr.sin_addr.s_addr);

		if (connect(kdc_socket, CAST_ADDR(&addr), sizeof(struct sockaddr_in)) < 0)
			sys_err("B: connection error");
			
		//validità chiave pubblica in possesso scaduta
		//recupero nuova chiave pubblica del peer A
		if (as_k_file.is_open())
			as_k_file.close();	

		//creazione ed invio del messaggio M4
		
		Nb = rand() % 1000 + 1;
		Mess M4(B_ID, A, Nb, "");
		M4.send_mes(kdc_socket);
	
		cout << "[B]: inviato M4" << endl;

		//ricezione del messaggio M5 e controlli sugli id
		Mess M5(0,0,0,"");
		M5.receive_mes(kdc_socket);
		check1 = M5.getSrc_id();
		check2 = M5.getDest_id();

		cout << "[B]: ricevuto M5" << endl;
		if (check1 != B_ID || check2 != A){
			cout << "[B]: ricevuto M5 con src_id " << check1 << " e dest_"
					"id " << check2 << endl;
			return -1;
		}
		//recupero del ciphertext e della chiave simmetrica da file (M5)
		cipher = M5.getCipher();

		kfile.open(SYM_KEY_FILE, ios::in | ios::binary);
		if (!kfile.is_open())
			user_err ("Sym key file not found");
		getline(kfile, sym_key);
		kfile.close();

		//decifratura del ciphertext (M5)
		Sym_Encryption S5;
		S5.sym_decrypt((const unsigned char*)sym_key.data(), cipher, &check1,
				&check2, &check3, A_asym_key);
		S5.~Sym_Encryption();
		if (check1 != B_ID || check2 != A || check3 != Nb){
			cout << "[B]: ciphertext di M5 con src_id " << check1 << " dest_"
					"id " << check2 << " nonce " << check3 << endl;
			return -1;
		}

		//considero la chiave pubblica di A valida, scrivo il file .pem
		as_k_file.open(A_PUB_KEY_FILE, ios::out | ios::binary);
		if (!as_k_file.is_open()) sys_err ("Unable to create asym key file");
		as_k_file.write(A_asym_key.data(), A_asym_key.length());
		
		printf("***FINE RECUPERO CHIAVE PUBBLICA DI A***\n");
	}
	as_k_file.close();
	close(kdc_socket);

	//Ricezione di M6 e controlli sugli ID
	Mess M6(0,0,0,"");
	M6.receive_mes(curr_sd);

	check1 = M6.getSrc_id();
	check2 = M6.getDest_id();
	as_cipher = M6.getCipher();

	cout << "[B]: ricevuto M6" << endl;

	if (check1 != A || check2 != B_ID){
		cout << "[B]: ricevuto M6 con src_id " << check1 << " e dest_id " <<
				check2 << endl;
		return -1;
	}

	//Decifratura del crittogramma contenuto in M6 e controllo sugli ID
	As_enc ae_M6("", PRIV_KEY_FILE);
	ae_M6.asym_decr(as_cipher);

	ae_M6.extract_integers(&check1, &check2, &as_a_nonce);

	if (check1 != A || check2 != B_ID){
		cout << "[B]: ciphertext di M6 con src_id " << check1 << " dest_"
				"id " << check2 << endl;
		return -1;
	}

	//Creazione del crittogramma da inviare in M7
	as_b_nonce = rand() % 1000 + 1;
	As_enc ae_M7(A_PUB_KEY_FILE, "");
	ae_M7.asym_encr(B_ID, A, as_a_nonce, as_b_nonce);

	//Creazione ed invio del messaggio M7
	Mess M7(B_ID, A, 0, ae_M7.getCipher());
	M7.send_mes(curr_sd);

	cout << "[B]: inviato M7" << endl;

	//Ricezione di M8 e controlli
	Mess M8(0,0,0,"");
	M8.receive_mes(curr_sd);
	check1 = M8.getSrc_id();
	check2 = M8.getDest_id();

	cout << "[B]: ricevuto M8" << endl;
	if (check1 != A || check2 != B_ID){
		cout << "[B]: ricevuto M8 con src_id " << check1 << " e dest_id " <<
				check2 << endl;
		return -1;
	}

	//decifratura del cipher di M8
	as_cipher = M8.getCipher();

	As_enc ae_M8("", PRIV_KEY_FILE);
	ae_M8.asym_decr(as_cipher);
	ae_M8.extract_integers(&check1, &check2, &check3, &check4);

	if (check1 != A || check2 !=B_ID ||
			check3 != as_b_nonce || check4 != as_a_nonce){
		cout << "[B]: ciphertext di M8 con src_id " << check1;
		cout << " dest_id " << check2 << " Yb " << check3;
		cout << " Ya " << check4 << endl;
		return -1;
	}

	cout << "Ya: " << as_a_nonce << " Yb: " << as_b_nonce << endl;



	//calcolo della chiave come hash dei nonce Ya e Yb

	int hash_len;
	hsh(as_a_nonce, as_b_nonce, "sha1", &shared_key, &hash_len);
//	for (int i = 0; i < hash_len; i++){
//		printbyte(shared_key[i]);
//	}
//	printf("\n");

	cout << "Protocollo completato, chiave di sessione stabilita" << endl;

	//ricezione messaggio cifrato con la chiave di sessione
	Mess M9(0,0,0,"");
	M9.receive_mes(curr_sd);
	int msg_len = M9.getCipher().size();
	Sym_Encryption test_mess;
	string plain = test_mess.generic_decrypt(shared_key,
			(unsigned char *) M9.getCipher().data(), msg_len);
	test_mess.~Sym_Encryption();
	cout << "Messaggio ricevuto:" << endl << plain << endl;

	close(curr_sd);
	close(rec_socket);
	return 0;
}
