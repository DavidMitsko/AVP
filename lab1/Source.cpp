#include <iostream>
#include <ctime>
#include <intrin.h>
#include "Function.hpp"

#define liniesA 200
#define columnsA 300
#define columnsB 400

using namespace std;

int main() {
	srand(time(0));

	double ****first_matrix_of_matrix = (double****)calloc(sizeof(double****), liniesA);
	double ****second_matrix_of_matrix = (double****)calloc(sizeof(double****), columnsA);

	for (int i = 0; i < liniesA; i++) {
		*(first_matrix_of_matrix + i) = (double***)calloc(sizeof(double***), columnsA);
		for (int j = 0; j < columnsA; j++) {
			*(*(first_matrix_of_matrix + i) + j) = (double**)calloc(sizeof(double**), 8);
			for (int k = 0; k < 8; k++) {
				*(*(*(first_matrix_of_matrix + i) + j) + k) = (double*)calloc(sizeof(double), 8);
				for (int l = 0; l < 8; l++) {
					*(*(*(*(first_matrix_of_matrix + i) + j) + k) + l) = rand() % 100 + 1;
				}
			}
		}
	}

	for (int i = 0; i < columnsA; i++) {
		*(second_matrix_of_matrix + i) = (double***)calloc(sizeof(double***), columnsB);
		for (int j = 0; j < columnsB; j++) {
			*(*(second_matrix_of_matrix + i) + j) = (double**)calloc(sizeof(double**), 8);
			for (int k = 0; k < 8; k++) {
				*(*(*(second_matrix_of_matrix + i) + j) + k) = (double*)calloc(sizeof(double), 8);
				for (int l = 0; l < 8; l++) {
					*(*(*(*(second_matrix_of_matrix + i) + j) + k) + l) = rand() % 100 + 1;
				}
			}
		}
	}

	double**** result1 = multMatrix(first_matrix_of_matrix, second_matrix_of_matrix);

	double**** result2 = multASMMatrix(first_matrix_of_matrix, second_matrix_of_matrix);

	bool r = equals(result1, result2);
	cout << r << endl;
	
	system("pause");
	return 0;
}