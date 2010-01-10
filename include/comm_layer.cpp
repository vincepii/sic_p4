#include "comm_layer.h"

Mess::Mess(int src_id, int dest_id, int nonce, string cipher)
{
	this->src_id = src_id;
	this->dest_id = dest_id;
	this->nonce = nonce;
	this->msg = cipher;
}

void Mess::send_mes(int socket)
{
	int ret = 0;
	int ll = 0;

	ret = write(socket, &this->src_id, sizeof(int));
	if (ret == -1)
		sys_err("Write error");

	ret = write(socket, &this->dest_id, sizeof(int));
	if (ret == -1)
		sys_err("Write error");

	ret = write(socket, &this->nonce, sizeof(int));
	if (ret == -1)
		sys_err("Write error");

	ll = this->msg.length();
	ret = write(socket, &ll, sizeof(int));
	if (ret == -1)
		sys_err("Write error");

	ret = write(socket, (this->msg.data()), ll);
	if (ret == -1)
		sys_err("Write error");
}

int Mess::receive_mes(int socket)
{
	int ret = 0;
	int ll = 0;

	ret = safe_read(socket, &(this->src_id), sizeof(int));
	if (ret <= 0) return ret;
	ret = safe_read(socket, &(this->dest_id), sizeof(int));
	if (ret <= 0) return ret;
	ret = safe_read(socket, &(this->nonce), sizeof(int));
	if (ret <= 0) return ret;
	ret = safe_read(socket, &ll, sizeof(int));
	if (ret <= 0) return ret;

	vector<char> vett(ll);
	ret = safe_read(socket, &vett[0], ll);
	if (ret == 0 && ll != 0) return ret;
	if (ret == -1) return ret;
	this->msg.assign(vett.begin(), vett.end());

	return 1;
}

void Mess::print()
{
	cout << "SRC_ID: " << src_id << endl;
	cout << "DEST_ID: " << dest_id << endl;
	cout << "Nonce: " << nonce << endl;
	cout << "Message: " << msg << endl;
}

string Mess::getCipher()
{
	return msg;
}

int Mess::getDest_id()
{
	return dest_id;
}

int Mess::getNonce()
{
	return nonce;
}

int Mess::getSrc_id()
{
	return src_id;
}

void Mess::print_hex()
{
	cout <<endl;
	cout << "SRC_ID: " << src_id << endl;
	cout << "DEST_ID: " << dest_id << endl;
	cout << "Nonce: " << nonce << endl;
	cout <<endl<< "Message: "<<endl;
	for (unsigned int i=0; i<msg.length(); i++)
		printbyte(msg.at(i));
	cout<<endl;
}
