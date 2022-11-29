// #include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);                // CE, CSN
const byte address[6] = "00001";  //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
int button_pin = 2;
boolean button_state = 0;



// C++ code
//
// LiquidCrystal lcd(7, 6, 12, 11,10,9);

int INq1 = 2;
int INq2 = 3;
int OUTq1trig = 4;
int OUTq1echo = 5;
int OUTq2trig = 6;
int OUTq2echo = 7;
// int OUTq2 = 5;
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
int flag = 0;
//int flag2 =0;


int readUltrasonicDistance(int triggerPin, int echoPin) {
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





void setup() {
  pinMode(ledPin, OUTPUT);
  //pinMode(inputIN, INPUT);
  //pinMode(inputOUT, INPUT);
  //  lcd.begin(16, 2);
  //   lcd.setCursor(0,0);
  // lcd.print("queue-1 ");
  // lcd.setCursor(0,1);
  // lcd.print("queue-2 ");
  pinMode(button_pin, INPUT);
  radio.begin();                   //Starting the Wireless communication
  radio.openWritingPipe(address);  //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);   //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();
  //This sets the module as transmitter



  Serial.begin(9600);
}


void exitq1() {
  Serial.println("zYo");



  limit = 15;

  val = 0.01273 * readUltrasonicDistance(OUTq1trig, OUTq1echo);
  if (val <= limit and val > 0) {

    state3 = HIGH;
    const char text[] = "q1_HIGH";
    unsigned int pp = 11;
    // Serial.println(text);
    // radio.write(&pp, sizeof(pp));
    Serial.println("queue1 transmitted high");

  } else {
    const char text[] = "q1_LOW";
    unsigned int pp = 10;
    // radio.write(&pp, sizeof(pp));
   
    if (state3 == HIGH) {
       Serial.println("queue1 transmitted low");
      pp=11;
      radio.write(&pp, sizeof(pp));
      Serial.println("queue1 data sent ");
      state3 = LOW;
    }
    else{

    }
  }
}

void exitq2() {
  limit = 15;

  val = (0.01273) * readUltrasonicDistance(OUTq2trig, OUTq2echo);
  if (val <= limit and val > 0) {
    state4 = HIGH;
    const char text[] = "q2_HIGH";
    unsigned int pp = 21;
    // Serial.println(text);
    // radio.write(&pp, sizeof(pp));

    Serial.println("queue2 transmitted high");

  } else {

    const char text[] = "q2_LOW";
    unsigned int pp = 20;
    if (state4 == HIGH) {
       Serial.println("queue2 transmitted low");
      pp=21;
      radio.write(&pp, sizeof(pp));
      Serial.println("queue2 data sent ");
      state4 = LOW;
    }

  }
}


void repeat() {


  exitq1();
  exitq2();
  //  merged();
  delay(100);
}

void loop() {
  //digitalWrite(ledPin, HIGH);
  //delay(50);
  //digitalWrite(ledPin, LOW);
  //delay(50);

  repeat();
  // if (counter > counter2 and flag) {
  //   Serial.print("Enter queue 2 : ");
  //   Serial.print(counter);
  //   Serial.print(" ");
  //   Serial.println(counter2);
  //   flag = 0;
  // } else if (counter < counter2 and flag) {
  //   Serial.print("Enter queue 1 : ");
  //   Serial.print(counter);
  //   Serial.print(" ");
  //   Serial.println(counter2);
  //   flag = 0;
  // } else if (counter == counter2 and flag) {
  //   Serial.print("Enter any queue : ");
  //   Serial.print(counter);
  //   Serial.print(" ");
  //   Serial.println(counter2);
  //   flag = 0;
  // }
}