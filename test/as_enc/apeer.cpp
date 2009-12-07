#include "../../include/utilities.h"
#include "../../include/comm_layer.h"
#include "../../include/asym_enc.h"

using namespace std;

#define PUB_KEY_FILE "kdcpub.pem"

int main (int argc, char* argv[])
{
	//-------------------------------------------------------------------------

	struct sockaddr_in addr;
	int server_port;
	int sd;
	string server_ip;

	if (argc < 2) user_err("usage: client <server_ip> <server_port>");

	server_ip = argv[1];
	server_port = atoi(argv[2]);
	if (server_ip == "127.0.0.1") server_ip = "localhost";

	sd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);
	inet_pton(AF_INET, server_ip.data(), &addr.sin_addr.s_addr);

	if (connect(sd, CAST_ADDR(&addr), sizeof(struct sockaddr_in)) < 0)
		sys_err("CL: connection error");

	//-------------------------------------------------------------------------

	int A = 10;
	int B = 15;
	int nA = 20;
	int ll = 0;

	//invio primo messaggio
	As_enc as(PUB_KEY_FILE, "niente");
	as.asym_encr(A, B, nA);
	ll = strlen((const char *)as.getCipher());

	Mess m(A, B, 0, as.getCipher(), ll);
	m.send_mes(sd);

	//invio secondo messaggio
	As_enc as2(PUB_KEY_FILE, "");
	as2.asym_encr(8, 4, 44, 14);
	ll = strlen((const char *)as2.getCipher());

	Mess m2(A, B, 0, as2.getCipher(), ll);
	m2.send_mes(sd);

	//invio terzo messaggio
	As_enc as3(PUB_KEY_FILE, "");
	as3.asym_encr(9, 30, 21, 86);
	ll = strlen((const char *)as3.getCipher());

	Mess m3(A, B, 0, as3.getCipher(), ll);
	m3.send_mes(sd);

	return 0;
}
