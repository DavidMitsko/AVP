#include "Function.hpp"

double **** multMatrix(double **** firstMatrix, double **** secondMatrix)
{
	double ****result = (double****)calloc(sizeof(double****), liniesA);
	for (int i = 0; i < liniesA; i++) {
		*(result + i) = (double***)calloc(sizeof(double***), columnsB);
		for (int j = 0; j < columnsB; j++) {
			*(*(result + i) + j) = (double**)calloc(sizeof(double**), 8);
			for (int k = 0; k < 8; k++) {
				*(*(*(result + i) + j) + k) = (double*)calloc(sizeof(double), 8);
				for (int l = 0; l < 8; l++) {
					*(*(*(*(result + i) + j) + k) + l) = 0;
				}
			}
		}
	}

	auto start_clocks = __rdtsc();

	for (int i = 0; i < liniesA; i++) {
		for (int r = 0; r < columnsB; r++) {
			for (int m = 0; m < 8; m++) {
				for (int j = 0; j < columnsA; j++) {
					for (int n = 0; n < 8; n++) {
						//#pragma loop(no_vector)
						for (int k = 0; k < 8; k++) {
							result[i][r][m][k] += firstMatrix[i][j][m][n] * secondMatrix[j][r][n][k];
						}
					}
				}
			}
		}
	}

	auto end_clocks = __rdtsc();
	cout << "Clock count with auto vectorization:  " << end_clocks - start_clocks << endl;

	return result;
}

double **** multASMMatrix(double **** firstMatrix, double **** secondMatrix)
{
	double ****result = (double****)calloc(sizeof(double****), liniesA);
	for (int i = 0; i < liniesA; i++) {
		*(result + i) = (double***)calloc(sizeof(double***), columnsB);
		for (int j = 0; j < columnsB; j++) {
			*(*(result + i) + j) = (double**)calloc(sizeof(double**), 8);
			for (int k = 0; k < 8; k++) {
				*(*(*(result + i) + j) + k) = (double*)calloc(sizeof(double), 8);
				for (int l = 0; l < 8; l++) {
					result[i][j][k][l] = 0;
				}
			}
		}
	}

	double *res;
	double *t2;

	__m128d a_line, b0_line, b1_line, b2_line, b3_line, r0_line, r1_line, r2_line, r3_line;

	auto start_clocks = __rdtsc();
	for (int i = 0; i < liniesA; i++) {
		for (int r = 0; r < columnsB; r++) {
			for (int m = 0; m < 8; m++) {

				res = result[i][r][m];
				r0_line = _mm_load_pd((res + 0));
				r1_line = _mm_load_pd((res + 2));
				r2_line = _mm_load_pd((res + 4));
				r3_line = _mm_load_pd((res + 6));
				
				for (int j = 0; j < columnsA; j++) {
					for (int n = 0; n < 8; n++) {
						//double t1 = firstMatrix[i][j][m][n];
						//int e = bitset<32>(i * 1000 + j * 100 + m * 10 + n).to_ulong() * 8;

						////double *t2 = secondMatrix[j][r][n];
						////double *t2 = (double*)calloc(sizeof(double), 8);//= secondMatrix[j][r][n];
						////t2 = secondMatrix[j][r];
						//int q = j * 1000 + r * 100 + n * 10;
						//int pos = bitset<32>(q).to_ulong() * 8;
						//_asm pusha

						//_asm {
						//	xor esi, esi
						//	xor edi, edi
						//	xor ecx, ecx

						//	mov ecx, e
						//	mov esi, pos
						//	mov edi, posr

						//	movupd XMM0, [secondMatrix + esi]//secondMatrix[j][r][n][esi]//t2[esi]//
						//	add esi, 16
						//	movupd XMM1, [secondMatrix + esi]//t2[esi]//secondMatrix[j][r][n][esi]//
						//	add esi, 16
						//	movupd XMM2, [secondMatrix + esi]//t2[esi]//secondMatrix[j][r][n][esi]//
						//	add esi, 16
						//	movupd XMM3, [secondMatrix + esi]//t2[esi]//secondMatrix[j][r][n][esi]//

						//	movhps XMM4, [firstMatrix + ecx]//t1//firstMatrix[i][j][m][n]
						//	movlps XMM4, [firstMatrix + ecx]//t1//firstMatrix[i][j][m][n]

						//	mulpd XMM0, XMM4
						//	mulpd XMM1, XMM4
						//	mulpd XMM2, XMM4
						//	mulpd XMM3, XMM4

						//	//movupd XMM4, [result + edi]//secondMatrix[j][r][n][esi]//t2[esi]//
						//	//add edi, 16
						//	//movupd XMM5, [result + edi]//t2[esi]//secondMatrix[j][r][n][esi]//
						//	//add edi, 16
						//	//movupd XMM6, [result + edi]//t2[esi]//secondMatrix[j][r][n][esi]//
						//	//add edi, 16
						//	//movupd XMM7, [result + edi]//t2[esi]//secondMatrix[j][r][n][esi]//

						//	addpd XMM0, [result + edi]//result[i][r][m][edi]
						//	add edi, 16
						//	addpd XMM1, [result + edi]//result[i][r][m][edi]
						//	add edi, 16
						//	addpd XMM2, [result + edi]//result[i][r][m][edi]
						//	add edi, 16
						//	addpd XMM3, [result + edi]//result[i][r][m][edi]

						//	xor edi, edi
						//	mov edi, posr

						//	movupd xmmword ptr result[edi], XMM0//result[i][r][m][edi], XMM1
						//	add edi, 16
						//	movupd [result + edi], XMM1//result[i][r][m][edi], XMM1
						//	add edi, 16
						//	movupd [result + edi], XMM2//result[i][r][m][edi], XMM1
						//	add edi, 16
						//	movupd [result + edi], XMM3//result[i][r][m][edi], XMM1

						//}

						//_asm popa
						t2 = secondMatrix[j][r][n];

						b0_line = _mm_load_pd(t2 + 0);
						b1_line = _mm_load_pd(t2 + 2);
						b2_line = _mm_load_pd(t2 + 4);
						b3_line = _mm_load_pd(t2 + 6);


						a_line = _mm_set1_pd(firstMatrix[i][j][m][n]);

						r0_line = _mm_add_pd(_mm_mul_pd(a_line, b0_line), r0_line);
						r1_line = _mm_add_pd(_mm_mul_pd(a_line, b1_line), r1_line);
						r2_line = _mm_add_pd(_mm_mul_pd(a_line, b2_line), r2_line);
						r3_line = _mm_add_pd(_mm_mul_pd(a_line, b3_line), r3_line);

					}
				}
				_mm_store_pd(res + 0, r0_line);
				_mm_store_pd(res + 2, r1_line);
				_mm_store_pd(res + 4, r2_line);
				_mm_store_pd(res + 6, r3_line);
			}
		}
	}
	auto end_clocks = __rdtsc();
	cout << "Clock count with asm:  " << end_clocks - start_clocks << endl;
	
	return result;
}

bool equals(double **** firstMatrix, double **** secondMatrix)
{
	for (int i = 0; i < liniesA; i++) {
		for (int j = 0; j < columnsB; j++) {
			for (int n = 0; n < 8; n++) {
				for (int m = 0; m < 8; m++) {
					if (firstMatrix[i][j][n][m] != secondMatrix[i][j][n][m]) {
						return false;
					}
				}
			}
		}
	}
	return true;
}
