#include "utilities.h"
void user_err (const char *s)
{
        printf("%s\n", s);
        exit(-1);
}

void sys_err (const char *s)
{
        perror(s);
        exit(-1);
}

int init_sd(int port)
{
        struct sockaddr_in myaddr;
        int sd;

        bzero(&myaddr, sizeof(struct sockaddr_in));
        myaddr.sin_family = AF_INET;
        myaddr.sin_port = htons(port);
        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        sd = socket(AF_INET, SOCK_STREAM, 0);
        if (bind(sd, CAST_ADDR(&myaddr), sizeof(myaddr)) < 0)
                sys_err("bind failed!");

        if (listen(sd, BACKLOG) < 0)
                sys_err("listen failed!");

        return sd;
}

int safe_read(int sd, void *arg, int len)
{
        int b_rcvd = 0;
        int tmp;
        do {
                tmp = b_rcvd;
                b_rcvd += read(sd, (char *)(arg) + b_rcvd, len - b_rcvd);
                if (tmp > b_rcvd){ //read returned -1
                	return -1;
                	//potrei fare b_rcvd++ e continue
                }
                if(tmp == b_rcvd){       //read returned 0
                	return 0;
				}
        }
        while (b_rcvd < len);
        return b_rcvd;
}

void printbyte (char b)
{
	char c;
	c = b;
	c = c >> 4;
	c = c & 15; //00001111
	printf("%X", c);
	c = b;
	c = c & 15;
	printf("%X", c);
}

void select_random_key (unsigned char* k, int b)
{
	int i;
	RAND_bytes(k, b);	//scrive su k b byte pseudo-random
	for (i = 0; i < b-1; i++)
		printbyte(k[i]);
	printbyte(k[b-1]);
	cout << endl;
}

void hsh(int a, int b, string hf, unsigned char** sk, int* len){
	EVP_MD_CTX md_ctx;
	const EVP_MD* md;
	unsigned char* buf;
	int ptr = 0;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	buf = new unsigned char[2 * sizeof(int)];
	memcpy(&buf[ptr], (const void *)&a, sizeof(int));
	ptr += sizeof(int);
	memcpy(&buf[ptr], (const void *)&b, sizeof(int));

	OpenSSL_add_all_digests();
	md = EVP_get_digestbyname(hf.data());
	if(!md) {
		printf("Unknown message digest\n");
		exit(1);
	}
	EVP_MD_CTX_init(&md_ctx);
	EVP_DigestInit_ex(&md_ctx, md, NULL);
	EVP_DigestUpdate(&md_ctx, buf, 2 * sizeof(int));
	EVP_DigestFinal_ex(&md_ctx, md_value, &md_len);


	*sk = new unsigned char[md_len];
	memcpy(*sk, &md_value[0], md_len);
	*len = md_len;

	EVP_MD_CTX_cleanup(&md_ctx);

//	for (int i = 0; i < *len; i++){
//		printbyte(sk[i]);
//	}

//	printf("Digest is: ");
//	for(unsigned int i = 0; i < md_len; i++) printf("%02x", md_value[i]);
//	printf("\n");
	return;
}
