#include<SoftwareSerial.h>

// Motor Declaration
int in1 = A1;
int in2 = A2;
int in3 = A3;
int in4 = A4;

// Speed Control Declaration
int enAL = 3;
int enBR = 11;

SoftwareSerial mySerial(0,1);
int speed=100;
char data;

void setup() {

  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(enAL,OUTPUT);
  pinMode(enBR,OUTPUT);

}

void loop() {
  if(mySerial.available()>0)
  {
    data=mySerial.read();
    Serial.println(data);
  }
  switch (data){
    case 'F':
      forward();
      break;
    case 'B':
      backward();
      break;
    case 'L':
      turnLeft();
      break;
    case 'R':
      turnRight();
      break;
    case 'S':
      Stop();
      break;

    case '0':
      speed = 100;
      break;

    case '1':
      speed = 110;
      break;

    case '2':
      speed = 120;
      break;

    case '3':
      speed = 130;
      break;

    case '4':
      speed = 140;
      break;

    case '5':
      speed = 150;
      break;

    case '6':
      speed = 160;
      break;

    case '7':
      speed = 180;
      break;

    case '8':
      speed = 200;
      break;

    case '9':
      speed = 255;
      break;
  }

}




void forward(){  //forword
  analogWrite(enAL, speed);
  analogWrite(enBR, speed);
  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void backward(){
  analogWrite(enAL, speed);
  analogWrite(enBR, speed);

  digitalWrite(in1, HIGH); //Right Motor forword Pin 
  digitalWrite(in2, LOW);  //Right Motor backword Pin 
  digitalWrite(in3, HIGH);  //Left Motor backword Pin 
  digitalWrite(in4, LOW); //Left Motor forword Pin 

}

void turnRight(){ //turnRight

  analogWrite(enAL, 255);
  analogWrite(enBR, 255);

  digitalWrite(in1, LOW);  //Right Motor forword Pin 
  digitalWrite(in2, HIGH); //Right Motor backword Pin  
  digitalWrite(in3, HIGH);  //Left Motor backword Pin 
  digitalWrite(in4, LOW); //Left Motor forword Pin 

}

void turnLeft(){ //turnLeft
  analogWrite(enAL, 255);
  analogWrite(enBR, 255);

  digitalWrite(in1, HIGH); //Right Motor forword Pin 
  digitalWrite(in2, LOW);  //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, HIGH);  //Left Motor forword Pin 

}

void Stop(){ //stop
  analogWrite(enAL, 0);
  analogWrite(enBR, 0);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, LOW); //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, LOW); //Left Motor forword Pin 
}