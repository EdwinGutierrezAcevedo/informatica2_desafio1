#include <LiquidCrystal.h>

int seconds = 0;
int bottom = false;
int analogPin = A0;//para ver grafica en el monitor serial
int valor = 0; //para ver grafica en el monitor serial

float val = 0;
float voltaje;
float voltajeMax=0;
float voltajeMin=0;

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

    }
    while( bottom == true) //mientras no se presione el boton final (derecho)
    {
        valor = analogRead(analogPin);
        voltaje=valor*(5/1023.0);
        lcd_1.setCursor(0, 1);
        Serial.println(voltaje);
        if (voltaje>=voltajeMax)    //guardar voltaje maximo
        {
            voltajeMax=voltaje;
        }
        if (voltaje<=voltajeMin)  //guardar voltaje minimo
        {
            voltajeMin=voltaje;
        }

        Serial.println(voltaje); //graficar voltaje vs tiempo
        if (digitalRead(8)==LOW)  // si detecta boton final
        {
            bottom=false;          //cambiamos el valor para salir del ciclo

        }
    }
    lcd_1.setCursor(0,0);
    lcd_1.print("Amplitud= ");
    lcd_1.setCursor(9,0);
    lcd_1.print(voltajeMax);
}
