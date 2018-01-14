#include <stdbool.h>
#include <SoftwareSerial.h>  
#include <Ardumoto.h>

#define ARRAY_SIZE 36
#define SCAN_NUM 10

int sendReceive();
void scanRoom();
void rotate();
int makeScan();

void driveForward();
void launch();

int solenoidPin   = 3;
int sensorPin     = 0;

int distanceArray[20] = {0};

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
  
  delay(2000);
  
  makeScan();

  while(hasLaunched == false)
  {
    do{
      decision = sendReceive();
      }while(decision == -1);

    if(decision == 0)
    {
      launch();
      hasLaunched = true;
    }

    else if(decision == 1)
    {
      driveForward();
    }

    else if(decision <= 2)
    {
      scanRoom();
    }

    else if(decision == -6)
      hasLaunched = true;
    
  }

  /*rotate();
  rotate();
  rotate();
  rotate();
  rotate();
  hasLaunched = true;*/

  while(hasLaunched == true)
  {
    delay(150);
  }

  

  

}

int sendReceive()
{
  int input = -1;
  
  for(int i = 0; i <= ARRAY_SIZE; i++)
    Serial.println(distanceArray[i]);

  delay(250);

  if(Serial.available() > 0)
    input = Serial.read();

  return input;
}

void launch()
{
  digitalWrite(solenoidPin, HIGH);
}

void scanRoom()
{
  int x;
  for(int n = 0; n < SCAN_NUM; n++)
  {
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
      distanceArray[i] += makeScan();
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

int makeScan()
{
  int val = analogRead(sensorPin);

  return val;
}

void rotate()
{
  Moto.setSpeed('A', 60);    //A is the left motor, B is the right motor
  Moto.setSpeed('B', 50);

  delay(100);

  Moto.stop('A');
  Moto.stop('B');

}

