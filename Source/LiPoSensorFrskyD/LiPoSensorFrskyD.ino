
#include <FrskyD.h>
#include <SoftwareSerial.h>

FrskyD FrskyD (4, 3);


void setup()
{
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  analogReference(INTERNAL);
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
  
  Serial.print(vfas);
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
  Serial.println(w6);

  
  FrskyD.sendCellVolt (0, w1);
  FrskyD.sendCellVolt (1, w2);
  FrskyD.sendCellVolt (2, w3);
  FrskyD.sendCellVolt (3, w4);
  FrskyD.sendCellVolt (4, w5);
  FrskyD.sendCellVolt (5, w6);
  
  delay (200);  // wait a bit to flush buffer
  
  digitalWrite(9, HIGH);
  digitalWrite(9, LOW);
}
