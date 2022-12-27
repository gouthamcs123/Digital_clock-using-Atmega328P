//#define outEnable 4
//#define masterReset 5
#define dataPin 6
#define clockPin 7
#define latchPin 8
#define setButton 2
#define upButton 3
#define downButton 4

#include <Wire.h>    //Library for SPI communication
#include <DS3231.h>   //Library for RTC module 

DS3231 RTC;         //Declare object RTC for class DS3231

int h;              //Variable declared for hour
int m;              //Variable declared for minute

int thousands;
int hundreds;
int tens;
int unit;
byte setH = 0, setM = 0;
bool showHr = true, showMin = true;

bool h24;
bool PM;


void setup ()
{
  Wire.begin();
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //  pinMode(outEnable, OUTPUT);
  // pinMode(masterReset, OUTPUT);
  pinMode(setButton, INPUT_PULLUP);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  //digitalWrite(outEnable, LOW);
  //digitalWrite(masterReset, HIGH);
  //Serial.begin(9600);
  RTC.setClockMode(false);
}

void printTime()
{
  int h = RTC.getHour(h24, PM); //To get the Hour
  //if(h>12)h=h-12;
  int m = RTC.getMinute();      //TO get the minute
  int number = h * 100 + m;     //Converts hour and minute in 4-digit
  int thousands = number / 1000 % 10; //Getting thousands digit from the 4 digit
  int hundreds = number / 100 % 10; //Getting hundreds digit from 4 digit
  int tens = number / 10 % 10;    //Getting tens digit from 4-digit
  int unit = number % 10;         //Getting last digit from 4-digit
  int t = unit;
  int u = tens;
  int v = hundreds;
  int w = thousands;
  Serial.println(number);


  //Converting the individual digits into corresponding number for passing it through the shift register so LEDs are turned ON or OFF in seven segment
  switch (t)
  {
    case 0:
      unit = 63;
      break;
    case 1:
      unit = 06;
      break;
    case 2:
      unit = 91;
      break;
    case 3:
      unit = 79;
      break;
    case 4:
      unit = 102;
      break;
    case 5:
      unit = 109;
      break;
    case 6:
      unit = 125;
      break;
    case 7:
      unit = 07;
      break;
    case 8:
      unit = 127;
      break;
    case 9:
      unit = 103;
      break;
  }

  switch (u)
  {
    case 0:
      tens = 63;
      break;
    case 1:
      tens = 06;
      break;
    case 2:
      tens = 91;
      break;
    case 3:
      tens = 79;
      break;
    case 4:
      tens = 102;
      break;
    case 5:
      tens = 109;
      break;
    case 6:
      tens = 125;
      break;
    case 7:
      tens = 07;
      break;
    case 8:
      tens = 127;
      break;
    case 9:
      tens = 103;
      break;
  }

  switch (v)
  {
    case 0:
      hundreds = 63;
      break;
    case 1:
      hundreds = 06;
      break;
    case 2:
      hundreds = 91;
      break;
    case 3:
      hundreds = 79;
      break;
    case 4:
      hundreds = 102;
      break;
    case 5:
      hundreds = 109;
      break;
    case 6:
      hundreds = 125;
      break;
    case 7:
      hundreds = 07;
      break;
    case 8:
      hundreds = 127;
      break;
    case 9:
      hundreds = 103;
      break;
  }

  switch (w)
  {
    case 0:
      thousands = 63;
      break;
    case 1:
      thousands = 06;
      break;
    case 2:
      thousands = 91;
      break;
    case 3:
      thousands = 79;
      break;
    case 4:
      thousands = 102;
      break;
    case 5:
      thousands = 109;
      break;
    case 6:
      thousands = 125;
      break;
    case 7:
      thousands = 07;
      break;
    case 8:
      thousands = 127;
      break;
    case 9:
      thousands = 103;
      break;
  }
  if (showHr == true)
  {

    digitalWrite(9, HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~thousands); // The thousand digit is sent
    digitalWrite(latchPin, HIGH);  // Set latch pin HIGH to store the inputs
    digitalWrite(9, LOW);         // Turinig on that thousands digit
    delay(3);
    digitalWrite(9, HIGH); // delay for multiplexing

    digitalWrite(10, HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~hundreds );  // The hundered digit is sent
    digitalWrite(latchPin, HIGH);
    digitalWrite(10, LOW);
    delay(3);
    digitalWrite(10, HIGH);
  }
  if (showMin == true)
  {

    digitalWrite(11, HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~ tens); // The tens digit is sent
    digitalWrite(latchPin, HIGH);
    digitalWrite(11, LOW);
    delay(3);
    digitalWrite(11, HIGH);

    digitalWrite(12, HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~ unit); // The last digit is sent
    digitalWrite(latchPin, HIGH);
    digitalWrite(12, LOW);
    delay(3);
    digitalWrite(12, HIGH);
  }

}
void  timeSet()
{
  if (digitalRead(setButton ) == LOW)
  { delay(300);
    while (digitalRead(setButton) == HIGH)
    {
      setH = RTC.getHour(h24, PM);
      Serial.print(setH);
      if (digitalRead(upButton ) == LOW)
      {
        addHr();
        delay(300);

      }
      if (digitalRead(downButton) == LOW)
      {
        subHr();
        delay(300);
      }
      RTC.setHour(setH);
      showHr = true;
      showMin = false;
      printTime();

    }

    delay(500);
    while (digitalRead(setButton) == HIGH)
    {
      setM = RTC.getMinute();
      if (digitalRead(upButton) == LOW)
      {
        addMin();
        RTC.setMinute(setM);
        delay(300);

      }
      if (digitalRead(downButton) == LOW)
      {
        subMin();
        RTC.setMinute(setM);
        delay(300);
      }
      showHr = false;
      showMin = true;
      printTime();
    }
    delay(500);
    while (digitalRead(setButton) == HIGH)
    {

      showHr = true;
      showMin = true;
      printTime();
    }



  }


}

//adds a minute to new time
void addMin()
{
  if (setM < 59)
  {
    setM++;
  }
  else if (setM == 59)
  {
    setM = 0;
    addHr();
  }
}

//subtracts a minute from new time
void subMin()
{
  if (setM > 0)
  {
    setM--;
  }
  else if (setM == 0)
  {
    setM = 59;
    subHr();
  }
}

//adds an hour to new time
void addHr ()
{
  if (setH < 23)
  {
    setH++;
  }
  else if (setH == 23)
  {
    setH = 0;
  }
}

//subtracts an hour from new time
void subHr ()
{
  if (setH > 0)
  {
    setH--;
  }
  else if (setH == 0)
  {
    setH = 23;
  }
}

void loop()
{
  timeSet();
  printTime();


}
