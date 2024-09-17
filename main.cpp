#include <LiquidCrystal.h>
using namespace std;

bool bottom = false;
int analogPin = A0;
float valor = 0;
float voltaje, voltajeInicial, voltajeMax, voltajeMin, frecuencia;
float periodo;
char tipo;
int capacidad = 33;
float* arr = new float[capacidad];
int cantElementos = 0;
float puntoMedio;
double inicio,fin,diff;
int capMax = 300;

void guardarEnArreglo(float*& arr, int& capacidad, float nuevoElemento, int& cantElementos);
void redimArr(float*& arr, int& capacidad);
void miCopy(float* inicio, float* fin, float* destino);
void mostrarResultados(float voltaje, float frecuencia, char tipo);
float calcularPeriodo(float* arr, int cantElementos, float puntoMedio);


LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

void setup() {
    lcd_1.begin(16, 2);
    pinMode(A0, INPUT);
    pinMode(13, INPUT);
    pinMode(8, INPUT);
    Serial.begin(9600);
}

void loop() {
    if (digitalRead(13) == LOW) { // si detecta botón inicio (izquierdo)
        bottom = true;
        voltajeMax = -10.0;
        voltajeMin = 10.0;
        inicio=0;
    }
    while (bottom == true) {
        valor = analogRead(analogPin);
        voltaje = valor * (5.0 / 1023.0);
        guardarEnArreglo(arr, capacidad, voltaje, cantElementos);
        //Serial.println(millis()) //Al hacer esto se observa que la tasa de refresco es de 6 milisegundos
        Serial.println(voltaje);
        if (voltaje >= voltajeMax) {
            voltajeMax = voltaje;
        }
        if (voltaje <= voltajeMin) {
            voltajeMin = voltaje;
        }
        if (digitalRead(8) == LOW) {
            bottom = false;
            puntoMedio = (voltajeMax + voltajeMin) / 2.0;
            periodo = calcularPeriodo(arr, cantElementos, puntoMedio);
            frecuencia = 1.0 / periodo;
            mostrarResultados((voltajeMax - voltajeMin) / 2.0, frecuencia, tipo); // imprimir en pantalla
            delete[] arr;
            cantElementos = 0;
            capacidad = 33;
            arr = nullptr;
            arr = new float[capacidad];

        }
    }
}

float calcularPeriodo(float* arr, int cantElementos, float puntoMedio) {
    bool cruzando = false;
    float tiempoCruzadoInicio = 0;
    float tiempoCruzadoFinal = 0;
    int cruces = 0;

    for (int i = 1; i < cantElementos; i++)
    {
        if ((arr[i-1] < puntoMedio && puntoMedio <= arr[i]) || (arr[i-1] > puntoMedio && puntoMedio >= arr[i]))
        {
            if (!cruzando)
            {
                tiempoCruzadoInicio = millis();
                cruzando = true;
            }
            else {
                cruces++;
                if (cruces == 2)
                {
                    tiempoCruzadoFinal = millis();
                    break;
                }
            }
        }
        if(cruzando)
        {
            delay(6);
        }
    }

    tiempoCruzadoFinal=millis()-tiempoCruzadoInicio;

    if (cruces < 2)
    {
        return 0; // No se detectaron suficientes cruces para calcular el periodo
    }

    return (tiempoCruzadoFinal)/1000.0; // Convertir a segundos
}




void mostrarResultados(float voltaje,float frecuencia,char tipo){
    lcd_1.setCursor(0,0);
    lcd_1.print("Amplitud= ");
    lcd_1.setCursor(9,0);
    lcd_1.print(voltaje);
}



void miCopy(float* inicio, float* fin, float* destino) {
    while (inicio != fin) {
        *destino = *inicio;
        ++inicio;
        ++destino;
    }
}

void redimArr(float*& arr, int& capacidad){
    int nuevaCap = capacidad *3;
    float* nuevoArr = new float[nuevaCap];
    miCopy(arr,arr+capacidad,nuevoArr);
    delete[] arr;
    arr=nullptr;
    arr = nuevoArr;
    capacidad = nuevaCap;
}


void guardarEnArreglo(float*& arr, int& capacidad, float nuevoElemento, int&cantElementos){
    if(cantElementos == capacidad){
        redimArr(arr,capacidad);
    }
    arr[cantElementos] = nuevoElemento;
    if(cantElementos<capMax){
        cantElementos++;
    }
    else{
        for(int j=0;j<capacidad;j++){
            arr[j]=arr[j+1];
        }


    }

}
