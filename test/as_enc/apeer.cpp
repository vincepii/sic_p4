#include "../../include/utilities.h"
#include "../../include/comm_layer.h"

using namespace std;

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

	//Creazione ed invio di un messaggio
	const char* mp = "Messaggio di prova";
	Mess mes(25, 10, 50, (unsigned char*)mp, strlen(mp));
	mes.send_mes(sd);

	return 0;
}
