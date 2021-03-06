#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

int ** array_of_n_threads;
vector<int> vector_of_operations;
int N;
int** arr1, **arr2;

//HANDLE hMutex;
DWORD WINAPI SummWorker(LPVOID lpStartNumber) {
	//WaitForSingleObject(hMutex, INFINITE);
	unsigned st = *((unsigned*)lpStartNumber);
	int el = 0;
	int z1 = st * vector_of_operations[0] + vector_of_operations[st];
	int z2 = st * vector_of_operations[0];
	int propysk_oper = 0;
	int z = 0;
	while (z != st)
	{
		propysk_oper += vector_of_operations[z];
		z++;
	}
	int i1;
	i1 = propysk_oper / (N * N);
	el = i1 * N*N;
	//cout << "pr " << propysk_oper << ", i1 " << i1 << ", j1 " << j1 << ", el: "<<el <<endl;
//	k1 = propysk_oper- i1*N*N - j1*N;j1 = (propysk_oper - i1*N*N) / N;

	//j1 = N/propysk_oper;
	//i1 = j1 * N;
	//el = i1*N*N + j1*N; !!!!!!!!!!!!!!!!!!!
	
	//cout << "propysk op: " << propysk_oper
	//	<< " el: " << el <<
	//	" i1: " << i1 << endl
	//	<< "z1: " << z1 << " z2: " << z2 << endl;
	for (int i = i1; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				if (el < z1 && el >= z2) {
				
					array_of_n_threads[i][j] += arr1[i][k] * arr2[k][j];
				}
				else if (el > z1) {
					//ReleaseMutex(hMutex); //освобождение
					return 0;
				}
				el++;
			}
		}
	}

	//ReleaseMutex(hMutex); //освобождение
	return 0;
}



vector<int> razbiv(int operacii, int k)
{
	vector<int> v(k,0);
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
	int n;
	cout << "Enter n: ";
	cin >> n;
	N = n;
	// заполнение массивов
	arr1 = new int*[n];
	arr2 = new int*[n];
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
	array_of_one_thread = new int*[n];
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
	}
	clock_t end = clock();
	//cout << "time: " << end - start << endl;
	
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
	int k;
	cin >> k;
	
	array_of_n_threads = new int *[n];
	HANDLE* hThreads = NULL;
	hThreads = new HANDLE[k];
	//vector<int> v;
	vector_of_operations = razbiv(n*n*n, k);
	//вычисление

	clock_t start1 = clock();
	array_of_n_threads = new int*[n];
	for (int i = 0; i < n; i++)
	{
		array_of_n_threads[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			array_of_n_threads[i][j] = 0;
		}
	}
/*	hMutex = CreateMutex(NULL, //security atributes
		FALSE, // флаг начального владельца
		NULL //mutex name
	);*/
	for (unsigned i = 0; i < k; i++) {
		unsigned* z = new unsigned(i);
		hThreads[i] = CreateThread(NULL, 0, SummWorker, (LPVOID)z, 0, NULL);
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

	//CloseHandle(hMutex);
	clock_t end1 = clock();
	//cout << "time: " << end1 - start1 << endl;
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