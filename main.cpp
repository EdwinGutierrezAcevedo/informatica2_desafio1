#include <LiquidCrystal.h>
using namespace std;
int unsigned seconds = 0;
bool bottom = false;
int analogPin = A0;
float valor = 0;
float voltaje;
float voltajeMax;
float voltajeMin;
float frecuencia;
char tipo;
int capacidad = 2;
float* arr = new float[capacidad];
int cantElementos=0;
float puntoMedio;
int cont=0;
int capMax=115;

void mostrarResultados(float voltaje,float frecuencia,char tipo);
void miCopy(int* inicio, int* fin, int* destino);
void redimArr(float*& arr, int& capacidad);
void guardarEnArreglo(float*& arr, int & capacidad, float nuevoElemento, int&cantElementos);

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);


void setup()
{
    lcd_1.begin(16, 2); //inicializar lcd
    pinMode(A0,INPUT);
    pinMode (13,INPUT);
    pinMode (8,INPUT);
    Serial.begin(9600);
}

void loop()
{
    if (digitalRead(13)==LOW) // si detecta boton inicio (izquierdo)
    {
        bottom = true;
        voltajeMax=-10.0;
        voltajeMin=10.0;

    }
    while( bottom == true) //mientras no se presione el boton final (derecho)
    {

        valor = analogRead(analogPin);
        voltaje=valor*(5.0/1023.0);

        guardarEnArreglo(arr,capacidad, voltaje, cantElementos);
        Serial.println(voltaje); //graficar voltaje vs tiempo
        //Serial.println(cantElementos); // con esto se descubre que la cantidad maxima de memoria que se puede reservar antes de que el programa colapse
        if (voltaje>=voltajeMax)    //guardar voltaje maximo
        {
            voltajeMax=voltaje;
        }
        if (voltaje<=voltajeMin)  //guardar voltaje minimo
        {
            voltajeMin=voltaje;
        }
        if (digitalRead(8)==LOW)  // si detecta boton final
        {
            bottom=false;     //cambiamos el valor para salir del ciclo
            voltaje=(voltajeMax-voltajeMin)/2.0;
            puntoMedio=voltajeMax-voltaje;
            mostrarResultados(voltaje, frecuencia,tipo); //imprimir en pantalla
            //for (int i=0;i<cantElementos;i++){   //si se descomenta, se pueden ver los datos guardados en el arreglo
                //Serial.println(arr[i]); //simula la grafica registrada
            //}
            delete[] arr;
            arr = new float[capacidad];
            cantElementos=0;
            capacidad=2;
        }

    }

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
    int nuevaCap = capacidad *2;
    float* nuevoArr = new float[nuevaCap];
    miCopy(arr,arr+capacidad,nuevoArr);
    delete[] arr;
    arr = nuevoArr;
    capacidad = nuevaCap;
}


void guardarEnArreglo(float*& arr, int& capacidad, float nuevoElemento, int&cantElementos){
    if(cantElementos == capacidad){
        redimArr(arr,capacidad);
    }
    arr[cantElementos] = nuevoElemento;
    cantElementos++;

}
