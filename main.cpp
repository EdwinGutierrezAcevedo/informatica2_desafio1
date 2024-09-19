#include <LiquidCrystal.h>
using namespace std;

bool bottom = false;
float voltaje, voltajeMax, voltajeMin, frecuencia,periodo,puntoMedio;
String tipo;
short int capacidad = 11;
float* arr = new float[capacidad];
short int cantElementos = 0;
short int capMax = 317;

void voltajeMaximo(float voltaje,float& voltajeMax);
void voltajeMinimo(float voltaje,float& voltajeMin);
void guardarEnArreglo(float*& arr, short int& capacidad, float nuevoElemento,short int& cantElementos);
void redimArr(float*& arr, short int& capacidad);
void miCopy(float* inicio, float* fin, float* destino);
void mostrarResultados(float voltaje, float frecuencia, char tipo);
float calcularPeriodo(float* arr, short int cantElementos, float puntoMedio);

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

void setup() {
    lcd_1.begin(16, 2);
    pinMode(A0, INPUT);
    pinMode(13, INPUT);
    pinMode(8, INPUT);
    Serial.begin(9600);
}

void loop() {
    if (digitalRead(13) == LOW) // si detecta botón inicio (izquierdo)
    {
        bottom = true;
        voltajeMax = -10.0;
        voltajeMin = 10.0;
    }
    while (bottom == true)
    {
        voltaje = analogRead(A0)* (5.0 / 1023.0);
        guardarEnArreglo(arr, capacidad, voltaje, cantElementos);
        //Serial.println(millis()); //Al hacer esto se observa que la tasa de refresco es de 6 milisegundos
        voltajeMaximo(voltaje,voltajeMax);
        voltajeMinimo(voltaje,voltajeMin);
        Serial.println(voltaje);
        if (digitalRead(8) == LOW)
        {
            bottom = false;
            puntoMedio = (voltajeMax + voltajeMin) / 2.0;
            periodo = calcularPeriodo(arr, cantElementos, puntoMedio);
            frecuencia = 1.0 / periodo;
            tipo=tipoOnda(arr,cantElementos,voltajeMax,puntoMedio,frecuencia);
            mostrarResultados((voltajeMax - voltajeMin) / 2.0, frecuencia, tipo); // imprimir en pantalla
            delete[] arr;
            cantElementos = 0;
            capacidad = 11;
            arr = nullptr;
            arr = new float[capacidad];

        }
    }
}

void voltajeMaximo(float voltaje,float& voltajeMax)
{
    if (voltaje>=voltajeMax)
    {
        voltajeMax = voltaje;
    }

}

void voltajeMinimo(float voltaje,float& voltajeMin)
{
    if (voltaje<=voltajeMin)
    {
        voltajeMin = voltaje;
    }

}

String tipoOnda(float* arr, int cantElementos,float voltajeMax,float puntoMedio,float frecuencia){
    float pendienteCresta1,diffPendientes,pendientePM1;
    float comparacion=frecuencia*(voltajeMax-voltajeMin)/100;
    bool repe=true;
    String tipo;

    for (int i=3;i<cantElementos;i++)
    {
        if ((voltajeMax - arr[i]<=0.1)&& repe ) //cerca de una cresta
        {
            pendienteCresta1=arr[i]-arr[i-1];
            repe=false;
        }
        if ((arr[i-1] < puntoMedio && puntoMedio <= arr[i])&& !repe) //cerca del punto medio
        {
            pendientePM1=arr[i]-arr[i-1];
            //}
            break;
        }
    }
    diffPendientes=pendientePM1-pendienteCresta1;
    if (diffPendientes==0||diffPendientes==(voltajeMax-voltajeMin))
    {
        tipo="cuadrada";
    }
    else if (diffPendientes>0 && diffPendientes<=frecuencia*(voltajeMax-voltajeMin))
    {
        if(diffPendientes<comparacion)
        {
            tipo="triangular";
        }
        else
        {
            tipo="sinusoidal";
        }
    }
    else
    {
        tipo="no identificada";
    }

    return tipo;
}

float calcularPeriodo(float* arr, short int cantElementos, float puntoMedio) {
    bool cruzando = false;
    float tiempoCruzadoInicio = 0;
    float tiempoCruzadoFinal = 0;
    short int cruces = 0;

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

void mostrarResultados(float voltaje, float frecuencia, String tipo) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Amplitud");
    lcd_1.setCursor(0, 1);
    lcd_1.print(voltaje);
    delay(4000);
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Frecuencia");
    lcd_1.setCursor(0, 1);
    lcd_1.print(frecuencia);
    delay(4000);
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Tipo de onda");
    lcd_1.setCursor(0, 1);
    lcd_1.print(tipo);
}

void miCopy(float* inicio, float* fin, float* destino) {
    while (inicio != fin) {
        *destino = *inicio;
        ++inicio;
        ++destino;
    }
}

void redimArr(float*& arr, short int& capacidad) {
    short int nuevaCap = capacidad * 30;
    float* nuevoArr = new float[nuevaCap];
    miCopy(arr, arr + capacidad, nuevoArr);
    delete[] arr;
    arr=nullptr;
    arr = nuevoArr;
    capacidad = nuevaCap;
}

void guardarEnArreglo(float*& arr, short int& capacidad, float nuevoElemento, short int& cantElementos) {
    if (cantElementos == capacidad) {
        redimArr(arr, capacidad);
    }
    arr[cantElementos] = nuevoElemento;
    if (cantElementos < capMax) {
        cantElementos++;
    } else {
        for (int j = 0; j < capacidad; j++) {
            arr[j] = arr[j + 1];
        }
    }
}
