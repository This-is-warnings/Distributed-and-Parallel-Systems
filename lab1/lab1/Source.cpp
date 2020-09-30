#include<windows.h>
#include <iostream>
#include <vector>

using namespace std;
vector<int> vector_of_operations;
double result_thread = 0;
double a, b, c, d, hx,hy;
int n, m, k;
int funcNumber;

double func(double x, double y, int number)
{
	switch (number)
	{
	case 1:
	{
		return exp(x) * sin(y);
		break;
	}
	case 2:
	{
		return sin(x + 2 * y);
		break;
	}
	case 3:
	{
		return cos(x * exp(y));
		break;
	}
	case 4:
	{
		return exp(cos(x * x + y));
		break;
	}
	}
}


DWORD WINAPI ThreadFunc(LPVOID lpStartNumber) {
	//WaitForSingleObject(hMutex, INFINITE);
	unsigned st = *((unsigned*)lpStartNumber);

	int z1 = st * vector_of_operations[0] + vector_of_operations[st];
	int z2 = st * vector_of_operations[0];
	int el = 0;
	double answer = 0;
	for (double x = a; x <= b; x += hx) {
		for (double y = c; y <= d; y += hy) {
			if (el < z1 && el >= z2) {
				answer += hx * hy * func(x + hx / 2, y + hy / 2, funcNumber);
			}
			else if (el > z1) {
				result_thread += answer;
				return 0;
			}
			el++;
		}
	}
	result_thread += answer;
	return 0;

}

vector<int> razbiv(int operacii, int k)
{
	vector<int> v(k, 0);
	int l = 0;
	for (int i = 0; i < operacii; i++)
	{
		if (l == k) l = 0;
		v[l]++;
		l++;
	}
	cout << "v: ";
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;
	return v;
}



int main()
{
	cout << "Enter a, b, c, d: \n";
	
	cin >> a >> b >> c >> d;
	if (a > b)
	{
		double tmp = a;
		a = b;
		b = tmp;
	}
	if (c > d)
	{
		double tmp = c;
		c = d;
		d = tmp;
	}

	cout << "n, m, K: \n";

	
	cin >> n >> m >> k;

	
	cout << "Choose function: " << endl
		<< "1) e^x * sin(y)" << endl
		<< "2) sin(x + 2y)" << endl
		<< "3) cos(x * e^y)" << endl
		<< "4) e^(cos(x^2 + y))" << endl;
	cin >> funcNumber;

	hx = (b - a) / n;
	hy = (d - c) / m;

	cout << "One thread \n";
	double answer = 0;
	for (double x = a; x <= b; x += hx) {
		for (double y = c; y <= d; y += hy) {
			answer += hx * hy * func(x + hx / 2, y + hy / 2,funcNumber);
		}
	}
	cout << answer << endl;
	cout << "K thread \n";
	int oper = 0;
	for (double x = a; x <= b; x += hx) {
		for (double y = c; y <= d; y += hy) {
			oper++;
		}
	}
	vector_of_operations = razbiv(oper, k);
	HANDLE* hThreads = NULL;
	hThreads = new HANDLE[k];

	for (unsigned i = 0; i < k; i++) {
		unsigned* z = new unsigned(i);
		hThreads[i] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)z, 0, NULL);
	}

	WaitForMultipleObjects(
		k, //размер массива
		hThreads, //указатель на поток 
		TRUE, //Если третий аргумент равен TRUE, процедура ждет перехода в сигнальное состояние всех объектов, 
			  //иначе — любого из указанных объектов.
		INFINITE
	); //время

	for (unsigned i = 0; i < k; i++)
		CloseHandle(hThreads[i]);



	cout << result_thread << endl;
	system("pause");
	return 0;
}