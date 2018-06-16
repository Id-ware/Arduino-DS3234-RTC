/******************************************************************************
DS3234_RTC_Demo.ino
By Jim Lindblom @ SparkFun Electronics (date: October 2, 2016)
https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library
Configures, sets, and reads from the DS3234 real-time clock (RTC).
Resources: SPI.h - Arduino SPI Library
---------

Arduino Extremely Accurate RTC (via SPI Bus)
with I2C lcd display

THIS CODE IS EDITED BY: Micromix (Date: 29/5/2018 4:04-PM)
*****************************************************************************/

#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>

// Comment out the line below if you want month printed before date.
// E.g. October 31, 2016: 10/31/16 vs. 31/10/16
//#define PRINT_USA_DATE

// Configurable Pin Definitions:
#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

LiquidCrystal_I2C lcd(0x26,16,2);     //initializing i2c display (address and screen size)

DHT dht(DHTPIN, DHTTYPE);

int dayname;      //we use it to store the 'rtc.day' readout

int hum;  //Stores humidity value
int temp; //Stores temperature value

int tempspcae;
int humspace;

int passFlag = 0; //check if the function ran already once or not

byte degreechar[] = {
  0x04,
  0x0A,
  0x04,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

byte degreewithCchar[] = {
  0x08,
  0x14,
  0x08,
  0x03,
  0x04,
  0x04,
  0x03,
  0x00
};

byte RHchar[] = {
  0x1C,
  0x12,
  0x1C,
  0x12,
  0x00,
  0x12,
  0x1E,
  0x12
};

byte AMchar[] = {
  0x0C,
  0x12,
  0x1E,
  0x12,
  0x00,
  0x1B,
  0x15,
  0x11
};

byte PMchar[] = {
  0x1C,
  0x12,
  0x1C,
  0x10,
  0x00,
  0x1B,
  0x15,
  0x11
};



void setup() 
{
  // Use the serial monitor to view time/date output
//  Serial.begin(9600);

/* Call rtc.begin([cs]) to initialize the library
   The chip-select pin should be sent as the only parameter */
  rtc.begin(DS13074_CS_PIN);
  
  /* Use rtc.set12Hour to set to 12-hour mode */
  rtc.set12Hour(); 

/* Now set the time...
   You can use the autoTime() function to set the RTC's clock and
   date to the compiliers predefined time. (It'll be a few seconds
   behind, but close!) */
//   rtc.autoTime();
  
/* Or you can use the rtc.setTime(s, m, h, day, date, month, year)
   function to explicitly set the time:
   e.g. 7:32:16 | Monday October 31, 2016: */
// rtc.setTime(20, 55, 19, 3, 29, 5, 18);  // Uncomment to manually set time

  /* Update time/date values, so we can set alarms */
//  rtc.update();

///////////////////////////////////////////

  dht.begin();        // initialize temp sensor
  
  lcd.init();         // initialize the lcd 
  lcd.backlight();    // turn on backlighting
  lcd.home();         // set cursor home
  
  lcd.createChar(0, degreechar);
  lcd.createChar(1, degreewithCchar);
  lcd.createChar(2, RHchar);
  lcd.createChar(3, AMchar);
  lcd.createChar(4, PMchar);
///////////////////////////////////////////

}

void loop() 
{
  static int8_t lastSecond = -1;
  
  // Call rtc.update() to update all rtc.seconds(), rtc.minutes(),
  // etc. return functions.
  rtc.update();

  if (rtc.second() != lastSecond) // If the second has changed
  {
    printTime(); // Print the new time
    
    lastSecond = rtc.second(); // Update lastSecond value
  } 
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp = dht.readTemperature();
}

void timespacing()   //function to clear unwanted chars between time and AM/PM
{  
  if (rtc.hour() == 1 && rtc.minute() == 0 && rtc.second() < 1)
    {
      if (passFlag == 0)
       {
         lcd.setCursor(13, 0);
         lcd.print(" ");
         passFlag++;                     // Increment passFlag
       }
    }
   else if (rtc.hour() == 1 && rtc.minute() == 0 && rtc.second() == 1)
   {
     if (passFlag != 0)
     {
      passFlag = 0;
     }
   }
}

void tempspacing()   //function for tematrue spcing
{  
  if (rtc.date() >= 0 && rtc.date() <= 9)
  {
     if (rtc.month() >= 0 && rtc.month() <= 9)
      {
        tempspcae = 7;
          lcd.setCursor(tempspcae, 1);
          lcd.print(temp);
          lcd.write(1);
          
        humspace = 11; 
          lcd.setCursor(humspace, 1);
          lcd.print(hum);
          lcd.print("%");
          lcd.print("RH");     
      }
     else if (rtc.month() >= 10 && rtc.month() <= 12)
      {
        tempspcae = 8;
          lcd.setCursor(tempspcae, 1);
          lcd.print(temp);
          lcd.write(1);
        humspace = 12;
          lcd.setCursor(humspace, 1);
          lcd.print(hum);
          lcd.print("%");
          lcd.write(2);  
      }
   }
   else if (rtc.date() >= 10 && rtc.date() <= 32)
   {
       if (rtc.month() >= 0 && rtc.month() <= 9)
        {
        tempspcae = 8;
          lcd.setCursor(tempspcae, 1);
          lcd.print(temp);
          lcd.write(1);
        humspace = 12;
          lcd.setCursor(humspace, 1);
          lcd.print(hum);
          lcd.print("%");
          lcd.write(2);       
        }
       else if (rtc.month() >= 10 && rtc.month() <= 12)
        {
         tempspcae = 9;
          lcd.setCursor(tempspcae, 1);
          lcd.print(temp);
          lcd.write(1);
         humspace = 13;
          lcd.setCursor(humspace, 1);
          lcd.print(hum);
          lcd.print("%");
        }
    } 
}
void printTime()
{
  lcd.setCursor(0, 0); 
  lcd.print("*");
    /* this function below is to show the day in shorter format */
    dayname = rtc.day();      
  
    switch (dayname) {
    case 1:
      lcd.print("Sun");
      break;
    case 2:
      lcd.print("Mon");
      break;
    case 3:
      lcd.print("Tue");
      break;
    case 4:
      lcd.print("Wed");
      break;
    case 5:
      lcd.print("Thu");
      break;
    case 6:
      lcd.print("Fri");
      break;
    case 7:
      lcd.print("Sat");
      break;
    }
    
  lcd.print("*");
  
  lcd.setCursor(6, 0);
  lcd.print(String(rtc.hour()) + ":"); // Print hour
  if (rtc.minute() < 10)
    lcd.print('0'); // Print leading '0' for minute
  lcd.print(String(rtc.minute()) + ":"); // Print minute
  if (rtc.second() < 10)
    lcd.print('0'); // Print leading '0' for second
  lcd.print(String(rtc.second())); // Print second


  if (rtc.is12Hour()) // If we're in 12-hour mode 
  {
    timespacing();
    lcd.setCursor(14, 0);
    /* Use rtc.pm() to read the AM/PM state of the hour */
    if (rtc.pm()) lcd.print("PM"); // Returns true if PM
    else lcd.print("AM");
  }
  
  /* Few options for printing the day, pick one: */
  //lcd.print(rtc.dayStr()); // Print day string
  //lcd.print(rtc.dayC()); // Print day character
  //lcd.print(rtc.day()); // Print day integer (1-7, Sun-Sat)


  lcd.setCursor(0, 1);  /* moving the cursor to new line */
  
#ifdef PRINT_USA_DATE
  lcd.print(String(rtc.month()) + "/" +   // Print month
                 String(rtc.date()) + "/");  // Print date
#else
  lcd.print(String(rtc.date()) + "/" +    // (or) print date
                 String(rtc.month()) + "/"); // Print month
#endif
  lcd.print(String(rtc.year()));        // Print year

  tempspacing();    //calling for our spacing function
  
//  lcd.setCursor(tempspcae, 1);
//  lcd.print(temp);
//  lcd.print("C");
//  lcd.setCursor(humspace, 1);
//  lcd.print(hum);
//  lcd.print("%");
}
