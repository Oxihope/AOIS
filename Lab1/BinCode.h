#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct Bin {
    int sostoyanie = 0;
    int znak = 0;
    vector<int> int_part;
    vector<int> double_part;
};

Bin Dec_To_Pryam(int dec);

Bin Double_To_Plav(double dec);

double Pryam_To_Dec(Bin bin);

Bin Pryam_To_Obratn(Bin bin);

Bin Pryam_To_Dopoln(Bin bin);

Bin Dopoln_To_Pryam(Bin bin);

Bin Bin_Sum(Bin bin1, Bin bin2);

Bin Plav_To_Pryam(Bin bin);

Bin Delete_Zero(Bin bin);

Bin Plav_Bin_Sum(Bin bin1, Bin bin2);

Bin Bin_Multiply(Bin bin1, Bin bin2);

Bin Devide_Bin(Bin bin1, Bin bin2);

void Cout_Bin(Bin bin);