#include "comm_layer.h"

Mess::Mess(int src_id, int dest_id, int nonce,
		unsigned char* cipher, int cipher_ll)
{
	this->src_id = src_id;
	this->dest_id = dest_id;
	this->nonce = nonce;
	this->cipher_ll = cipher_ll;
	if(cipher_ll > 0){
		this->cipher = new unsigned char[cipher_ll];
		*(this->cipher) = *cipher;
	}

	else this->cipher = NULL;
}

void Mess::send_mes(int socket)
{
	write(socket, &this->src_id, sizeof(int));
	write(socket, &this->dest_id, sizeof(int));
	write(socket, &this->nonce, sizeof(int));
	write(socket, &this->cipher_ll, sizeof(int));
	write(socket, this->cipher, this->cipher_ll);
}

int Mess::receive_mes(int socket)
{
	int ret = 0;

	ret = safe_read(socket, &(this->src_id), sizeof(int));
	if (ret <= 0) return ret;
	ret = safe_read(socket, &(this->dest_id), sizeof(int));
	if (ret <= 0) return ret;
	ret = safe_read(socket, &(this->nonce), sizeof(int));
	if (ret <= 0) return ret;
	ret = safe_read(socket, &(this->cipher_ll), sizeof(int));
	if (ret <= 0) return ret;

	this->cipher = new unsigned char[cipher_ll];
	ret = safe_read(socket, this->cipher, cipher_ll);

	//unsigned char* buf = (unsigned char *)malloc(cipher_ll);
	//ret = safe_read(socket, buf, cipher_ll);
	//this->cipher = buf;

	//vector<unsigned char> vett(this->cipher_ll);
	//safe_read(socket, &vett[0], this->cipher_ll);
	//this->cipher = vett.data();
	cout << "Test: " << this->cipher << endl;

	return 1;
}

void Mess::print()
{
	cout << "SRC_ID: " << src_id << endl;
	cout << "DEST_ID: " << dest_id << endl;
	cout << "Nonce: " << nonce << endl;
	cout << "Cipher Length: " << cipher_ll << endl;
	cout << "Message: " << cipher << endl;
}