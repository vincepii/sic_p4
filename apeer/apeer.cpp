#include "../include/utilities.h"
#include "../include/comm_layer.h"
#include <fstream>
#include "../include/asym_enc.h"
#include "../include/encryption.h"

using namespace std;

#define A_ID 1
#define B_ID 2
#define SYM_KEY_FILE "KAT"
#define PRIV_KEY_FILE "privkey.pem"
#define B_PUB_KEY_FILE "B_pubkey.pem"

int main (int argc, char* argv[])
{
	//-------------------------------------------------------------------------
	//Creazione socket per comunicazione con KDC e B (sempre da client)

	struct sockaddr_in kdc_addr, b_addr;
	int kdc_port;
	int kdc_sd;
	string kdc_ip;
	int b_port;
	int b_sd;
	string b_ip;

	if (argc < 4) user_err("usage: apeer <server_ip> <server_port> <bpeer ip> <bpeer port>");

	kdc_ip = argv[1];
	b_ip = argv[3];
	kdc_port = atoi(argv[2]);
	b_port = atoi(argv[4]);
	if (kdc_ip == "127.0.0.1") kdc_ip = "localhost";
	if (b_ip == "127.0.0.1") b_ip = "localhost";

	kdc_sd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&kdc_addr, sizeof(struct sockaddr_in));
	kdc_addr.sin_family = AF_INET;
	kdc_addr.sin_port = htons(kdc_port);
	inet_pton(AF_INET, kdc_ip.data(), &kdc_addr.sin_addr.s_addr);

	if (connect(kdc_sd, CAST_ADDR(&kdc_addr), sizeof(struct sockaddr_in)) < 0)
		sys_err("CL: connection error");

	b_sd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&b_addr, sizeof(struct sockaddr_in));
	b_addr.sin_family = AF_INET;
	b_addr.sin_port = htons(b_port);
	inet_pton(AF_INET, b_ip.data(), &b_addr.sin_addr.s_addr);

	if (connect(b_sd, CAST_ADDR(&b_addr), sizeof(struct sockaddr_in)) < 0)
		sys_err("CL: connection error");

	//-------------------------------------------------------------------------

	/*
	 * Operazioni del peer A:
	 * 1. Invio di un messaggio a B per iniziare la richiesta della chiave pubblica
	 * 2. Invio di un messaggio alla KDC
	 */

	int check1 = 0;
	int check2 = 0;
	int check3 = 0;
	int Na;
	string cipher;
	ifstream kfile;
	string sym_key;
	string B_asym_key;
	ofstream as_k_file;
	int as_a_nonce;
	int as_b_nonce;
	//int as_cipher_ll;
	string as_cipher;
	//unsigned char* as_plain;
	
	cout << "[A]: Running..." << endl;

	//Creazione e invio M1
	Mess M1(A_ID, B_ID, 0, "");
	M1.send_mes(b_sd);
	
	cout << "[A]: inviato M1" << endl;

	//Creazione e invio M2
	Na = rand() % 100 + 1;
	Mess M2(A_ID, B_ID, Na, "");
	M2.send_mes(kdc_sd);
	
	cout << "[A]: inviato M2" << endl;

	//Ricezione M3
	Mess M3(0,0,0,"");
	M3.receive_mes(kdc_sd);
	check1 = M3.getSrc_id();
	check2 = M3.getDest_id();

	cout << "[A]: ricevuto M3" << endl;
	if (check1 != A_ID || check2 != B_ID){
		cout << "[A]: ricevuto M3 con src_id " << check1 << " e dst_id ";
		cout << check2 << endl;
		return -1;
	}
	//cipher.assign((const char *)M3.getCipher());
	cipher = M3.getCipher();
	kfile.open(SYM_KEY_FILE, ios::in | ios::binary);
	if (!kfile.is_open())
		user_err ("Sym key file not found");
	getline(kfile, sym_key);
	kfile.close();

	Sym_Encryption S3;
	
	S3.sym_decrypt((const unsigned char *)sym_key.data(), cipher, &check1,
			&check2, &check3, B_asym_key);

	S3.~Sym_Encryption();

	if (check1 != A_ID || check2 != B_ID || check3 != Na){
		cout << "[A]: ciphertext di M3 con src_id " << check1 << " dest_"
				"id " << check2 << " nonce " << check3 << endl;
		return -1;
	}
//cout << B_asym_key << endl;
//	as_k_file.open(B_PUB_KEY_FILE, ios::out | ios::binary);
//	if (!as_k_file.is_open()) sys_err ("Unable to create asym key file");
//	as_k_file.write(B_asym_key.data(), B_asym_key.length());
//	as_k_file.close();

	//creazione ed invio M6
	as_a_nonce = rand() % 100 + 1;
	As_enc ae_M6(B_PUB_KEY_FILE, "");
	ae_M6.asym_encr(A_ID, B_ID, as_a_nonce);
	//as_cipher_ll = strlen((const char *)ae_M6.getCipher());
cout<<"Ya= " <<as_a_nonce<<endl;
	Mess M6(A_ID, B_ID, 0, ae_M6.getCipher());
	M6.send_mes(b_sd);
M6.print_hex();
cout<<ae_M6.getCipher().length()<<endl;
	cout << "[A]: inviato M6" << endl;

	//ricezione M7
	Mess M7(0,0,0,"");
	M7.receive_mes(b_sd);
	check1 = M7.getSrc_id();
	check2 = M7.getDest_id();

	cout << "[A]: ricevuto M7" << endl;
	if (check1 != B_ID || check2 != A_ID){
		cout << "[A]: ricevuto M7 con src_id " << check1 << " e dst_id ";
		cout << check2 << endl;
		return -1;
	}

	as_cipher = M7.getCipher();
	//as_cipher = (unsigned char *)M7.getCipher().c_str();
	//as_cipher_ll = M7.getCipher_ll();

	As_enc ae_M7("", PRIV_KEY_FILE);
	ae_M7.asym_decr(as_cipher);
	ae_M7.extract_integers(&check1, &check2, &check3, &as_b_nonce);

//	as_plain = ae_M7.getPlain();
//	check1 = as_plain[0];
//	check2 = as_plain[1 * sizeof(int)];
//	check3 = as_plain[2 * sizeof(int)];
//	as_b_nonce = as_plain[3 * sizeof(int)];

	if (check1 != B_ID || check2 != A_ID || check3 != as_a_nonce){
		cout << "[A]: ciphertext di M8 con src_id " << check1;
		cout << " dest_id " << check2 << " Ya " << check3 << endl;
		return -1;
	}

	//creazione e invio M8
	As_enc ae_M8(B_PUB_KEY_FILE, "");
	ae_M8.asym_encr(A_ID, B_ID, as_b_nonce, as_a_nonce);
	//as_cipher_ll = strlen((const char *)ae_M8.getCipher());

	Mess M8(A_ID, B_ID, 0, ae_M8.getCipher());
	M8.send_mes(b_sd);
	
	cout << "[A]: inviato M8" << endl;

	//creazione della chiave di sessione usando as_a_nonce e as_b_nonce

	cout << "Ya: " << as_a_nonce << " Yb: " << as_b_nonce << endl;

	return 0;
}
