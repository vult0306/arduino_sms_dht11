#include <SoftwareSerial.h>
#include <dht.h>

SoftwareSerial sim(10, 11); // RX , TX
dht DHT;

#define RELAY 12
#define DHT11_PIN 7

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;

int _timeout;
String _buffer;
String number = "+84369899816"; //-> change with your number +84792393770 +84702996423 +84392864506 0938934804
String ReadSMS = "";

void setup()
{
  delay(7000);
  Serial.begin(9600);
  
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  delay(10);
  _buffer.reserve(50);
  inputString.reserve(200);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  delay(1000);
  RecieveMessage();
  delay(1000);
  Read_Sim();
  delay(1000);
  SendMessage();
  delay(1000);
}
void loop()
{
  Read_Sim();
}

// digitalWrite(RELAY, HIGH);
void Read_Sim()
{
  while (sim.available())
  {
    char inChar = (char)sim.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete)
    {
      Serial.println(inputString);
      if (inputString.indexOf("ON") != -1)
      {
        Serial.println("ON LED");
        digitalWrite(RELAY, LOW);
        SendOnOff(LOW);
      }
      else if (inputString.indexOf("OFF") != -1)
      {
        Serial.println("OFF LED");
        digitalWrite(RELAY, HIGH);
        SendOnOff(LOW);
      }
      else if (inputString.indexOf("DHT") != -1)
      {
        int chk = DHT.read11(DHT11_PIN);
        Serial.print("Temperature = ");
        Serial.println(DHT.temperature);
        Serial.print("Humidity = ");
        Serial.println(DHT.humidity);
        SendDHT();
      }
      inputString = "";
      stringComplete = false;
    }
  }
}

void SendMessage()
{
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "START";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
void SendOnOff(int value)
{
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  if( value == LOW )
  {
    sim.println("MOTOR ON");
  }
  else if( value == HIGH )
  {
    sim.println("MOTOR OFF");
  }
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
void SendDHT()
{
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  sim.println("Temperature:\"" + String(DHT.temperature) + "\"\r");
  sim.println("Humidity:\"" + String(DHT.humidity) + "\"\r");
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  delay (1000);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  Serial.write ("Unread Message done");
}
String _readSerial()
{
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available())
  {
    return sim.readString();
  }
}
