#include <iostream>
#include <cstdlib>
#include <cmath>

class Estadistica{
    private:
        int tamanyo, *conjunto, media;

    public:
        Estadistica(int _conjunto[], int _tamanyo);
        void cambiarConjunto(int _conjunto[], int _tamanyo);
        float obtenerMedia();
        float obtenerMediana();
        int obtenerRango();
        float obtenerVarianza();
        float obtenerDesviacionE();
        ~Estadistica();
};

Estadistica::Estadistica(int _conjunto[], int _tamanyo){
    int i, j, aux;
    tamanyo = _tamanyo;
    conjunto = new int [_tamanyo];
    for(i = 0; i < tamanyo; i++)
        *(conjunto + i) = _conjunto[i];

    for(i = 0; i < tamanyo; i++){
        for(j = 0; j < tamanyo; j++){
            if(*(conjunto + i) < *(conjunto + j)){
                aux = *(conjunto + i);
                *(conjunto + i) = *(conjunto + j);
                *(conjunto + j) = aux;
            }
        }
    }

    // Aqui hacemos la media porque la necesitaremos para las demas funciones
    float _media = 0;
    for(i = 0; i < tamanyo; i++)
        _media += *(conjunto + i);

    media = _media / tamanyo;
}

void Estadistica::cambiarConjunto(int _conjunto[], int _tamanyo){
    int i, j, aux;
    tamanyo = _tamanyo;
    delete[] conjunto;
    conjunto = new int [_tamanyo];
    for(i = 0; i < tamanyo; i++)
        *(conjunto + i) = _conjunto[i];

    for(i = 0; i < tamanyo; i++){
        for(j = 0; j < tamanyo; j++){
            if(*(conjunto + i) < *(conjunto + j)){
                aux = *(conjunto + i);
                *(conjunto + i) = *(conjunto + j);
                *(conjunto + j) = aux;
            }
        }
    }

    // Aqui hacemos la media porque la necesitaremos para las demas funciones
    float _media = 0;
    for(i = 0; i < tamanyo; i++)
        _media += *(conjunto + i);

    media = _media / tamanyo;
}

float Estadistica::obtenerMedia(){
    return media;
}

float Estadistica::obtenerMediana(){
    if(tamanyo % 2 == 0)
        return (*(conjunto + (tamanyo / 2)) + *(conjunto + ((tamanyo / 2) - 1))) / 2;
    else
        return *(conjunto + (tamanyo / 2));
}

int Estadistica::obtenerRango(){
    return (*(conjunto + (tamanyo - 1))) - (*(conjunto + 0));
}

float Estadistica::obtenerVarianza(){
    int _conjunto[tamanyo], i;
    float varianza, mediaCuadrado = 0;

    for(i = 0; i < tamanyo; i++)
        _conjunto[i] = (*(conjunto + i)) * (*(conjunto + i));

    for(i = 0; i < tamanyo; i++)
        mediaCuadrado += _conjunto[i];

    mediaCuadrado /= tamanyo;

    varianza = mediaCuadrado - (media * media);

    return varianza;
}

float Estadistica::obtenerDesviacionE(){
    float desviacionE = 0;
    int i;

    for(i = 0; i < tamanyo; i++)
        desviacionE += (*(conjunto + i) - media) * (*(conjunto + i) - media);

    desviacionE /= (tamanyo - 1);

    return sqrt(desviacionE);
}

Estadistica::~Estadistica(){
    delete[] conjunto;
}
