#include "fileNumbers.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

int comp(const void *a, const void *b) {
    const T aa = *(T*)a;
    const T bb = *(T*)b;

    if(aa < bb) {
        return -1;
    }
    if(aa > bb) {
        return 1;
    }

    return 0;
}

int ignoreRestOfLine(FILE* file) {
    while(1) {
        int c2 = fgetc(file);

        if('\n' == c2 || EOF == c2) { //jesli koniec linii lub koniec pliku zakoncz
            break;
        }
    }
}

void sortNumbers(struct Numbers* numbers) {
    if(numbers->numberCount > 0) {
        qsort(numbers->numbers, numbers->numberCount, sizeof(T), comp);
    }
}

void sortNumbers2(struct Numbers numbers) { //mozna tez bez wskaznika
    if(numbers.numberCount > 0) {
        qsort(numbers.numbers, numbers.numberCount, sizeof(T), comp);
    }
}

void removeDuplicates(struct Numbers* n) {

    if(n->numberCount < 2) {
        return;
    }

    size_t uniqueNumberCount = 0;
    for(size_t i = 1; i < n->numberCount; ++i) {
        if(n->numbers[i] != n->numbers[uniqueNumberCount]) {
            ++uniqueNumberCount;
        }
        n->numbers[uniqueNumberCount] = n->numbers[i];
    }
    n->numberCount = uniqueNumberCount + 1;
}

struct Numbers getSortedUniqueNumbersFromFile(const char* filename)
{
    struct Numbers numbers = {0, 0, 0};

    if(NULL == filename) {
        exit(1);
    }

    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        exit(2);
    }

    numbers.numbers = malloc(sizeof(T)*1000); //mamy to w naglowku (33. linia fileNumbers.h)

    while(1) {


        int c = fgetc(file); //czytanie po znaku

        if(EOF == c) {
            break; //jesli jest koniec pliku przerywamy petle
        }

        if('\n' == c) {
            ++numbers.linesCount;
            continue;
        }

        if(isdigit(c)) { //1 234
//            ungetc(c, file);//cofnac operacje odczytu aby cala liczba byla wczytana 1234 (tak to by było bez 1)
            fseek(file, -1, SEEK_CUR); //lepsza opcja od tego wyzej
            fscanf(file, "%" SCNd64, &numbers.numbers[numbers.numberCount]);
            ++numbers.numberCount;
        }

        if('#' == c) { //jesli na poczatku '#' zjadamy cala linie
            ignoreRestOfLine(file);
        }
    }

//    sortNumbers(&numbers);
    sortNumbers2(numbers);

    removeDuplicates(&numbers);

//  #error "W tym pliku nalezy dokonac implementacji, oczywiscie prosze potem usunac te linijke aby sie skompilowalo."
//  #error "Tresc co nalezy zrobic w pliku naglowkowym"

    return numbers;
}
