#include <Wire.h>

float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float LoopTimer;

void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8);
  Wire.endTransmission();                                                   
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
  AccX=(float)AccXLSB/4096;
  AccY=(float)AccYLSB/4096;
  AccZ=(float)AccZLSB/4096;
  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
  AnglePitch=atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1.0/(3.142/180.0);
}

void setup() {
  Serial.begin(57600);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  pinMode(0,INPUT);
  pinMode(1,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  pinMode(A0,OUTPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  //pinMode(A4,INPUT);
  //pinMode(A5,INPUT);

  digitalWrite(0,LOW);
}

void loop() {
  gyro_signals();
  /*
  Serial.print("    pin 0=");
  Serial.print(digitalRead(0));
  Serial.print("    pin 1=");
  Serial.print(digitalRead(1));
  Serial.print("    pin 2=");
  Serial.print(digitalRead(2));
  Serial.print("    pin 3=");
  Serial.print(digitalRead(3));
  Serial.print("    pin 4=");
  Serial.print(digitalRead(4));
  Serial.print("    pin 5=");
  Serial.print(digitalRead(5));
  Serial.print("    pin 6=");
  Serial.print(digitalRead(6));
  Serial.print("    pin 7=");
  Serial.print(digitalRead(7));
  Serial.print("    pin 8=");
  Serial.print(digitalRead(8));
  Serial.print("    pin 9=");
  Serial.print(digitalRead(9));
  Serial.print("    pin 10=");
  Serial.print(digitalRead(10));
  Serial.print("    pin 11=");
  Serial.print(digitalRead(11));
  Serial.print("    pin 12=");
  Serial.print(digitalRead(12));
  Serial.print("    pin 13=");
  Serial.print(digitalRead(13));
  Serial.print("    pin A0=");
  Serial.print(digitalRead(A0));
  Serial.print("    pin A1=");
  Serial.print(digitalRead(A1));
  Serial.print("    pin A2=");
  Serial.print(digitalRead(A2));
  Serial.print("    pin A3=");
  Serial.print(digitalRead(A3));
  Serial.print("    pin A4=");
  Serial.print(digitalRead(A4));
  Serial.print("    pin A5=");
  Serial.print(digitalRead(A5));
  */
  Serial.print(" Angle= ");
  Serial.println(AnglePitch);
  delay(50);
}