#include "mesgs.h"

Message::Message(int src_id, int dest_id, int nonce,
		unsigned char* cipher, int chipher_ll)
{
	this->src_id = src_id;
	this->dest_id = dest_id;
	this->nonce = nonce;
	this->cipher = cipher;
	this->cipher_ll = cipher_ll;
}

int Message::send_mes(int socket)
{
	write(socket, &this->src_id, sizeof(int));
	write(socket, &this->dest_id, sizeof(int));
	write(socket, &this->nonce, sizeof(int));
	write(socket, &this->cipher_ll, sizeof(int));
	write(socket, cipher, cipher_ll);

	return 1;
}

int Message::receive_mes(int socket)
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

	vector<unsigned char> r(cipher_ll);
	safe_read(socket, &r[0], cipher_ll);
	cipher = r.data();

	return 1;
}
