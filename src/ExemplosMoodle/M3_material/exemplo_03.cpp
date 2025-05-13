#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

unsigned char *open(string file, int &width, int &height) {
    ifstream arq(file);
    char BUFFER[1024];
    arq.getline(BUFFER, 1024);
    char type = BUFFER[1];
    cout << BUFFER << endl;
    do {
        arq.getline(BUFFER, 1024);
        cout << "." << BUFFER << "." << endl;
    } while (BUFFER[0] == '#');
    stringstream sstr(BUFFER);
    int w;
    int h;
    sstr >> w;
    sstr >> h;
    width = w;
    height = h;
    int maxValue;
    arq >> maxValue;
    cout << w << " X " << h << " mv: " << maxValue << endl; 
    unsigned char *data = new unsigned char [w * h * 3];
    if (type == '3') {
        // modo texto
        int j=0;
        while (!arq.eof()) {
            int g;
            arq >> g;
            data[j++] = (unsigned char)g;
        }
    } else { 
        // modo binário
        arq.read(reinterpret_cast<char*> (&data), sizeof(data));
    }
    arq.close();
    return data;
}

void save(string file, unsigned char *data, int &w, int &h) {
    ofstream arq(file);
    arq << "P3" << endl;
    arq << "#Gerado por chroma-key." << endl;
    arq << w << " " << h << endl << "255" << endl;
    int length = w * h * 3;
    for (int i = 0; i < length; i++) {
        arq << (int)data[i] << endl;
    }    
    arq.close();
}

double dist(int &r1, int &g1, int &b1, int &r2, int &g2, int &b2) {
    double r = r1 - r2;
    double g = g1 - g2;
    double b = b1 - b2;
    return sqrt(r*r + g*g + b*b);
}

void chromaKey(unsigned char *data, int w, int h) {
    int r, g, b;
    cout << "Cor-chave: " << endl;
    cout << "\tR: ";
    cin >> r;
    cout << "\tG: ";
    cin >> g;
    cout << "\tB: ";
    cin >> b;

    cout << "% Tolerência (0..1): ";
    double t;
    cin >> t;
    

    double dmax = 441.6729559301;

    int length = w * h * 3;
    for (int i = 0; i < length; i += 3) {
        int ri = data[i] & 0xff;
        int gi = data[i+1] & 0xff;
        int bi = data[i+2] & 0xff;
        double d = dist(r, g, b, ri, gi, bi);
        if (d/dmax < t) {
            data[i] = 0;
            data[i+1] = 0;
            data[i+2] = 0;
        }
    }
}

void grayScale(unsigned char *data, int w, int h) {
    cout << "Média aritmética (S) ou ponderada? ";
    char op;
    cin >> op;
    double rw, gw, bw;
    if ((op == 'S') || (op == 's')) {
        rw = gw = bw = 1.0/3.0;
    } else {
        rw = 0.2125; 
        gw = 0.7154;
        bw = 0.0721;
    }

    int length = w * h * 3;
    for (int i = 0; i < length; i += 3) {
        int ri = data[i] & 0xff;
        int gi = data[i+1] & 0xff;
        int bi = data[i+2] & 0xff;

        data[i] = data[i+1] = data[i+2] = (int)(ri * rw + gi * gw + bi * bw);
    }
}

void colorize(unsigned char *data, int w, int h) {
    int r, g, b;
    cout << "Cor de base: " << endl;
    cout << "\tR: ";
    cin >> r;
    cout << "\tG: ";
    cin >> g;
    cout << "\tB: ";
    cin >> b;
    
    int length = w * h * 3;
    for (int i = 0; i < length; i += 3) {
        int ri = data[i] & 0xff;
        int gi = data[i+1] & 0xff;
        int bi = data[i+2] & 0xff;

        data[i]   = ri | r;
        data[i+1] = gi | g;
        data[i+2] = bi | b;
    }
}

void negative(unsigned char *data, int w, int h) {
    int length = w * h * 3;
    for (int i = 0; i < length; i += 3) {
        int ri = data[i] & 0xff;
        int gi = data[i+1] & 0xff;
        int bi = data[i+2] & 0xff;

        data[i]   = ri ^ 255;
        data[i+1] = gi ^ 255;
        data[i+2] = bi ^ 255;
    }
}

int main() {
    string file;
    
    // AQUI PRA LER DO USUÁRIO O NOME DO ARQUIVO
    // cout << "Digite caminho para o arquivo da imagem de entrada: ";
    // getline(cin, file);
    file = "../src/ExemplosMoodle/M3_material/M3_exemplo1.ppm";

    int w, h;
    unsigned char *data = open(file, w, h);
    // cout << ((int)data[0]) << "..." << ((int)data[w * h * 3 - 1]) << endl;


    int opt;
    cout << "Qual opção de filtro você quer aplicar (1-chroma-key, 2-gray-scale, 3-colorize, 4-negative)? ";
    cin >> opt;

    switch(opt) {
        case 1:  chromaKey(data, w, h); break;
        case 2:  grayScale(data, w, h); break;
        case 3:  colorize(data, w, h);  break;
        case 4:  negative(data, w, h);  break;
        default: cout << "Opção inválida!!";
    }

    if ((opt > 0) && (opt < 5)){
        save("../src/ExemplosMoodle/M3_material/output.ppm", data, w, h);
    }
    
    delete [] data;
    return EXIT_SUCCESS;
}
