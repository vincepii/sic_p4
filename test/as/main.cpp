#include "../../include/asym_enc.h"
#include <string>

#define PUB "pubkey.pem"
#define PRI "privkey.pem"

int main ()
{
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	string cipher;

	srand ( time(NULL) );

	cout << "---- Prima esecuzione ----" << endl;

	a = rand() % 1000 + 1;
	b = rand() % 1000 + 1;
	c = rand() % 1000 + 1;
	d = rand() % 1000 + 1;
//	a = 10;
//	b = 20;
//	c = 30;
//	d = 40;
	cout << "Valori generati: " << endl;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	cout << "C: " << c << endl;
	cout << "D: " << d << endl << endl;

	As_enc as_obj1(PUB, PRI);
	as_obj1.asym_encr(a,b,c,d);
	cipher = as_obj1.getCipher();

	As_enc as_obj2("", PRI);
	as_obj2.asym_decr(cipher);
	as_obj2.extract_integers(&a, &b, &c, &d);
	cout << "Valori decifrati: " << endl;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	cout << "C: " << c << endl;
	cout << "D: " << d << endl << endl;

	cout << "---- Seconda esecuzione ----" << endl;

	a = rand() % 1000 + 1;
	b = rand() % 1000 + 1;
	c = rand() % 1000 + 1;
	d = rand() % 1000 + 1;
//	a = 10;
//	b = 20;
//	c = 30;
//	d = 40;
	cout << "Valori generati: " << endl;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	cout << "C: " << c << endl;
	cout << "D: " << d << endl << endl;

	As_enc as_obj3(PUB, "");
	as_obj3.asym_encr(a,b,c,d);
	cipher = as_obj3.getCipher();

	As_enc as_obj4("", PRI);
	as_obj4.asym_decr(cipher);
	as_obj4.extract_integers(&a, &b, &c, &d);
	cout << "Valori decifrati: " << endl;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	cout << "C: " << c << endl;
	cout << "D: " << d << endl << endl;

/*
	As_enc as_obj1(PUB, PRI);
	as_obj1.asym_encr(a,b,c,d);
	cipher = as_obj1.getCipher();

	//As_enc as_obj2("", PRI);
	as_obj1.asym_decr(cipher);
	as_obj1.extract_integers(&a, &b, &c, &d);
	cout << "Valori decifrati: " << endl;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	cout << "C: " << c << endl;
	cout << "D: " << d << endl << endl;

	cout << "---- Seconda esecuzione ----" << endl;

	a = rand() % 1000 + 1;
	b = rand() % 1000 + 1;
	c = rand() % 1000 + 1;
	d = rand() % 1000 + 1;
	cout << "Valori generati: " << endl;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	cout << "C: " << c << endl;
	cout << "D: " << d << endl << endl;

	//As_enc as_obj3(PUB, "");
	as_obj1.asym_encr(a,b,c,d);
	cipher = as_obj1.getCipher();

	//As_enc as_obj4("", PRI);
	as_obj1.asym_decr(cipher);
	as_obj1.extract_integers(&a, &b, &c, &d);
	cout << "Valori decifrati: " << endl;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	cout << "C: " << c << endl;
	cout << "D: " << d << endl << endl;
*/
	return 0;
}
