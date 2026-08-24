/*
  Make sure your Firebase project's '.read' and '.write' rules are set to 'true'. 
  Ignoring this will prevent the MCU from communicating with the database. 
  For more details- https://github.com/Rupakpoddar/ESP8266Firebase 
*/

#include <time.h>
#include <FirebaseESP8266.h>
// #include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include "RTClib.h"
#include "max6675.h"


// int thermoDO = 14;
// int thermoCS = 4;
// int thermoCLK = 5;

int thermoDO = 0;
// int thermoCS = 4;
int thermoCS = 2;
// int thermoCLK = 5;
int thermoCLK = 16;

float Temperature;
unsigned char Masked;

// MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


FirebaseESP8266 firebase("https://firstdb-e4a0f-default-rtdb.firebaseio.com/");
bool firebaseConnected = false;

// #define _SSID "vivo Y28 5G"          // Your WiFi SSID
// #define _PASSWORD "1006disha"      // Your WiFi Password

// #define _SSID "Asus"          // Your WiFi SSID
// #define _PASSWORD "@Allahis1@"      // Your WiFi Password

#define _SSID "Pragatie"          // Your WiFi SSID
#define _PASSWORD "Pra@@1235"      // Your WiFi Password

// #define _SSID "Personal WiFi"          // Your WiFi SSID
// #define _PASSWORD "kkkkkkkk"      // Your WiFi Password

// #define _SSID "Roshan Time"          // Your WiFi SSID
// #define _PASSWORD "@Allahis1#"      // Your WiFi Password

// #define _SSID "Sutar_2.4G"          // Your WiFi SSID
// #define _PASSWORD "s9820343888"      // Your WiFi Password

// #define _SSID "Personal WiFi"          // Your WiFi SSID
// #define _PASSWORD "kkkkkkkk"      // Your WiFi Password

#define REFERENCE_URL "https://firstdb-e4a0f-default-rtdb.firebaseio.com/"  // Your Firebase project reference url


RTC_DS3231 RTC;

DateTime now;
unsigned char PreviousSeconds, Seconds, Minutes, Hours;
unsigned char iDuration;
unsigned char  Date, Month;
unsigned int  Year;
struct tm timeinfo;

// Firebase firebase(REFERENCE_URL);

unsigned int Count;
unsigned int Command;
String FeedTime,str1;
String StartTimeHours, StartTimeMinutes;
unsigned char iStartTimeHours, iStartTimeMinutes;
unsigned int Status;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
// const int   daylightOffset_sec = 3600;
const int   daylightOffset_sec = 0;

unsigned char OutputStatus;

#define   FEEDER_OUTPUT 12
#define   LED_BUILTIN 2
#define   RELAY1  15
#define   RELAY2  13

String  strOxygenTimer;
unsigned char OxygenTimer;


void setup() 
{
  delay(3000);
  Serial.begin(115200);
  delay(3000);
  Serial.println ();
  Serial.println ();
  Serial.println ("Starting Program...");

  pinMode(thermoCS, OUTPUT);
  pinMode(thermoCLK, OUTPUT);
  pinMode(thermoDO, INPUT_PULLUP);

  // RTC.begin();  // Init RTC

  // // pinMode(LED_BUILTIN, OUTPUT);
  // // pinMode(FEEDER_OUTPUT, OUTPUT);
  // // pinMode(RELAY1, OUTPUT);
  // return ;


  RTC.begin();  // Init RTC

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FEEDER_OUTPUT, OUTPUT);
  pinMode(RELAY1, OUTPUT);

  // int thermoDO = 0;
  // int thermoCS = 4;
  // int thermoCLK = 5;

    pinMode(thermoCS, OUTPUT);
    pinMode(thermoCLK, OUTPUT);
    pinMode(thermoDO, INPUT_PULLUP);
    // pinMode(thermoDO, OUTPUT);
    pinMode(D6, OUTPUT);

    digitalWrite(LED_BUILTIN, HIGH);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);

    // Connect to WiFi
    Serial.println();
    Serial.println();
    Serial.print("Connecting to: ");
    Serial.print(_SSID);
    Serial.print("  ");
    Serial.println(_PASSWORD);
    WiFi.begin(_SSID, _PASSWORD);

    while (WiFi.status() != WL_CONNECTED) 
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(150);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(150);
      Serial.print("-");
    }

    Serial.println("");
    Serial.println("WiFi Connected");

    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);

    // Print the IP address
    Serial.print("IP Address: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    // digitalWrite(LED_BUILTIN, HIGH);

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //================================================================//
  //================================================================//

    // Examples of setting String, integer and float values.
    // firebase.setString("Example/setString", "It's Working");
    // int data2 = firebase.getInt("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Duration");
    // firebase.setInt("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Duration", 83);
    // firebase.setInt("Example/setInt", 321);
    // firebase.setFloat("Example/setFloat", 45.32);

    // Examples of pushing String, integer and float values.
    // firebase.pushString("push", "Hello");
    // firebase.pushInt("push", 456);
    // firebase.pushFloat("push", 89.54);

    // Example of getting a String.
    // String data1 = firebase.getString("Example/setString");
    // Serial.print("Received String:\t");
    // Serial.println(data1);

    // Example of getting an int.
    // int data2 = firebase.getInt("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Duration");
    // int data2 = firebase.getInt("Example/setInt");

  //****

    // String data1 = firebase.getString("Fruits/Product");
    // Serial.print("Received Product:\t");
    // Serial.println(data1);

    String data2 = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Duration");
    Serial.print("Duration :\t\t");
    Serial.println(data2);

    StartTimeHours = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/StartTimeHour");
    Serial.print("Start Time :\t\t");
    Serial.print(StartTimeHours);
    Serial.print(":");

    StartTimeMinutes = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/StartTimeMinutes");
    // Serial.print("Received FeedTime :\t\t");
    Serial.print(StartTimeMinutes);
    Serial.print("\t\t");

    strOxygenTimer = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/OxygenTimer");
    // Serial.print("Received FeedTime :\t\t");
    Serial.print(strOxygenTimer);
    Serial.println (" ");

    Count=5;
    // Serial.print(StartTimeHours);

    // Example of data deletion.
    // firebase.deleteData("Example");

    RTC.adjust(DateTime(2024, 8, 25, 21, 12, 0));

    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_OFF);

}



void printLocalTime()
{
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  // Serial.print("Day of week: ");
  // Serial.println(&timeinfo, "%A");
  // Serial.print("Month: ");
  // Serial.println(&timeinfo, "%B");
  // Serial.print("Day of Month: ");
  // Serial.println(&timeinfo, "%d");
  // Serial.print("Year: ");
  // Serial.println(&timeinfo, "%Y");
  // Serial.print("Hour: ");
  // Serial.println(&timeinfo, "%H");
  // Serial.print("Hour (12 hour format): ");
  // Serial.println(&timeinfo, "%I");
  // Serial.print("Minute: ");
  // Serial.println(&timeinfo, "%M");
  // Serial.print("Second: ");
  // Serial.println(&timeinfo, "%S");

  // Serial.println("Time variables");
  char timeHour[3],timeMin[3],timeSecond[3], timeDate[6], timeMonth[6], timeYear[8];
  String str1;

  strftime(timeHour,3, "%H", &timeinfo);
  strftime(timeMin,3, "%M", &timeinfo);
  strftime(timeSecond,3, "%S", &timeinfo);

  strftime(timeDate,6, "%d", &timeinfo);
  strftime(timeMonth,6, "%m", &timeinfo);
  strftime(timeYear,6, "%Y", &timeinfo);

  // char timeWeekDay[10];
  // strftime(timeWeekDay,10, "%S", &timeinfo);

  // Serial.print(timeHour); Serial.print (":");
  // Serial.print(timeMin); Serial.print (":");
  // Serial.print(timeSecond);
  // Serial.print ("  ");

  str1=String (timeSecond);
  Seconds=str1.toInt();
  str1=String (timeMin);
  Minutes=str1.toInt();
  str1=String (timeHour);
  Hours=str1.toInt();

  str1=String (timeDate);
  Date=str1.toInt();
  str1=String (timeMonth);
  Month=str1.toInt();
  str1=String (timeYear);
  Year=str1.toInt();

  // Serial.print(timeDate); Serial.print ("/");
  // Serial.print(timeMonth); Serial.print ("/");
  // Serial.println(timeYear);


  // Serial.println();
}



// int thermoDO = 0;
// int thermoCS = 4;
// int thermoCLK = 5;



void ReadMAX6675 (void)
{
  unsigned int Temp;
  unsigned char i,j;


  digitalWrite(thermoCS,  HIGH);
  delay (100);
  digitalWrite(thermoCLK, LOW);
  delay (100);
  digitalWrite(thermoCS,  LOW);
  delay (100);

  Temp=0;
  for (i=0; i<16; i++)
  {
    digitalWrite(thermoCLK, HIGH);
    delay (1);
    j=digitalRead(thermoDO);
    // delay (100);
    Temp<<=1;
    if (j!=0) Temp|=1;
    delay (1);
    digitalWrite(thermoCLK, LOW);
    delay (1);
  }
  digitalWrite(thermoCS, HIGH);

  Masked=Temp & 7;
  Temperature=Temp>>3;
  Temperature = (Temperature-32) * 5;
  Temperature/= 9;
  Temperature-= 12;
  return ;
}


void ReadMAX6675_1 (void)
{
  unsigned int Temp;
  unsigned char i,j;


  digitalWrite(thermoCLK, LOW);
  digitalWrite(thermoCS, LOW);
  delay (100);

  Temp=0;
  for (i=0; i<16; i++)
  {
    digitalWrite(thermoCLK, HIGH);
    delay (1);
    j=digitalRead(thermoDO);
    delay (1);
    Temp<<=1;
    if (j!=0) Temp|=1;
    digitalWrite(thermoCLK, LOW);
    delay (1);
  }
  digitalWrite(thermoCS, HIGH);

  Temperature=Temp;
  return ;
}



void loop() 
{
  String S1;
  unsigned int Count=0;


  // ReadMAX6675 ();
  // Serial.print ("Temperature= ");
  // Serial.println (Temperature);
  // delay (1000);
  // return ;



  printLocalTime();

  // for (;;)
  // {
  //   digitalWrite(LED_BUILTIN, HIGH);

  //   digitalWrite(D1, HIGH);
  //   digitalWrite(thermoDO, HIGH);
  //   digitalWrite(thermoCS, HIGH);
  //   delay (1000);
  //   digitalWrite(LED_BUILTIN, LOW);
  //   digitalWrite(D1, LOW);
  //   digitalWrite(thermoDO, LOW);
  //   digitalWrite(thermoCS, LOW);
  //   delay (1000);
  //   Count++;
  //   Serial.println (Count);
  // }
  // delay (1000);
  // return ;
  // Serial.print("C = ");
  // Serial.println(thermocouple.readCelsius());
  // delay (1000);


  // if(!getLocalTime(&timeinfo))
  // {
  //   Serial.println("Failed to obtain time");
  //   return;
  // }
  // Serial.print ("Seconds : ");
  // Serial.println(timeinfo.tm_sec);


  // now = RTC.now();
  // Seconds = now.second();
  // Serial.println (Seconds);
  // delay (1000);

  if (PreviousSeconds != Seconds) 
  {
    // Serial.print ("Diff:");
    // Serial.print (PreviousSeconds);
    // Serial.print ("#");
    // Serial.print (Seconds);
    PreviousSeconds = Seconds;
    // if  (Seconds & 0x01)
    // {
    //   digitalWrite(FEEDER_OUTPUT, HIGH);
    //   digitalWrite(LED_BUILTIN, HIGH);
    // }
    // else
    // {
    //   digitalWrite(FEEDER_OUTPUT, LOW);
    //   digitalWrite(LED_BUILTIN, LOW);
    // }

    // if (Seconds & 0x01)  
    //   digitalWrite(RELAY1, HIGH);
    // else
    //   digitalWrite(RELAY1, LOW);
    
    ReadMAX6675 ();

    Serial.print (" Time : ");
    Serial.print (Hours); Serial.print (":");
    Serial.print (Minutes); Serial.print (":");
    Serial.print (Seconds);
    Serial.print(" ");
    Serial.print(StartTimeHours);
    Serial.print(":");
    Serial.print(StartTimeMinutes);

    Serial.print ("\tTemperature= ");
    Serial.print (Temperature); Serial.print (" -> ");
    Serial.print (int(Temperature));


    // StartTimeMinutes = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/StartTimeMinutes");
    // // Serial.print("Received FeedTime :\t\t");
    // Serial.println(StartTimeMinutes);

    if ( (iStartTimeHours==Hours) && (iStartTimeMinutes==Minutes) && (Seconds<iDuration) )
    {
      OutputStatus=1;
      Serial.print("Feeder Gate Open.");
      digitalWrite(RELAY1, HIGH);
      digitalWrite(FEEDER_OUTPUT, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      if (OutputStatus==1)
      {
        // Serial.println ("Writing Status on Firebase");

        // Serial.print("Day of Month: ");
        S1=String(Date);
        S1+="-";
        S1+=String(Month);
        S1+="-";
        S1+=String(Year);
        S1+="-";

        S1+=String(Hours);
        S1+="-";
        S1+=String(Minutes);
        S1+="-";
        S1+=String(Seconds);
        // Serial.println (S1);


        firebase.setString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Status1", S1);
      }
      OutputStatus=0;
      digitalWrite(RELAY1, LOW);
      digitalWrite(FEEDER_OUTPUT, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
    }

    if (Seconds==25)
    {
      firebase.setString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Temperature", String(int(Temperature)));
      Serial.print ("  Start....");
      String datax = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Duration");  Serial.print ("#");
      StartTimeHours = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/StartTimeHour");  Serial.print ("#");
      StartTimeMinutes = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/StartTimeMinutes");  Serial.print ("#");
      strOxygenTimer = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/OxygenTimer");  Serial.print ("#");
      Serial.print ("...End\t\t");
      Serial.println ();

      Serial.print("Start Time : ");
      Serial.print(StartTimeHours);   Serial.print (":");
      Serial.print(StartTimeMinutes);

      str1=String (StartTimeHours);
      iStartTimeHours=str1.toInt();

      str1=String (StartTimeMinutes);
      iStartTimeMinutes=str1.toInt();

      str1=String (datax);
      iDuration=str1.toInt();

      Serial.print("  Duration: ");
      Serial.print(iDuration);

      Serial.print("  OxygenTimer: ");
      OxygenTimer=strOxygenTimer.toInt();
      Serial.print(OxygenTimer);
    }
    Serial.println();
    if (OxygenTimer!=0)
    {
      digitalWrite(D6, HIGH);
      OxygenTimer--;
      if (OxygenTimer==1)
        firebase.setString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/OxygenTimer", "0");
    }
    else
    {
      digitalWrite(D6, LOW);
    }

  }



  // delay (1000);
  // Serial.println (Count);
  // Count++;

  // delay (5000);
  // int datay = firebase.getInt("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Command");
  // Serial.print("Received Command:\t\t");
  // Serial.println(datay);

  // FeedTime = firebase.getString("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/FeedTime");
  // Serial.print("Received FeedTime :\t\t");
  // Serial.println(FeedTime);

  // Serial.print ("Setting Status to ");
  // Serial.println (Count);
  // firebase.setInt("FishFeeder/RIZWI_COLLEGE/2024/FISH_FEEDER/Status", Count);

  // Count++;
}
