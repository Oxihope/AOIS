#include "BinCode.h"

Bin Dec_To_Pryam(int dec) {
    Bin bin;
    bin.sostoyanie = 0;
    int natural;
    if (dec >= 0) {
        bin.znak = 0;
        natural = dec;
    }
    else {
        bin.znak = 1;
        natural = (-1) * dec;
    }
    while (natural > 0) {
        bin.int_part.insert(bin.int_part.begin(), natural % 2);
        natural = natural / 2;
    }
    return bin;
}

Bin Double_To_Plav(double dec) {
    Bin bin;
    bin.sostoyanie = 0;
    if (dec < 0) {
        bin.znak = 1;
        dec = (-1) * dec;
    }
    else {
        bin.znak = 0;
    }
    int exponent = 0;
    while (dec >= 2) {
        dec = dec / 2;
        exponent++;
    }
    while (dec < 1 && dec > 0) {
        dec = dec * 2;
        exponent--;
    }
    exponent += 127;
    Bin exp = Dec_To_Pryam(exponent);
    int size = exp.int_part.size();
    for (int i = 0, k = 0; i < 8; i++) {
        if (i < 8 - size) {
            bin.int_part.push_back(0);
        }
        else {
            bin.int_part.push_back(exp.int_part[k]);
            k++;
        }
    }
    dec = dec - 1;
    for (int i = 0; i < 23; ++i) {
        dec = dec * 2;
        if (dec >= 1) {
            bin.double_part.push_back(1);
            dec = dec - 1;
        }
        else {
            bin.double_part.push_back(0);
        }
    }
    return bin;
}

double Pryam_To_Dec(Bin bin) {
    int integer = 0;
    for (int i = bin.int_part.size(); i > 0; i--) {
        integer += bin.int_part[i - 1] * pow(2, bin.int_part.size() - i);
    }
    double real = 0;
    for (int i = 0; i < bin.double_part.size(); i++) {
        real += bin.double_part[i] * pow(2, -1 - i);
    }
    int znak;
    if (bin.znak == 0) {
        znak = 1;
    }
    else {
        znak = -1;
    }
    return znak * (integer + real);
}

Bin Pryam_To_Obratn(Bin bin) {
    if (bin.znak == 1) {
        for (int i = 0; i < bin.int_part.size(); i++) {
            if (bin.int_part[i] == 0) {
                bin.int_part[i] = 1;
            }
            else {
                bin.int_part[i] = 0;
            }
        }
    }
    bin.sostoyanie = 1;
    return bin;
}

Bin Pryam_To_Dopoln(Bin bin) {
    if (bin.znak == 1) {
        bin = Pryam_To_Obratn(bin);
        int i = bin.int_part.size() - 1;
        for (; bin.int_part[i] != 0; i--) {
            bin.int_part[i] = 0;
        }
        bin.int_part[i] = 1;
    }
    bin.sostoyanie = 2;
    return bin;
}

Bin Dopoln_To_Pryam(Bin bin) {
    if (bin.znak == 1) {
        for (int i = 0; i < bin.int_part.size(); i++) {
            if (bin.int_part[i] == 0) {
                bin.int_part[i] = 1;
            }
            else {
                bin.int_part[i] = 0;
            }
        }
        int i = bin.int_part.size() - 1;
        for (; bin.int_part[i] != 0; i--) {
            bin.int_part[i] = 0;
        }
        bin.int_part[i] = 1;
    }
    bin.sostoyanie = 0;
    return bin;
}

Bin Bin_Sum(Bin bin1, Bin bin2) {
    Bin bin;
    int size1 = bin1.int_part.size();
    int size2 = bin2.int_part.size();
    if (size1 > size2) {
        for (int i = 0; i <= size1 - size2; i++) {
            bin2.int_part.insert(bin2.int_part.begin(), bin2.znak);
        }
        bin1.int_part.insert(bin1.int_part.begin(), bin1.znak);
        size1++;
        size2 = size1;
    }
    else {
        if (size2 > size1) {
            for (int i = 0; i <= size2 - size1; i++) {
                bin1.int_part.insert(bin1.int_part.begin(), bin1.znak);
            }
            bin2.int_part.insert(bin2.int_part.begin(), bin2.znak);
            size2++;
            size1 = size2;
        }
        else {
            bin1.int_part.insert(bin1.int_part.begin(), bin1.znak);
            bin2.int_part.insert(bin2.int_part.begin(), bin2.znak);
            size1++;
            size2++;
        }
    }
    int sum = 0;
    for (int i = size2 - 1; i != -1; i--) {
        bin.int_part.insert(bin.int_part.begin(), (sum + bin1.int_part[i] + bin2.int_part[i]) % 2);
        sum = (sum + bin1.int_part[i] + bin2.int_part[i]) > 1 ? 1 : 0;
    }
    bin.sostoyanie = 2;
    bin.znak = (bin1.znak + bin2.znak + sum) % 2;
    return bin;
}

Bin Plav_To_Pryam(Bin bin) {
    Bin result;
    result.znak = bin.znak;
    result.sostoyanie = 0;
    Bin dop;
    dop.znak = 0;
    dop.int_part = bin.int_part;
    int exp = Pryam_To_Dec(dop) - 127;
    result.int_part.push_back(1);
    for (int i = 0; i < bin.double_part.size(); i++) {
        result.double_part.push_back(bin.double_part[i]);
    }
    if (exp > 0) {
        for (int i = 0; i < exp; i++) {
            int c = result.double_part[0];
            result.int_part.push_back(c);
            result.double_part.erase(result.double_part.begin());
        }
    }
    if (exp < 0) {
        result.double_part.insert(result.double_part.begin(), 1);
        result.int_part[0] = 0;
        exp++;
        if (exp < 0) {
            for (int i = exp; i < 0; i++) {
                result.double_part.insert(result.double_part.begin(), 0);
            }
        }
    }
    return result;
}

Bin Delete_Zero(Bin bin) {
    if (bin.sostoyanie == 0) {
        while (bin.int_part.size() != 0 && bin.int_part[0] == 0) {
            bin.int_part.erase(bin.int_part.begin());
        }
    }
    return bin;
}

Bin Plav_Bin_Sum(Bin bin1, Bin bin2) {
    Bin bin1_pryam = Plav_To_Pryam(bin1);
    Bin bin2_pryam = Plav_To_Pryam(bin2);
    int exp1 = bin1_pryam.double_part.size();
    int exp2 = bin2_pryam.double_part.size();
    int exp;
    if (exp1 > exp2) {
        for (int i = 0; i < exp1 - exp2; i++) {
            bin2_pryam.double_part.push_back(0);
        }
        exp = exp1;
    }
    if (exp2 > exp1) {
        for (int i = 0; i < exp2 - exp1; i++) {
            bin1_pryam.double_part.push_back(0);
        }
        exp = exp2;
    }
    if (exp1 == exp2) {
        exp = exp1;
    }
    while (bin1_pryam.double_part.size() != 0) {
        int c = bin1_pryam.double_part[0];
        bin1_pryam.int_part.push_back(c);
        bin1_pryam.double_part.erase(bin1_pryam.double_part.begin());
    }
    while (bin2_pryam.double_part.size() != 0) {
        int c = bin2_pryam.double_part[0];
        bin2_pryam.int_part.push_back(c);
        bin2_pryam.double_part.erase(bin2_pryam.double_part.begin());
    }
    Bin bin = Dopoln_To_Pryam(Bin_Sum(Pryam_To_Dopoln(bin1_pryam), Pryam_To_Dopoln(bin2_pryam)));
    bin = Delete_Zero(bin);
    for (int i = 0; i < exp; i++) {
        int c = bin.int_part[bin.int_part.size() - 1];
        bin.int_part.pop_back();
        bin.double_part.insert(bin.double_part.begin(), c);
    }
    double dec = Pryam_To_Dec(bin);
    bin = Double_To_Plav(dec);
    return bin;
}

Bin Bin_Multiply(Bin bin1, Bin bin2) {
    Bin bin;
    bin.znak = (bin1.znak + bin2.znak) % 2;
    bin.sostoyanie = 0;
    int size1 = bin1.int_part.size();
    int size2 = bin2.int_part.size();
    vector<int> adder(size1 + size2, 0);
    for (int j = size2 - 1; j != -1; j--) {
        for (int i = size1 - 1; i != -1; i--) {
            adder[i + j + 1] = adder[i + j + 1] + bin1.int_part[i] * bin2.int_part[j];
            if (adder[i + j + 1] >= 2) {
                adder[i + j] = adder[i + j] + adder[i + j + 1] / 2;
                adder[i + j + 1] = adder[i + j + 1] % 2;
            }
        }
    }
    if (adder.size() != 0) {
        while (adder.size() != 0 && adder[0] == 0) {
            adder.erase(adder.begin());
        }
    }
    bin.int_part = adder;
    return bin;
}

Bin Devide_Bin(Bin bin1, Bin bin2) {
    if (Pryam_To_Dec(bin2) != 0) {
        Bin bin;
        bin.sostoyanie = 0;
        bin.znak = (bin1.znak + bin2.znak) % 2;
        Bin carry;
        carry.sostoyanie = 0;
        carry.znak = 0;
        int devider = Pryam_To_Dec(bin2);
        if (devider < 0) {
            devider = devider * (-1);
        }
        for (int i = 0; i < bin1.int_part.size(); i++) {
            carry.int_part.push_back(bin1.int_part[i]);
            if (Pryam_To_Dec(carry) < devider) {
                bin.int_part.push_back(0);
            }
            else {
                bin.int_part.push_back(1);
                carry = Dec_To_Pryam(Pryam_To_Dec(carry) - devider);
            }
        }
        if (carry.int_part.size() != 0) {
            for (int i = 0; i < 5; i++) {
                carry.int_part.push_back(0);
                if (Pryam_To_Dec(carry) < devider) {
                    bin.double_part.push_back(0);
                }
                else {
                    bin.double_part.push_back(1);
                    carry = Dec_To_Pryam(Pryam_To_Dec(carry) - devider);
                }
            }
        }
        bin = Delete_Zero(bin);
        return bin;
    }
}

void Cout_Bin(Bin bin) {
    if (bin.sostoyanie == 0) {
        cout << "Прямой код: ";
    }
    else {
        if (bin.sostoyanie == 1) {
            cout << "Обратный код: ";
        }
        else {
            cout << "Дополнительный код: ";
        }
    }
    cout << bin.znak << " ";
    for (int i = 0; i < bin.int_part.size(); i++) {
        cout << bin.int_part[i];
    }
    cout << ".";
    for (int i = 0; i < bin.double_part.size(); i++) {
        cout << bin.double_part[i];
    }
}