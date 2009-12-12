#include "comm_layer.h"

Mess::Mess(int src_id, int dest_id, int nonce,
		string cipher)
{
	this->src_id = src_id;
	this->dest_id = dest_id;
	this->nonce = nonce;
	this->msg = cipher;

	//this->cipher_ll = cipher_ll;

	//this->vett(cipher_ll);
	//this->vett = vector<unsigned char>(cipher_ll);
	//this->vett.push_back(cipher);


//	if(cipher_ll > 0){
//		this->cipher = new unsigned char[cipher_ll];
//		strcpy((char *)this->cipher, (const char *)cipher);
//		//for (int i = 0; i < cipher_ll; i++)
//			//this->cipher[i] = cipher[i];
//		//cout << this->cipher << endl;
//	}
//	else{
//		this->cipher = NULL;
//	}
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

	//this->cipher = new unsigned char[cipher_ll];
	//ret = safe_read(socket, this->cipher, cipher_ll);

	//unsigned char* buf = (unsigned char *)malloc(cipher_ll);
	//ret = safe_read(socket, buf, cipher_ll);
	//this->cipher = buf;

	vector<char> vett(ll);
	ret = safe_read(socket, &vett[0], ll);
	if (ret == 0 && ll != 0) return ret;
	if (ret == -1) return ret;
	this->msg.assign(vett.begin(), vett.end());

	//cout << "Test: " << this->cipher << endl;

	return 1;
}

void Mess::print()
{
	cout << "SRC_ID: " << src_id << endl;
	cout << "DEST_ID: " << dest_id << endl;
	cout << "Nonce: " << nonce << endl;
	//cout << "Cipher Length: " << cipher_ll << endl;
	cout << "Message: " << msg << endl;
}

string Mess::getCipher()
{
	return msg;
}

//int Mess::getCipher_ll()
//{
//	return cipher_ll;
//}

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
	//cout << "Cipher Length: " << cipher_ll << endl;
	cout <<endl<< "Message: "<<endl;
	for (unsigned int i=0; i<msg.length(); i++)
		printbyte(msg.at(i));
	cout<<endl;
}
