#include "FrSkySportSensorFcs.h"
#include "FrSkySportSensorFlvss.h"
#include "FrSkySportSingleWireSerial.h"
#include "FrSkySportTelemetry.h"
#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
#include "SoftwareSerial.h"
#endif

FrSkySportSensorFcs fcs;                               // Create FCS-40A sensor with default ID (use ID8 for FCS-150A)
FrSkySportSensorFlvss flvss1;                          // Create FLVSS sensor with default ID
FrSkySportTelemetry telemetry;                         // Create telemetry object

void setup()
{
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  analogReference(INTERNAL);
  
  // Configure the telemetry serial port and sensors (remember to use & to specify a pointer to sensor)
#if defined(__MK20DX128__) || defined(__MK20DX256__)
  telemetry.begin(FrSkySportSingleWireSerial::SERIAL_3, &ass, &fcs, &flvss1, &flvss2, &gps, &rpm, &sp2uart, &vario);
#else
  telemetry.begin(FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_3, &fcs, &flvss1);
#endif

}

float voltage(int adc, float r1, float r2)
{
  return (float)adc / 1023.0 * 1.1 * ((r2+r1)/r1);
}

void loop()
{
  int cell1 = analogRead(A0);
  int cell2 = analogRead(A1);
  int cell3 = analogRead(A2);
  int cell4 = analogRead(A3);
  int cell5 = analogRead(A4);
  int cell6 = analogRead(A5);

  float v1 = voltage(cell1, 3.9, 12);
  float v2 = voltage(cell2, 2.2, 15);
  float v3 = voltage(cell3, 1.8, 22);
  float v4 = voltage(cell4, 1.2, 18);
  float v5 = voltage(cell5, 1, 22);
  float v6 = voltage(cell6, 1, 22);

  float w1 = v1;
  float w2 = max(v2 - v1, 0);
  float w3 = max(v3 - v2, 0);
  float w4 = max(v4 - v3, 0);
  float w5 = max(v5 - v4, 0);
  float w6 = max(v6 - v5, 0);
  
  float vfas = w1 + w2 + w3 + w4 + w5 +w6;
  
  /*Serial.print(vfas);
  Serial.print(";");
  Serial.print(w1);
  Serial.print(";");
  Serial.print(w2);
  Serial.print(";");
  Serial.print(w3);
  Serial.print(";");
  Serial.print(w4);
  Serial.print(";");
  Serial.print(w5);
  Serial.print(";");
  Serial.println(w6);*/

  
  // Set current/voltage sensor (FCS) data
  // (set Voltage source to FAS in menu to use this data for battery voltage,
  //  set Current source to FAS in menu to use this data for current readins)
  fcs.setData(0,   // Current consumption in amps
              vfas);  // Battery voltage in volts

  // Set LiPo voltage sensor (FLVSS) data (we use two sensors to simulate 8S battery 
  // (set Voltage source to Cells in menu to use this data for battery voltage)
  flvss1.setData(w1, w2, w3, w4, w5, w6);  // Cell voltages in volts (cells 1-6)
  
  // Send the telemetry data, note that the data will only be sent for sensors
  // that are being polled at given moment
  telemetry.send();
  
  digitalWrite(9, HIGH);
  digitalWrite(9, LOW);
}
