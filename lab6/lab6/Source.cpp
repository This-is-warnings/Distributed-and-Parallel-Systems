#include "mpi.h"
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <time.h>
#include <string>

using namespace std;
int** massiv;
int** result;
vector<int> vector_of_operations;
int N;
int** arr1, ** arr2;


void calculation(int i)
{
	int st = i;
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
	el = i1 * N * N;
	for (int i = i1; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				if (el < z1 && el >= z2) {

					massiv[i][j] += arr1[i][k] * arr2[k][j];
					/*cout << "massiv[" << i << "][" << j << "]= " << massiv[i][j] << endl
						<< "arr1[" << i << "][" << k << "]= " << arr1[i][k] << endl
						<< "arr2[" << k << "][" << j << "]= " << arr2[k][j] << endl;*/
				}
				else if (el > z1) {
					return;
				}
				el++;
			}
		}
	}
	return;
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


int** array_to_matrix(int* arr)
{
	int** res = new int* [N];

	for (int i = 0, f = 0; i < N; i++)
	{
		res[i] = new int[N];
		for (int j = 0; j < N; j++, f++)
		{
			res[i][j] = arr[f];
		}
	}
	delete[] arr;
	return res;
}

int* matrix_to_array(int** arr)
{
	int* res = new int[N * N];
	for (int i = 0, f = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++, f++)
		{
			res[f] = arr[i][j];
		}
	}
	delete[] arr;
	return res;
}

void matrix_sum(int** arr1, int** arr2)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			arr1[i][j] += arr2[i][j];
		}
	}
	//return arr1;
}
int main(int* argc, char** argv)
{
	setlocale(LC_ALL, "rus");

	int numtasks, rank;
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);


	ifstream in("arguments.txt");
	in >> N;
	//cout << N;
	arr1 = new int* [N];
	arr2 = new int* [N];
	massiv = new int* [N];
	result = new int* [N];
	for (int i = 0; i < N; i++) {
		arr1[i] = new int[N];
		arr2[i] = new int[N];
		massiv[i] = new int[N];
		result[i] = new int[N];
		for (int j = 0; j < N; j++) {
			arr1[i][j] = 0;
			arr2[i][j] = 0;
			massiv[i][j] = 0;
			result[i][j] = 0;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			in >> arr1[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			in >> arr2[i][j];
		}
	}
	
	vector_of_operations = razbiv(N * N * N, numtasks);
	if (rank == 0)
	{
		
		calculation(0);
		result = massiv;
		for (int i = 0; i < numtasks - 1; i++) {
			int* arrayka = new int[N * N];
			MPI_Recv(&arrayka[0], N * N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			matrix_sum(result, array_to_matrix(arrayka));
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << result[i][j] << setw(7);
			}
			cout << endl;
		}
	}
	else
	{
		calculation(rank);
		int* arrayka = matrix_to_array(massiv);
		MPI_Send(&arrayka[0], N * N, MPI_INT, 0, rank + 10, MPI_COMM_WORLD);
	}


	MPI_Finalize();
	//system("pause");
}