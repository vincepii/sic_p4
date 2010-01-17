#include "comm_layer.h"

Mess::Mess(int src_id, int dest_id, int nonce, string iv, string cipher)
{
	this->src_id = src_id;
	this->dest_id = dest_id;
	this->nonce = nonce;
	this->iv = iv;
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

	ll = this->iv.length();
	ret = write(socket, &ll, sizeof(int));
	if (ret == -1)
		sys_err("Write error");

	ret = write(socket, (this->iv.data()), ll);
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
	vector<char> vett1(ll);
	ret = safe_read(socket, &vett1[0], ll);
	if (ret == 0 && ll != 0) return ret;
	if (ret == -1) return ret;
	this->iv.assign(vett1.begin(), vett1.end());


	ret = safe_read(socket, &ll, sizeof(int));
	if (ret <= 0) return ret;
	vector<char> vett2(ll);
	ret = safe_read(socket, &vett2[0], ll);
	if (ret == 0 && ll != 0) return ret;
	if (ret == -1) return ret;
	this->msg.assign(vett2.begin(), vett2.end());

	return 1;
}

void Mess::print()
{
	cout << "SRC_ID: " << src_id << endl;
	cout << "DEST_ID: " << dest_id << endl;
	cout << "Nonce: " << nonce << endl;
	cout << "IV: " << iv << endl;
	cout << "Message: " << msg << endl;
}

string Mess::getCipher()
{
	return msg;
}

string Mess::getIv()
{
	return iv;
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
	cout << "IV: ";
	for (unsigned int i=0; i<iv.length(); i++)
		printbyte(iv.at(i));
	cout<<endl;
	cout << "Message: ";
	for (unsigned int i=0; i<msg.length(); i++)
		printbyte(msg.at(i));
	cout<<endl;
}
