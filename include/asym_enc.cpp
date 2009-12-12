#include "asym_enc.h"


As_enc::As_enc(string pubfile, string privfile)
{
	cout << "sono quiiiiii" << endl;
	this->pubkey_file = pubfile;
	this->privkey_file = privfile;
	//this->cipher = "";
	//this->plain = "";
	cout << "fine costruttore" << endl;
}

int As_enc::asym_encr(int src_id, int dst_id, int nonce1)
{
	RSA* pubkey;
	FILE* pubk_file;
	int check = 0;
	int dim;
	unsigned char* from;
	unsigned char* dest;
	int ptr = 0;



	pubk_file = fopen(this->pubkey_file.c_str(), "rb");
	if (pubk_file == NULL) sys_err("Cannot find public key file");
	pubkey = PEM_read_RSA_PUBKEY(pubk_file, NULL, NULL, NULL);
	if (pubkey == NULL) sys_err("Cannot create RSA object with public key");
	fclose(pubk_file);

	dim = 3 * sizeof(int);

	//preparazione del buffer da cifrare
	from = new unsigned char[dim];
	bzero(from, dim);

	memcpy(&from[ptr], &src_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &dst_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &nonce1, sizeof(int));

	dest = new unsigned char[RSA_size(pubkey)];
	bzero(dest, RSA_size(pubkey));

	check = RSA_public_encrypt(dim, from, dest, pubkey, RSA_PKCS1_OAEP_PADDING);

	if (check < dim || check == -1){
		cout << ERR_error_string(ERR_get_error(), NULL) << endl;
		sys_err ("Public key encryption error");
	}

	this->cipher.assign((const char *)dest);

	delete[] from;
	delete[] dest;
	RSA_free(pubkey);

	return 1;
}

int As_enc::asym_encr(int src_id, int dst_id, int nonce1, int nonce2)
{
	RSA* pubkey;
	FILE* pubk_file;
	int check = 0;
	int dim;
	unsigned char* from;
	unsigned char* dest;
	int ptr = 0;

	pubk_file = fopen(this->pubkey_file.c_str(), "rb");
	if (pubk_file == NULL) sys_err("Cannot open public key file");
	pubkey = PEM_read_RSA_PUBKEY(pubk_file, NULL, NULL, NULL);
	if (pubkey == NULL) sys_err("Cannot create RSA object with public key");
	fclose(pubk_file);

	dim = 4 * sizeof(int);

	from = new unsigned char[dim];
	bzero(from, dim);

	memcpy(&from[ptr], &src_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &dst_id, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &nonce1, sizeof(int));
	ptr += sizeof(int);
	memcpy(&from[ptr], &nonce2, sizeof(int));

	dest = new unsigned char[RSA_size(pubkey)];
	bzero(dest, RSA_size(pubkey));

	check = RSA_public_encrypt(dim, from, dest, pubkey, RSA_PKCS1_OAEP_PADDING);

	if (check < dim || check == -1) {
		cout << ERR_error_string(ERR_get_error(), NULL) << endl;
		sys_err ("Public key encryption error");
	}

	this->cipher.assign((const char *)dest);

	delete[] from;
	delete[] dest;
	RSA_free(pubkey);

	return 1;
}

int As_enc::asym_decr(string ctxt)
{
	RSA* privkey;
	FILE* prik_file;
	int dim = 0;
	int check;
	unsigned char* dest;
	unsigned char* from;

	prik_file = fopen(this->privkey_file.c_str(), "rb");
	if (prik_file == NULL) sys_err("Cannot open private key file");
	privkey = PEM_read_RSAPrivateKey(prik_file, NULL, NULL, NULL);
	if (privkey == 0) sys_err("Cannot create RSA object with private key");
	fclose(prik_file);

	from = (unsigned char *)ctxt.data();

	dest = new unsigned char[RSA_size(privkey)];
	bzero(dest, RSA_size(privkey));
	//this->plain = new unsigned char[16];
	//bzero(this->plain, 16);
	//dim = cipher_length;
	dim = ctxt.length();
	check = RSA_private_decrypt(dim, from, dest, privkey, RSA_PKCS1_OAEP_PADDING);

	if (check == -1) {
		cout << ERR_error_string(ERR_get_error(), NULL) << endl;
		sys_err ("Private key decryption error");
	}

	this->plain.assign((const char *)dest);
	delete[] dest;
	RSA_free(privkey);

	return 1;
}

void As_enc::extract_integers(int* a, int* b, int* c)
{
	unsigned char* p = (unsigned char *)this->plain.data();
	memcpy(a, &p[0], sizeof(int));
	memcpy(b, &p[1 * sizeof(int)], sizeof(int));
	memcpy(c, &p[2 * sizeof(int)], sizeof(int));
	return;
}

void As_enc::extract_integers(int* a, int* b, int* c, int* d)
{
	unsigned char* p = (unsigned char *)this->plain.data();
	memcpy(a, &p[0], sizeof(int));
	memcpy(b, &p[1 * sizeof(int)], sizeof(int));
	memcpy(c, &p[2 * sizeof(int)], sizeof(int));
	memcpy(d, &p[3 * sizeof(int)], sizeof(int));
	return;
}

void As_enc::print_plain(int number_of_int_carried)
{
	int out = 0;
	int ptr = 0;
	unsigned char* p = (unsigned char *)this->plain.data();
	for (int i = 0; i < number_of_int_carried; i++){
		ptr = i * sizeof(int);
		memcpy(&out, &p[ptr], sizeof(int));
		cout << "Intero " << i << ": " << out << endl;
	}

}

string As_enc::getCipher()
{
	return this->cipher;
}

string As_enc::getPlain()
{
	return this->plain;
}

/*As_enc::~As_enc()
{
	//if (cipher != NULL)
		delete[] cipher;
	//if (plain != NULL)
		delete[] plain;
}
*/
