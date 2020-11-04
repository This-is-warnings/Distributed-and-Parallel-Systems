#include <omp.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <Windows.h>

using namespace std;

// ������� �������
double random(double min, double max)
{
	return (double)(rand()) / RAND_MAX * (max - min) + min;
}

int main(int argc, char*argv[])
{
	srand(time(NULL));
	int N, K;
	// ������ ����������
	double** arr;

	if (argc != 1)
	{
		cerr << "Cipher: " << 1 << endl;
		// ���� ���� �� �����
		//cerr << "file:arguments.txt" << endl;
		//cerr << "file:Result.txt" << endl;
		// ���� ���� � ���������
		cerr << "stdin" << endl;
		cerr << "stdout" << endl;
		system("pause");
		return 0;
	}
	

	// ���������� �� ����� ������� K, N, ����� �������
	/*ifstream in("arguments.txt");
	in >> K;
	in >> N;
	cout << "N: " << N << endl<< "K: " << K << endl;

	arr = new double* [N];
	for (int i = 0; i < N; i++) {
		arr[i] = new double[N];
		for (int j = 0; j < N; j++) {
			arr[i][j] = 0;
		}
	}

	cout << setw(7);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			in >> arr[i][j];
			cout << arr[i][j] << setw(7);
		}
		cout << endl;
	}
	in.close();*/

	cout << "Enter N: ";
	cin >> N;
	cout << "Enter K: ";
	cin >> K;

	cout << setw(10);
	arr = new double* [N];
	for (int i = 0; i < N; i++) {
		arr[i] = new double[N];
		for (int j = 0; j < N; j++) {
			arr[i][j] = random(-100,100);
			cout << arr[i][j] << setw(10);
		}
		cout << endl;
	}
	omp_set_num_threads(K);

	// ���������� ����������� ������������ �����
	double res = (double)INT64_MAX;

#pragma omp parallel for //shared(res)
		for (int i = 1; i < N; i += 2) // �� ������ ��������
		{
			
			double sum = 0;
			for (int j = 0; j < N; j++)
			{
				sum += arr[j][i]; //����� �� ������� �������
			}
			if (sum < res) // ���� ����� �� ������� ������ ����������
				res = sum;
			Sleep(10000);
			//cout << sum << endl;
		}
	
	cout << "Result: " << res << endl;

	//����� � ����
	/*ofstream out;
	out.open("Result.txt"); // �������� ���� ��� ������
	if (out.is_open())
	{
		out << res << endl;
	}
	out.close();*/

	system("pause");
	return 0;
}