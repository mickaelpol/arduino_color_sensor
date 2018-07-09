// TCS230 or TCS3200 pins wiring to Arduino
#define s0 4
#define s1 5
#define s2 6
#define s3 7
#define outPin 8

#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,A4,A5,3,2);

boolean DEBUG = true;

// Variables
int red;
int grn;
int blu;
String color ="";
int count = 0;
long startTiming = 0;
long elapsedTime =0;
Servo monServo;
int positionDuServo = 5;
int pinRed = 13;
int pinGreen = 10;
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;


/***************************************************
* Showing capured data at LCD
****************************************************/
void showDataLCD(void)
{
  lcd.print("Nina a m");
  lcd.setCursor(40,0); // Works for showing characters on the right half of GDM1601A controlled 16x1 LCD displays.
  lcd.print("anger : ");
  delay(500);
  lcd.clear();
  delay(500);
  lcd.setCursor(0,0);
  String ph = " fois";
  lcd.print(buttonPushCounter + ph);
  lcd.setCursor(40,0);
  lcd.print("        ");
  delay(500);
  lcd.clear();
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("Aujourd'");
  lcd.setCursor(40,0);
  lcd.print("hui     ");
  delay(500);
  lcd.clear();
  
}

/***************************************************
* Showing captured data at Serial Monitor
****************************************************/
void printData(void)
{
  Serial.print("red= ");
  Serial.print(red);
  Serial.print("   green= ");
  Serial.print(grn);
  Serial.print("   blue= ");
  Serial.print(blu);
  Serial.print (" - ");
  Serial.print (color);
  Serial.println (" detected!");
  
}



void setup() {
  // put your setup code here, to run once:
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT); //out from sensor becomes input to arduino
  
  // Led rouge et verte en fonction de l'etat du servomoteur
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  // Le Servomoteur
  monServo.attach(9);
  monServo.write(positionDuServo);

    // Setting frequency scaling to 100%
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);
  
  Serial.begin(9600);
  // lcd.noBacklight();
  lcd.begin(8,2);
  startTiming = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(pinGreen);
  Serial.println(buttonState);

  getColor();
  delay(500);
  if (DEBUG) printData(); 
  elapsedTime = millis()-startTiming; 

  if(color == "WHITE")
  {
    digitalWrite(pinRed, LOW);
    digitalWrite(pinGreen, HIGH);
    monServo.write(170);
    delay(15);
  } else {
      digitalWrite(pinRed, HIGH);
      digitalWrite(pinGreen, LOW);
      monServo.write(5);
      delay(15);
    }

      if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == 1) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println(buttonPushCounter);
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
  showDataLCD();
    
}

/* Based on RGB components ==> Determine color */
void getColor()
{  
  readRGB();
  
  if (red > 14 && red < 350 && grn > 16 && grn < 500 && blu > 14 && blu < 480) color = "WHITE";
  else  color = "BLACK";
}

/* read RGB components */
void readRGB() 
{
  red = 0;
  grn = 0;
  blu = 0;
  int n = 10;
  for (int i = 0; i < n; ++i)
  {
    //read red component
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    red = red + pulseIn(outPin, LOW);
  
  //read green component
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    grn = grn + pulseIn(outPin, LOW);
    
  //let's read blue component
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    blu = blu + pulseIn(outPin, LOW);
  }
  red = red/n;
  grn = grn/n;
  blu = blu/n;
}

