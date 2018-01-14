#include <stdbool.h>
#include <stdlib.h>
#include <SoftwareSerial.h>  
#include <Ardumoto.h>

#define ARRAY_SIZE 36
#define SCAN_NUM 3

int sendReceive();
void sendScan();
void scanRoom();
void rotate();
int makeScan();

void driveForward();
void launch(int radius);

int solenoidPin   = 5;
int sensorPin     = 0;

int distanceArray[ARRAY_SIZE] = {0};

Ardumoto Moto;

void setup() {
  Serial.begin(9600);

  pinMode(solenoidPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  Moto.begin();
}

void loop() {
  bool hasLaunched = false;
  int decision = -1;
  int rotation = 0;
  analogWrite(solenoidPin, 0);
  delay(5000);

  char flag = ' ';

  while(flag != 'R')
  {
    if(Serial.available() > 0)
      flag = Serial.read();
    
  }
  
  scanRoom();
  sendScan();

  while(hasLaunched == false)
  {
    delay(10000);
    scanRoom();
    do{
      decision = sendReceive();
      }while(decision == -1);

    launch(decision);
    hasLaunched = true;

    
  }/*

  for(int i = 0; i < 1; i ++){
    driveForward();
    Serial.println(analogRead(sensorPin));}
  hasLaunched = true;*/

  while(hasLaunched == true)
  {
    delay(150);
  }


}

int sendReceive()
{
  int input = -1;
  char rawInput[5] = {' '};

  Serial.println("SECONDSPIN");
  
  for(int i = 0; i <= ARRAY_SIZE; i++)
    Serial.println(distanceArray[i]);

  while(*rawInput == ' ')
  {
    if(Serial.available() > 0)
      *rawInput = Serial.read();
  }

  input = atoi(rawInput);
  
  return input;
}

void launch(int radius)
{
  int turns = radius - 200;
  int distance = distanceArray[turns];

  for(int i = 0; i <= turns; i++)
    rotate();

  for(int j = distance; j > 0; j = j-10);
    driveForward();

  for(int k = 0; k < 18; k++)
    rotate();
    
  analogWrite(solenoidPin, 255);
  delay(250);
  analogWrite(solenoidPin, 0);
  delay(250);
  analogWrite(solenoidPin, 255);
  delay(250);
  analogWrite(solenoidPin, 0);
}

void scanRoom()
{
  int x;
  for(int n = 0; n < SCAN_NUM; n++)
  {
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
      distanceArray[i] += makeScan();
      delay(100);
      rotate();
    }
  }

  for(int j = 0; j <ARRAY_SIZE; j++)
  {
    x = distanceArray[j];
    x -= x%10;
    x = x / SCAN_NUM;
    distanceArray[j] = x;
  }
}

void sendScan()
{
  Serial.println("FIRSTSPIN");
  
  for(int i = 0; i < ARRAY_SIZE; i++)
    Serial.println(distanceArray[i]);

}

int makeScan()
{
  int val = analogRead(sensorPin);

  return val;
}

void rotate()
{
  Moto.setSpeed('A', 60);    //A is the left motor, B is the right motor
  Moto.setSpeed('B', 50);

  delay(55);

  Moto.stop('A');
  Moto.stop('B');

}

void driveForward()
{
  Moto.setSpeed('A', -60);
  Moto.setSpeed('B', 50);
  
  delay(650);

  Moto.stop('A');
  Moto.stop('B');
}

