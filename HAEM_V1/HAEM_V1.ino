//blynk template connection
#define BLYNK_TEMPLATE_ID "TMP****cOLx"
#define BLYNK_TEMPLATE_NAME "light control"
#define BLYNK_AUTH_TOKEN "kIjmEqcx7pGD********wa1Y0sJKcmt"
#define BLYNK_PRINT Serial
#include "EmonLib.h"   //https://github.com/openenergymonitor/EmonLib
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define relay1_pin 25 //relay pin
int relay1_state = 1;
#define button1_pin 32 //button pin

EnergyMonitor emon;
#define vCalibration 512   // voltage calibration                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
#define currCalibration 0.46 //current calibration

// blynk start and wifi setup
BlynkTimer timer; 
char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "Tuntuna";
char pass[] = "mainnhiderha";
 
float kWh = 0;
float bill = 0;
unsigned long lastmillis = millis();
 
void myTimerEvent() {
 // voltage print
    emon.calcVI(20, 100);
    Serial.print("Vrms: ");
    Serial.print(emon.Vrms, 2);
    Serial.print("V");
    Blynk.virtualWrite(V0, emon.Vrms);
 // current print
    Serial.print("\tIrms: ");
    Serial.print(emon.Irms, 4);
    Serial.print("A");
    Blynk.virtualWrite(V1, emon.Irms);
 // power print
    Serial.print("\tPower: ");
    Serial.print(emon.apparentPower, 4);
    Serial.println("W");
 /* at the start volatge values are very high due to which the value of power consumed are very 
  high which will be inaccuarte so to minimize this if statement is added */
    if (emon.Vrms<265){
      Blynk.virtualWrite(V2, emon.apparentPower-0.12);
      Serial.print("\tkWh: ");
      kWh = kWh + emon.apparentPower*(millis()-lastmillis)/3600000000.0;
      Serial.print(kWh, 4);
      Serial.println("kWh");
      lastmillis = millis();
      Blynk.virtualWrite(V3, kWh);
     // energy meter billing 
      bill = kWh * 10;
     Serial.print(bill );
     Serial.println("₹");
     Blynk.virtualWrite(V5, bill);
    }

}

//blynk button setup 
#define button1_vpin V4
BLYNK_CONNECTED() {
  Blynk.syncVirtual(button1_vpin);
}
BLYNK_WRITE(button1_vpin) {
  relay1_state = param.asInt();
  digitalWrite(relay1_pin, relay1_state);
}

void setup() {
  Serial.begin(9600);
  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration); // Current: input pin, calibration.
  Blynk.begin(auth, ssid, pass);
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(relay1_pin, OUTPUT);
  digitalWrite(relay1_pin, HIGH);
  timer.setInterval(1000L, myTimerEvent);
}  
 
void loop() {
  Blynk.run();
  timer.run();
  listen_push_buttons();
}
// relay controlling using push button
void listen_push_buttons(){
    if(digitalRead(button1_pin) == LOW){
      control_relay(1);
      Blynk.virtualWrite(button1_vpin, relay1_state); //update button state
    }
}

void control_relay(int relay){
  if(relay == 0){
    relay1_state = !relay1_state;
    digitalWrite(relay1_pin, relay1_state);
    Serial.print("Relay1 State = ");
    Serial.println(relay1_state);
    delay(50);
  }

}
