#include "../include/utilities.h"
#include "../include/comm_layer.h"
#include "../include/asym_enc.h"
#include "../include/encryption.h"

using namespace std;

#define B_ID 2

int main (int argc, char* argv[])
{
	//-------------------------------------------------------------------------
	//PREPARAZIONE SOCKET PER COLLEGAMENTO A KDC

	struct sockaddr_in addr;
	int server_port;
	int kdc_socket;
	string server_ip;

	if (argc < 3) user_err("usage: bpeer <server_ip> <server_port> <local port>");

	server_ip = argv[1];
	server_port = atoi(argv[2]);
	if (server_ip == "127.0.0.1") server_ip = "localhost";

	kdc_socket = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);
	inet_pton(AF_INET, server_ip.data(), &addr.sin_addr.s_addr);

	if (connect(kdc_socket, CAST_ADDR(&addr), sizeof(struct sockaddr_in)) < 0)
		sys_err("CL: connection error");

	//-------------------------------------------------------------------------
	//PREPARAZIONE SOCKET PER RICEVERE DA A

	int port;

	port = atoi(argv[3]);

	int rec_socket, curr_sd;
	struct sockaddr_in c_add;
	unsigned int addrlen;

	rec_socket = init_sd(port);
	addrlen = sizeof(c_add);

	curr_sd = accept(rec_socket, CAST_ADDR(&c_add), &addrlen);
	if(curr_sd < 0) sys_err("KDC: accept failed");

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
	unsigned char* cipher;

	Mess M1(0,0,0,0,0);
	M1.receive_mes(curr_sd);
	A = M1.getSrc_id();
	B = M1.getDest_id();

	if (B != B_ID){
		// A non vuole comunicare con me
		cout << "[B]: ricevuto M1 con dest_id " << B << endl;
		return 0;
	}

	Nb = rand();
	Mess M4(B, A, Nb);
	M4.send_mes(kdc_socket);

	Mess M5(0,0,0,0,0);
	M5.receive_mes(kdc_socket);
	check1 = M5.getSrc_id();
	check2 = M5.getDest_id();

	if (check1 != B || check2 != A){
		cout << "[B]: ricevuto M2 con src_id " << B << "e dest_id" << A << endl;
		return 0;
	}
	cipher = M5.getCipher();

	return 0;
}
