#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int contarMarcas() {
    string file = "marcas.txt";
    ifstream fileMarcas(file);
    string marca;

    int num_marcas = 0;
    if (fileMarcas.is_open()) {
        while (getline(fileMarcas, marca)) {
            num_marcas++;
        }
    }

    fileMarcas.close();

    return num_marcas;
}


int contarModelos() {
    string file = "modelos.txt";
    ifstream fileModelos(file);
    string modelo;

    int num_modelos = 0;
    if (fileModelos.is_open()) {
        while (getline(fileModelos, modelo)) {
            num_modelos++;
        }
    }

    fileModelos.close();

    return num_modelos;
}

int contarETS() {
    srand(time(0));
    return rand() % 6 + 3;
}