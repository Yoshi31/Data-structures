#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

void SplitingNumber(string num, int* sign, int* intPart, double* fracPart)
//функция разбивает входное число на целую и дробную часть, переводит их в int, а так-же определяет знак("1" если отрицательное и "0", если положительное)
{
	string integerPart, fractionalPart;
	if (num[0] == '-')
	{
		*sign = 1;
		num.erase(0, 1);
	}
	else
		*sign = 0;

	auto dot = num.find(".");

	if (dot != string::npos)
	{
		integerPart = num.substr(0, dot);
		fractionalPart = num.substr(dot + 1);
		cout << "Entire part = " << integerPart << endl << "Fractional part = " << fractionalPart << endl;
	}
	else
	{
		integerPart = num;
		fractionalPart = "0";
	}

	try {
		*intPart = stoi(integerPart);
	}
	catch (invalid_argument e) {
		cout << "Caught Invalid Argument Exception" << endl;
	}

	try {
		*fracPart = stoi(fractionalPart);
	}
	catch (invalid_argument e) {
		cout << "Caught Invalid Argument Exception" << endl;
	}
}

string BinaryIntegerPart(int intPart)
//функция которая переводит целое число в бинарное представление
{
	string binInt = "";
	while (intPart > 0) {
		if ((intPart % 2) == 1)
			binInt += "1";
		else
			binInt += "0";
		intPart = intPart / 2;
	}
	int lengthStr;

	lengthStr = binInt.size() - 1;
	string transit = "";
	while (lengthStr >= 0)
	{
		transit += binInt[lengthStr];
		lengthStr--;
	}

	return transit;
}

string BinaryFractionalPart(double fracPart)
//функция которая переводит дробную часть в бинарный вид
{
	string binFractional = "";
	while (fracPart > 1)
	{
		fracPart = fracPart / 10;
	}

	while (fracPart != 0)
	{
		fracPart = fracPart * 2;
		if (fracPart >= 1)
		{
			binFractional += "1";
			fracPart -= 1;
		}
		else
			binFractional += "0";
	}
	return binFractional;
}

int searchExp(string binaryInt, string binaryFrac, string* mantissa)
//функция, которая высчитывает значение экспоненты
{
	string module;
	int firstUnit,exp,lengthStr;
	module = binaryInt + binaryFrac;
	firstUnit = module.find("1",0)+1;
	lengthStr = binaryInt.size();
	exp = lengthStr - firstUnit;
	*mantissa=module.assign(module, firstUnit,52);
	return exp;
}

int binaryExp(int exp, string type)
//Функция высчитывает экспоненту, в зависимости от типа
{
	int expType,exhibitor;
	if (type == "float")
		expType = 127;
	else
		expType = 1023;
	exhibitor = exp + expType;
	return exhibitor;
}

string CheckExp(string binExp, string* mantissa, string type)
//функция проверяет, все ли биты экспоненты и мантиссы удовлетворяют типу. Если нет, то дополняет их нулями или удаляет лишние.
{
	int lengthExp, lengthMant;
	lengthExp = binExp.size();
	lengthMant = mantissa->size();
	if (type == "float")
	{
		if (lengthMant < 23)
			while (lengthMant < 23)
			{
				mantissa->insert(lengthMant, "0");
				lengthMant++;
			}
		if (lengthExp < 8)
			while (lengthExp < 8)
			{
				binExp.insert(0, "0");
				lengthExp++;
			}
		return binExp;
	}
	if (type == "double")
	{
		if (lengthMant < 52)
			while (lengthMant < 52)
			{
				mantissa->insert(lengthMant, "0");
				lengthMant++;
			}
		if (lengthExp < 11)
			while (lengthExp < 11)
			{
				binExp.insert(0, "0");
				lengthExp++;
			}
		return binExp;
	}
	return binExp;
}

typedef union {
	//union для float
	float f;
	struct
	{
		unsigned int mantissa : 23;
		unsigned int exponent : 8;
		unsigned int sign : 1;
	} raw;
} myfloat;

typedef union {
	//union для double
	double d;
	struct
	{
		unsigned long long int mantissa : 52;
		unsigned long long int exponent : 11;
		unsigned long long int sign : 1;
	} raw;
} mydouble;

void printBinary(unsigned long long int n, int bits)
{
	//побитовый перевод числа из 10 сс в 2 сс
	int k;
	for (k = bits - 1; k >= 0; k--) {

		if ((n >> k) & 1)
			cout<<"1";
		else
			cout<<"0";
	}
}

void printIEEE(myfloat var)
{
	//вывод юнион float
	cout << "[";
	printBinary(var.raw.sign,1);
	cout << "] [";
	printBinary(var.raw.exponent, 8);
	cout << "] [";
	printBinary(var.raw.mantissa, 23);
	cout << "]" << endl;
}

void printIEEE(mydouble var)
{
	//перегрузка функции для вывода
	cout << "[";
	printBinary(var.raw.sign, 1);
	cout << "] [";
	printBinary(var.raw.exponent, 11);
	cout << "] [";
	printBinary(var.raw.mantissa, 52);
	cout << "]" << endl;
}


int main(int argc, char **argv)
{
	if (argc == 1) { // если в аргументах только имя программы
		cout << "no arguments!" << endl; // выводим, что нет аргументов
	}
	else {
		// иначе выводим все аргументы, которые переданы
		for (int i = 1; i < argc; i++) {
			cout << "argv[" << i << "] - " << argv[i] << endl;
		}
	}

	string bfp, bip, binaryExhibitor, mantissa;
	string type, way, number;
	int sign, intPart,exp,exhibitor;
	double fracPart;
	number = argv[1];
	type = argv[2];
	way = argv[3];

	if (way == "classic")
	{
		//Если классический способ
		SplitingNumber(number, &sign, &intPart, &fracPart);
		bip = BinaryIntegerPart(intPart);
		bfp = BinaryFractionalPart(fracPart);
		exp = searchExp(bip, bfp, &mantissa);
		exhibitor = binaryExp(exp, type);
		binaryExhibitor = BinaryIntegerPart(exhibitor);
		binaryExhibitor = CheckExp(binaryExhibitor, &mantissa, type);
		cout << "[" << sign << "] [" << binaryExhibitor << "] [" << mantissa << "]" << endl;
	}
	if (way == "union")
	{
		//Если способ юнион(потом надо свич воткнуть)
		if (type == "float")
		{
			myfloat num;
			num.f = stof(number);
			printIEEE(num);
		}

		if (type == "double")
		{
			mydouble num;
			num.d = stod(number);
			printIEEE(num);
		}
	}
	
}

