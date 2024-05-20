#include "DHT.h"

#define DHTPIN 2     // Pin donde está conectado el sensor

//#define DHTTYPE DHT11   // Descomentar si se usa el DHT 11
#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);

typedef struct telemetry_status{ 

    double data;
//    double *data;
    double min_treshold;
    double max_treshold;   
        
}telemetry_t;


float get_temp(void *){
    float t = dht.readTemperature();
//    Serial.print(F("Temperatura: "));
//    Serial.print(t);
//    Serial.print(F(" °C"));
//    Serial.print(F("\n"));
    return t;
};

float get_hum(void *){
    float h = dht.readHumidity();
//    Serial.print(F("Humedad: "));
//    Serial.print(h);
//    Serial.print(F(" %"));
//    Serial.print(F("\n"));
    return h;
};
