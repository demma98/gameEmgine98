#include "disk.h"

int Disk :: loadNumber(FILE *f){
    int res = 0;
    char temp = 'A';
    
    if(feof(f)){
        res = 1;
    }

    while ((temp != ' ') && !feof(f)) {
        res *= 26;
        res += temp - 'A';
        temp = fgetc(f);
    }

    return res;
}

void Disk :: saveNumber(FILE *f, int num){
    char temp[100];
    int i = 0;
    do{
        temp[i] = (num % 26) + 'A';
        num /= 26;
        i++;
    }
    while(num != 0);

    i--;
    while(i >= 0){
        fputc(temp[i], f);
        i--;
    }
    fputc(' ', f);
}