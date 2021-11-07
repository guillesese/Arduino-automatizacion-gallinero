/***   Included libraries  ***/
#include <Servo.h>

/***   Global variables and function definition  ***/
uint8_t PotToldo = A0;
Servo ServoToldo;
uint8_t SensorHuevos = 6;
uint8_t LedHuevos = 5;
uint8_t LedAlerta = 4;
uint8_t SensorLuz = A1;
uint8_t SensorDepredador = 3;
uint8_t ZumbadorAlerta = 2;
Servo ServoAlerta;
Servo ServoVentilador;
uint8_t BotonVentilador = 12;
float mapeado_pot = 0;
float valor_sensor = 0;
float estado_Ventilador = 0;
float velocidad_Ventilador = 0;

/** Función que controla el sensor de infrarrojos. Detecta la presencia o ausencia de elementos en el mismo.**/
void sensorHuevos() {
    if (digitalRead(SensorHuevos)) {
        digitalWrite(LedHuevos, HIGH);
        delay(500);
    }
    else {
        digitalWrite(LedHuevos, LOW);
        delay(500);
    }
}

/** Función que controla los depredadores. Sólo funciona en ausencia de luz y cuando detecta presencia
  en el sensor de infrarrojos correspondiente. En el caso de detectar presencia, mueve un servo, hace sonar
  un zumbador y enchufa o apaga un led.**/
void sensorDepredadores() {
    if (analogRead(SensorLuz) < 100 && digitalRead(SensorDepredador)) {
        for (int bitbloqForCounter = 0; bitbloqForCounter < 3; bitbloqForCounter += 1) {
            tone(ZumbadorAlerta, 261, 100);
            delay(100);
            digitalWrite(LedAlerta, HIGH);
            ServoAlerta.write(30);
            delay(200);
            ServoAlerta.write(10);
            delay(200);
        }
        delay(500);
        ServoAlerta.write(20);
    }
    else {
        ServoAlerta.write(20);
        digitalWrite(LedAlerta, LOW);
        delay(100);
    }
}

/** Función que modificará la velocidad del ventilador que dispone el gallinero. 
    Tendrá 3 velocidades de rotación, alta, baja y parado. Esta velocidad se controla
    a través de un botón.**/
void ventilacionGallinero() {
    if (digitalRead(BotonVentilador) == false) {
        ServoVentilador.write(velocidad_Ventilador);
    }
    else {
        if (estado_Ventilador == 0) {
            velocidad_Ventilador = 100;
            estado_Ventilador = 1;
        }
        else if (estado_Ventilador == 1) {
            velocidad_Ventilador = 180;
            estado_Ventilador = 2;
        }
        else {
            velocidad_Ventilador = 90;
            estado_Ventilador = 0;
        }
        delay(500);
    }
}


/***   Setup  ***/
void setup() {
    pinMode(PotToldo, INPUT);
    ServoToldo.attach(7);
    pinMode(SensorHuevos, INPUT);
    pinMode(LedHuevos, OUTPUT);
    pinMode(LedAlerta, OUTPUT);
    pinMode(SensorLuz, INPUT);
    pinMode(SensorDepredador, INPUT);
    pinMode(ZumbadorAlerta, OUTPUT);
    ServoAlerta.attach(8);
    ServoVentilador.attach(13);
    pinMode(BotonVentilador, INPUT);
    estado_Ventilador = 0;
    velocidad_Ventilador = 90;
    ServoAlerta.write(20);
}

/***   Loop  ***/
void loop() {
    if (analogRead(SensorLuz) > 100) {
        mapeado_pot = map(analogRead(PotToldo), 0, 1023, 5, 95);
        ServoToldo.write(mapeado_pot);
    }
    mapeado_pot = map(analogRead(PotToldo), 0, 1023, 5, 95);
    ServoToldo.write(mapeado_pot);
    sensorHuevos();
    sensorDepredadores();
    ventilacionGallinero();
}
