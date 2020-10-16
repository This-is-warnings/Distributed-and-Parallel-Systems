#include <iostream>
#include<Windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <omp.h>
using namespace std;
int** array_of_n_threads;
vector<int> vector_of_operations;
int N;
int** arr1, ** arr2;


int main()
{
	int n;
	cout << "Enter n: ";
	cin >> n;
	N = n;
	// заполнение массивов
	arr1 = new int* [n];
	arr2 = new int* [n];
	for (int i = 0; i < n; i++) {
		arr1[i] = new int[n];
		arr2[i] = new int[n];
		for (int j = 0; j < n; j++) {
			arr1[i][j] = rand() % 30;
			arr2[i][j] = rand() % 30;
		}
	}
	cout << "First: " << endl;
	cout << setw(10);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//cout << arr[i][j] << setw(10);
		}
		//cout << endl;
	}

	cout << "Second: " << endl;
	cout << setw(10);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//cout << arr2[i][j] << setw(10);
		}
		//cout << endl;
	}

	// простое умножение
	clock_t start = clock();
	int l = 0;
	int** array_of_one_thread;
	array_of_one_thread = new int* [n];
	for (int i = 0; i < n; i++)
	{
		array_of_one_thread[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			array_of_one_thread[i][j] = 0;
			for (int k = 0; k < n; k++)
			{
				array_of_one_thread[i][j] += arr1[i][k] * arr2[k][j];
			}
		}

	cout << "Result: " << endl;
	cout << setw(15);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << array_of_one_thread[i][j] << setw(15);
		}
		cout << endl;
	}
	// Вывод в файл

	ofstream fout("one_thread.txt", ios::out);
	fout << setw(15);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fout << array_of_one_thread[i][j] << setw(15);
		}
		fout << endl;
	}
	// умножение потоками
	cout << "Thread: ";
	array_of_n_threads = new int* [n];
	for (int i = 0; i < n; i++)
	{
		array_of_n_threads[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			array_of_n_threads[i][j] = 0;
		}
	}
	int k;
	cin >> k;
	omp_set_num_threads(k);
	#pragma omp parallel // цикл будет выполняться в K потоков. Параметры цикла будут автоматически распределены между потоками.
	{
	#pragma omp for // прагма for помогает распределить рабочую нагрузку цикла for между несколькими потоками
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				for (int k = 0; k < n; k++)
					array_of_n_threads[i][j] += arr1[i][k] * arr2[k][j];
			}
		}
		
	}

	cout << "Result: " << endl;
	cout << setw(15);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << array_of_n_threads[i][j] << setw(15);
		}
		cout << endl;
	}

	ofstream fout1("n_thread.txt", ios::out);
	fout1 << setw(15);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fout1 << array_of_n_threads[i][j] << setw(15);
		}
		fout1 << endl;
	}

	cout << endl;
	system("pause");
	return 0;
}