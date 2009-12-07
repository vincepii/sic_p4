#include "asym_enc.h"


As_enc::As_enc()
{
	rsa_obj = RSA_new();
	if (rsa_obj == NULL) sys_err("Error creating RSA object");
	RSA
}
