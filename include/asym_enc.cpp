#include "asym_enc.h"


As_enc::As_enc(string pubfile, string privfile)
{
	this->pubkey_file = pubfile;
	this->privkey_file = privfile;
	this->cipher = NULL;
	this->plain = NULL;
}

int As_enc::asym_encr(int src_id, int dst_id, int nonce1)
{
	RSA* pubkey;
	FILE* pubk_file;
	int check = 0;
	int dim;
	unsigned char* from;
	int ptr = 0;



	pubk_file = fopen(this->pubkey_file.c_str(), "rb");
	if (pubk_file == NULL) sys_err("Cannot find public key file");
	pubkey = PEM_read_RSA_PUBKEY(pubk_file, NULL, NULL, NULL);
	if (pubkey == NULL) sys_err("Cannot create RSA object with public key");
	fclose(pubk_file);

	//chiave pubblica pronta per la cifratura

	dim = 3 * sizeof(int);

	from = new unsigned char[dim];

	memcpy(&from[ptr], &src_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &dst_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &nonce1, sizeof(int));

	this->cipher = new unsigned char[RSA_size(pubkey)];

	check = RSA_public_encrypt(dim, from, this->cipher, pubkey, RSA_PKCS1_OAEP_PADDING);

	if (check < dim || check == -1) sys_err ("Public key encryption error");

	RSA_free(pubkey);
	delete[] from;

	return 1;
}

int As_enc::asym_encr(int src_id, int dst_id, int nonce1, int nonce2)
{
	RSA* pubkey;
	FILE* pubk_file;
	int check = 0;
	int dim;
	unsigned char* from;
	int ptr = 0;

	pubk_file = fopen(this->pubkey_file.c_str(), "rb");
	if (pubk_file == NULL) sys_err("Cannot open public key file");
	pubkey = PEM_read_RSA_PUBKEY(pubk_file, NULL, NULL, NULL);
	if (pubkey == NULL) sys_err("Cannot create RSA object with public key");
	fclose(pubk_file);

	//chiave pubblica pronta per la cifratura

	dim = 4 * sizeof(int);

	from = new unsigned char[dim];

	memcpy(&from[ptr], &src_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &dst_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &nonce1, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &nonce2, sizeof(int));

	this->cipher = new unsigned char[RSA_size(pubkey)];

	check = RSA_public_encrypt(dim, from, this->cipher, pubkey, RSA_PKCS1_OAEP_PADDING);

	if (check < dim || check == -1) sys_err ("Public key encryption error");

	RSA_free(pubkey);
	delete[] from;

	return 1;
}

int As_enc::asym_decr(unsigned char* from, int cipher_length)
{
	RSA* privkey;
	FILE* prik_file;
	int dim = 0;
	int check;

	prik_file = fopen(this->privkey_file.c_str(), "rb");
	if (prik_file == NULL) sys_err("Cannot open private key file");
	privkey = PEM_read_RSAPrivateKey(prik_file, NULL, NULL, NULL);
	if (privkey == 0) sys_err("Cannot create RSA object with private key");
	fclose(prik_file);

	this->plain = new unsigned char[RSA_size(privkey)];
	dim = cipher_length;
	check = RSA_private_decrypt(dim, from, this->plain, privkey, RSA_PKCS1_OAEP_PADDING);

	if (check == -1) sys_err ("Private key decryption error");

	RSA_free(privkey);

	return 1;
}

void As_enc::print_plain(int number_of_int_carried)
{
	int out = 0;
	int ptr = 0;

	for (int i = 0; i < number_of_int_carried; i++){
		ptr = i * sizeof(int);
		memcpy(&out, &this->plain[ptr], sizeof(int));
		cout << "Intero " << i << ": " << out << endl;
	}

}

unsigned char* As_enc::getCipher()
{
	return this->cipher;
}

unsigned char* As_enc::getPlain()
{
	return this->plain;
}

As_enc::~As_enc()
{
	//if (cipher != NULL)
		delete[] cipher;
	//if (plain != NULL)
		delete[] plain;
}

