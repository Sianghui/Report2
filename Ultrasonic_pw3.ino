#include <LiquidCrystal.h> 

// Motor Declaration
int in1 = A1;
int in2 = A2;
int in3 = A3;
int in4 = A4;

// Speed Control Declaration
int enAL = 3;
int enBR = 11;

// defines pins numbers
const int trigPin = 12;
const int echoPin = 13;

// defines variables
long duration;
int distance;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  lcd.begin(16, 2); //Initialise 16*2 LCD

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(enAL,OUTPUT);
  pinMode(enBR,OUTPUT);

}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  if(distance <=15){
    Stop();
  } else{
    forward();
  }
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
    
  // LCD Printer
  lcd.print(" Distance "); //Intro Message line 1
  lcd.setCursor(1, 1);
  lcd.print(distance); //Intro Message line 2
  lcd.print("cm");
  delay(50);
  lcd.clear();

}


void forward(){  //forword
  analogWrite(enAL, 100);
  analogWrite(enBR, 100);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, HIGH);  //Right Motor backword Pin 
  digitalWrite(in3, LOW);  //Left Motor backword Pin 
  digitalWrite(in4, HIGH); //Left Motor forword Pin 
}


void Stop(){ //stop

  analogWrite(enAL, 0);
  analogWrite(enBR, 0);

  digitalWrite(in1, LOW); //Right Motor forword Pin 
  digitalWrite(in2, LOW); //Right Motor backword Pin 
  digitalWrite(in3, LOW); //Left Motor backword Pin 
  digitalWrite(in4, LOW); //Left Motor forword Pin 
}