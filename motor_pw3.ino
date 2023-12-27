#include <LiquidCrystal.h> 
//#include<SoftwareSerial.h>
#include <Wire.h>

// Variables for MPU 6050
float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float LoopTimer;
float degree;
float prev_degree = 0;
float lcd_degree;
int t;
int speedup;
float timer;

// Encoder Declaration
int EA = 2; // from 0  to 2
int EB = 10; // from 1 to 10

int Previous_OutputA;
int Previous_OutputB;
float EncoderA;
float EncoderB;
float Encoder_Count;

// MPU 6050 Declaration
// SDA && SCL

// Ultrasonic Sensor Declaration
//int trigPin = 12;
//int echoPin = 13;

// Bluetooth Declaration
//SoftwareSerial mySerial(0,1); //Rx & Tx

// Sensor Declaration
int R_S = 0; 
int L_S = 12; 

// Motor Declaration
int in1 = A1;  
int in2 = A2; 
int in3 = A3; 
int in4 = A0; 

// Speed Control Declaration
int enAL = 3;
int enBR = 11;

// Setup Declaration
int A;
unsigned long startTime;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int climbed = 0;

void setup(){

  Serial.begin(57600);
  
  //pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  //pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(EA,INPUT); // Left Encoder Input
  pinMode(EB,INPUT); // Right Encoder Input

  pinMode(L_S, INPUT); // Left Sensor Input
  pinMode(R_S, INPUT); // Right Sensor Input

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(enAL,OUTPUT);
  pinMode(enBR,OUTPUT);

  //===========================
  lcd.begin(16, 2); //Initialise 16*2 LCD

  // MPU 6050 Setup
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();


  analogWrite(enAL, 120);
  analogWrite(enBR, 120);
  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, HIGH);  //Left Motor forword Pin 
  delay(250);

  // Start Engine for Running

  // Encoder Reading
  Previous_OutputA = digitalRead(EA);
  Previous_OutputB = digitalRead(EB);
  Encoder_Count = 0;
}

void loop(){
  
  int time = millis();

  if (time % 40 == 0 && climbed == 0){
    gyro_signals();
  }

  Serial.print("Degree = ");
  Serial.println(AnglePitch);

  line_following();

  if (AnglePitch >= 5 && climbed == 0){

    climbing();
    climbed = 1;

    if (speedup == 0){
    analogWrite(enAL, 120);
    analogWrite(enBR, 120);
    digitalWrite(in1, LOW); //Right Motor forword Pin 
    digitalWrite(in2, HIGH);  //Right Motor backword Pin 
    digitalWrite(in3, LOW); //Left Motor backword Pin 
    digitalWrite(in4, HIGH);  //Left Motor forword Pin 
    delay(250);
    speedup = 1;

    }
  }

  if (climbed == 1) {
    line_following();
    distance2();
  }

}

void display_angle(){
  
  lcd.print("Degree : ");
  lcd.print(AnglePitch,2);

}

void task2(){
  lcd.clear();
  while(Encoder_Count<=0.7){
    distance();
    line_following();
  }
  Stop();
  delay(2000);
}

void line_following(){

  if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)){
    analogWrite(enAL, 0);
    analogWrite(enBR, 0);

    digitalWrite(in1, LOW); //Right Motor forword Pin 
    digitalWrite(in2, LOW); //Right Motor backword Pin 
    digitalWrite(in3, LOW); //Left Motor backword Pin 
    digitalWrite(in4, LOW); //Left Motor forword Pin 
  } 

  if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)){ // forward
    analogWrite(enAL, 64);
    analogWrite(enBR, 64);

    digitalWrite(in1, LOW); //Right Motor forword Pin 
    digitalWrite(in2, HIGH);  //Right Motor backword Pin 
    digitalWrite(in3, LOW);  //Left Motor backword Pin 
    digitalWrite(in4, HIGH); //Left Motor forword Pin 
    //distance();
  } 

  if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){ // turn left
    analogWrite(enAL, 255);
    analogWrite(enBR, 255);

    digitalWrite(in1, LOW); //Right Motor forword Pin 
    digitalWrite(in2, HIGH);  //Right Motor backword Pin 
    digitalWrite(in3, HIGH); //Left Motor backword Pin 
    digitalWrite(in4, LOW);  //Left Motor forword Pin 
    //distance();
  } 

  if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){ // turn right
    analogWrite(enAL, 255);
    analogWrite(enBR, 255);

    digitalWrite(in1, HIGH);  //Right Motor forword Pin 
    digitalWrite(in2, LOW); //Right Motor backword Pin  
    digitalWrite(in3, LOW);  //Left Motor backword Pin 
    digitalWrite(in4, HIGH); //Left Motor forword Pin 
    //distance();
  }  
}

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
  AnglePitch=atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180) + 8;
}

void distance(){
    EncoderA=EncodeA();
    EncoderB=EncodeB();
    Encoder_Count = (EncoderA + EncoderB)/2.0;
    lcd.clear();
    lcd.print("Distance:");
    lcd.print(Encoder_Count,2);
    lcd.print("m");
    lcd.setCursor(0,1);
    lcd.print("Time:");
    lcd.print(timer/100.0);
    lcd.print("s");
    delay(1);
  timer++;
}

void distance2(){
    EncoderA=EncodeA();
    EncoderB=EncodeB();
    Encoder_Count = (EncoderA + EncoderB)/2.0;
    lcd.clear();
    lcd.print("Distance:");
    lcd.print(Encoder_Count,2);
    lcd.print("m");
    lcd.setCursor(0,1);
    lcd.print("Time:");
    lcd.print(timer/100.0);
    lcd.print("s");
    delay(1);
  timer++;
}

float EncodeA(){
  if (digitalRead(EA) != Previous_OutputA){
    EncoderA+=((1.0/3900.0)*21.2);
  }
  Previous_OutputA=digitalRead(EA);
  return EncoderA;
}

float EncodeB(){
  if (digitalRead(EB) != Previous_OutputB){
    EncoderB+=((1.0/3900.0)*21.2);
  }
  Previous_OutputB=digitalRead(EB);
  return EncoderB;
}

void climbing(){  //forword

  // go up ramp
  analogWrite(enAL, 230);
  analogWrite(enBR, 230);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 

  delay(450);

  Stop();
  gyro_signals();
  
  do{
    display_angle();
  }while(0==1);

  delay(1000);

  analogWrite(enAL, 255);
  analogWrite(enBR, 255);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 

  delay(650);

  // ready for 360
  analogWrite(enAL, 0);
  analogWrite(enBR, 0);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, LOW); //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, LOW); //Left Motor forword Pin 
  delay(4000);

  // turn 360
  analogWrite(enAL, 255);
  analogWrite(enBR, 255);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, HIGH); //Left Motor backword Pin 
  digitalWrite(in4, LOW);  //Left Motor forword Pin 

  delay(1180);
  
  Stop();

  delay(1000);

  // Continue for Task 2
  
  analogWrite(enAL, 80);
  analogWrite(enBR, 80);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 

  delay(900);

  /*
  for (t=0;t=300;t++){
    line_following();
    delay(1);
  }

  Stop();
  delay(1000);
  */

  task2();
}

/*
void forward(){  //forword
  analogWrite(enAL, 70);
  analogWrite(enBR, 70);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnRight(){ //turnLeft

  analogWrite(enAL, 255);
  analogWrite(enBR, 255);

  digitalWrite(in1, HIGH);  //Right Motor forword Pin 
  digitalWrite(in2, LOW); //Right Motor backword Pin  
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 

  delay(50);
  Stop();
}

void turnLeft(){
  analogWrite(enAL, 255);
  analogWrite(enBR, 255);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, HIGH); //Left Motor backword Pin 
  digitalWrite(in4, LOW);  //Left Motor forword Pin 

  delay(50);
  Stop();

}
*/

void Stop(){ //stop
  analogWrite(enAL, 0);
  analogWrite(enBR, 0);
  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, LOW); //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, LOW); //Left Motor forword Pin 
}

void serialcheck(){

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

}