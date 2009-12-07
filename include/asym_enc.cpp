#include "asym_enc.h"


As_enc::As_enc()
{
	rsa_obj = RSA_new();
	if (rsa_obj == NULL) sys_err("Error creating RSA object");
}

string As_enc::asym_enc(int src_id, int dst_id, int nonce1, int nonce2)
{
	//FILE* pk_file;
	if (src_id == 1){
		//pk_file = fopen();
	}
	return 0;
}
