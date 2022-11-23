#include <LiquidCrystal.h>



// C++ code
//
LiquidCrystal lcd(7, 6, 12, 11,10,9);

int buzzPin = 6;
int INq1trig = 2;
int INq1echo = 3;
int INq2trig = 4;
int INq2echo = 5;
//int OUTq1 = 4;
//int OUTq2 = 5;
int limit = 0;
int ledPin = 13;
int state = LOW;
int state2 = LOW;
int state3 = LOW;
int state4 = LOW;
//queue<int> qu;
int counter = 0;
int counter2 = 0;
unsigned int val = 0;
int flag =0;

int max_limit =10;
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

void buzz()
{
  Serial.println("Limit crossed.");
  tone(buzzPin, 1000);
  delay(50);
  tone(buzzPin, 10);
  digitalWrite(buzzPin, LOW);
  delay(50);
}


void setup()
{
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
void recieve()
{
  if(radio.available())
  {
    char text[32]="";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}

void entryq1()
{
  limit = 25;
  
  val = 0.01273 * readUltrasonicDistance(INq1trig,INq1echo);
  if(val < limit and val>1)
  {
  	digitalWrite(ledPin, HIGH);
    state = HIGH;
  }
  else
  {
  	digitalWrite(ledPin, LOW);
    if(state == HIGH)
    {
      Serial.println("Person entered queue");
      counter++;
      flag=1;
      Serial.print("The number of people in the queue1 are: ");
      Serial.println(counter);
      state=LOW;
    }
  }
}

void entryq2()
{
  limit = 25;
  
  val = 0.01273 * readUltrasonicDistance(INq2trig,INq2echo);
  Serial.println(val);
  delay(50);
  if(val < limit and val>1){
  	//digitalWrite(ledPin, HIGH);
    state2 = HIGH;
  }
  else
  {
  	//digitalWrite(ledPin, LOW);
    if(state2 == HIGH)
    {
      Serial.println("Person entered queue");
      counter2++;
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
    const char text[] ="";
    unsigned int pp;
    radio.read(&pp, sizeof(pp));
//    Serial.println(pp);
    //radio.read(&button_state, sizeof(button_state));
    // limit = 25;
    if(pp == 11)
    {
//      Serial.println("q1_ok_high");
      state3 = HIGH;
    }
    else if(pp == 10)    
    {
//      Serial.println("q1_ok_low");
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
    const char text[]  ="";
    unsigned int pp;
    radio.read(&pp, sizeof(pp));
//    Serial.println(text);
//    radio.read(&button_state, sizeof(button_state));
    limit = 25;
    if(pp == 21)
    {
//      Serial.println("q2_ok_high");
      state4 = HIGH;
    }
    else if(pp == 20)
    {
//      Serial.println("q2_ok_low");
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
//  entry(state, counter, INq1trig, INq1echo);
//  entry(state2, counter2, INq2trig, INq2echo);
  if(counter >=10)
  { 
    buzz(); 
    Serial.print(" in queue 1");
  }
  if(counter2>=10)
  { 
    buzz();
    Serial.print(" in queue 2");
  }
//  entryq1();
//  entryq2();
 // delay(10);
//  recieve();
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
