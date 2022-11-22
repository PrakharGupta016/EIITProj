#include <LiquidCrystal.h>



// C++ code
//
LiquidCrystal lcd(7, 6, 12, 11,10,9);

int INq1 = 2;
int INq2 = 3;
int OUTq1 = 4;
int OUTq2 = 5;
int limit = 0;
int ledPin = 13;
int state = LOW;
int state2 = LOW;
int state3 = LOW;
int state4 = LOW;
//queue<int> qu;
int counter = 0;
int counter2 = 0;
int val = 0;
int flag =0;
//int flag2 =0;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
//int led_pin = 3;


int readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}



void setup()
{
  pinMode(ledPin, OUTPUT);
  //pinMode(inputIN, INPUT);
  //pinMode(inputOUT, INPUT);
  lcd.begin(16, 2); 
  lcd.setCursor(0,0);
  lcd.print("queue-1 ");
  lcd.setCursor(0,1);
  lcd.print("queue-2 ");
  
  pinMode(6, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              
  
  Serial.begin(9600);
}

void entryq1()
{
  limit = 100;
  
  val = 0.01273 * readUltrasonicDistance(INq1,INq1);
  if(val <= limit){
  	digitalWrite(ledPin, HIGH);
    state = HIGH;
  }
  else{
  	digitalWrite(ledPin, LOW);
    if(state == HIGH)
    {
      Serial.println("Person entered queue");
      counter++;
      lcd.setCursor(8,0);
      lcd.print(counter);
      flag=1;
      Serial.print("The number of people in the queue1 are: ");
      Serial.println(counter);
      state=LOW;
      
    }
  }
}

void entryq2()
{
  limit = 100;
  
  val = 0.01273 * readUltrasonicDistance(INq2,INq2);
  if(val <= limit){
  	digitalWrite(ledPin, HIGH);
    state2 = HIGH;
  }
  else{
  	digitalWrite(ledPin, LOW);
    if(state2 == HIGH)
    {
      Serial.println("Person entered queue");
      counter2++;
      lcd.setCursor(8,1);
      lcd.print(counter2);
      flag=1;
      Serial.print("The number of people in the queue2 are: ");
      Serial.println(counter2);
      state2=LOW;
      
    }
  }
}

void exitq1()
{
  if(radio.available())
  {
    char text[32]="";
    radio.read(&text, sizeof(text));
    radio.read(&button_state, sizeof(button_state));
    limit = 100;
    if(text == "q1_HIGH")
    {
      state3 = HIGH;
    }
    else if(text == "q1_LOW")
    {
      state3 = LOW;
    }

    if(state3 ==HIGH)
    {
      
      if(counter){
        counter--;
        flag=1;
        //lcd.setCursor(8,0);
     	  //lcd.print(counter);
      }
      else{
        Serial.println("Queue empty");
        state3 = LOW;
        return;
      }
      Serial.println("Person exited queue");
      Serial.print("The number of people in the queue1 are: ");
      Serial.println(counter);
      state3=LOW;
      
    }
  }
}

void exitq2()
{
  if(radio.available())
  {
    char text[32]="";
    radio.read(&text, sizeof(text));
    radio.read(&button_state, sizeof(button_state));
    limit = 100;
    if(text == "q2_HIGH")
    {
      state4 = HIGH;
    }
    else if(text == "q2_LOW")
    {
      state4 = LOW;
    }

    if(state4 ==HIGH)
    {
      
      if(counter2){
        counter2--;
        flag=1;
        //lcd.setCursor(8,0);
     	  //lcd.print(counter);
      }
      else{
        Serial.println("Queue empty");
        state4 = LOW;
        return;
      }
      Serial.println("Person exited queue");
      Serial.print("The number of people in the queue2s are: ");
      Serial.println(counter2);
      state4=LOW;
      
    }
  }
}

void repeat()
{
  entryq1();
  entryq2();
 // delay(10);
  exitq1();
  exitq2();
  //delay(10);
}

void loop()
{
  //digitalWrite(ledPin, HIGH);
  //delay(50);
  //digitalWrite(ledPin, LOW);
  //delay(50);
  repeat();
  if(counter > counter2 and flag)
  {
    Serial.print("Enter queue 2 : ");
    Serial.print(counter);
    Serial.print(" ");
    Serial.println(counter2);
  	flag=0;
  }
  else if(counter < counter2 and flag)
  {
    Serial.print("Enter queue 1 : ");
    Serial.print(counter);
    Serial.print(" ");
    Serial.println(counter2);
  	flag=0;
  }
  else if(counter == counter2 and flag)
  {
    Serial.print("Enter any queue : ");
    Serial.print(counter);
    Serial.print(" ");
    Serial.println(counter2);
  	flag=0;
  }
  //delay(100);
      
}