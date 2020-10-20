#include<windows.h>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;
vector<int> vector_of_operations;
double result_thread = 0;
double a, b, c, d, hx, hy;
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
			answer += hx * hy * func(x + hx / 2, y + hy / 2, funcNumber);
		}
	}
	cout << answer << endl;
	cout << "K thread \n";
	
	omp_set_num_threads(k);
	int s = 0;
	hx = (b - a) / n;
	hy = (d - c) / m;

	double ss = 0;
	int count = (b - a) / hx;
#pragma omp parallel reduction(+:ss)
	{
#pragma omp for
		for (int i = 0; i <= count; i++) {
			double x = a + hx * i;
			for (double y = c; y <= d; y += hy) {
				ss += hx * hy * func(x + hx / 2, y + hy / 2, funcNumber);
			}
		}
		
	}
	cout << ss << endl;
	system("pause");
	return 0;
}