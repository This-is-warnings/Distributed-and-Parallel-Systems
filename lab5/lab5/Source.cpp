#include "mpi.h"
#include<windows.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <typeinfo>
#include <fstream>
#include <string>

using namespace std;
vector<int> vector_of_operations;
double result_thread = 0;
double a, b, c, d, hx, hy;
int n, m, k;
int funcNumber;
vector<float> v;

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

double calculation(int i)
{

	int st = i;
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
				return answer;
				//return 0;
			}
			el++;
		}
	}
	return answer;

	/*double sum = 0;
	double x = a + hx * i;
	for (double y = c; y <= d; y += hy)
	{
		sum+= hx * hy * func(x + hx / 2, y + hy / 2, funcNumber);
	}
	cout << "process " << i << " return " << sum << endl;
	//cout << "Закончило " << i;
	return sum;*/
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
	/*cout << "v: ";
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;*/
	return v;
}

int main(int *argc,char**argv)
{
	setlocale(LC_ALL, "rus");
	/*cout << "Enter a, b, c, d: \n";
	
	cin >> a >> b >> c >> d;*/
	int numtasks, rank;
	double ss = 0;
	int count = (b - a) / hx;
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	ifstream in("arguments.txt"); 
	string line;
	getline(in, line);
	a = stoi(line);

	getline(in, line);
	b = stoi(line);

	getline(in, line);
	c = stoi(line);

	getline(in, line);
	d = stoi(line);
	
	getline(in, line);
	n = stoi(line);

	getline(in, line);
	m = stoi(line);

	getline(in, line);
	funcNumber = stoi(line);

	hx = (b - a) / n;
	hy = (d - c) / m;
	int oper = 0;
	for (double x = a; x <= b; x += hx) {
		for (double y = c; y <= d; y += hy) {
			oper++;
		}
	}
	vector_of_operations = razbiv(oper, numtasks);
	
	//cout << numtasks << endl;
	if (rank == 0)
	{ 
		double m = calculation(0);
		for (int i = 0; i < numtasks - 1; i++)
		{
			double b;
			cout << "ждет " << endl;
			MPI_Recv(&b, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "0 получил " << b << endl;
			m += b;
		}
		cout << endl << "Result: " << m;
	}
	else
	{
		double m = calculation(rank);
		cout << rank << " " << m << endl;
		MPI_Send(&m, 1, MPI_DOUBLE, 0, rank+10, MPI_COMM_WORLD);
		cout << "отправлено " << rank << endl;;
	}

	MPI_Finalize();
	//system("pause");
}