#pragma once
#include <iostream>
#include <intrin.h>
#include <bitset>
#include <Windows.h>

using namespace std;

#define liniesA 200
#define columnsA 300
#define columnsB 400

double**** multMatrix(double**** firstMatrix, double**** secondMatrix);
double**** multASMMatrix(double**** firstMatrix, double**** secondMatrix);
bool equals(double**** firstMatrix, double**** secondMatrix);