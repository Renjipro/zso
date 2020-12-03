#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//
//KEYPAD CONFIG SECTION
//
const byte ROWS = 4;
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20, 4);

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//
//SIM800L CONFIG SECTION
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+48"; //-> change with your number
String SMS = "Wiadomosc probna";
String tmp = "";
char incoming_char;
String str;
String var;



void setup() {
  //delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Aby wysłać sms naciśnij A, aby odebrać sms naciśnij B, aby zadzwonić naciśnij C, aby odebrać połączenie naciśnij D");
  //LCD CONFIG
  lcd.init();
  lcd.backlight();
  RecieveMessage();
}
void loop() {

  char key = keypad.getKey();

  if(key)
  {
  Serial.println(key);
  switch(key)
    {
      case 'A':
        while(Serial.available() > 0)
        {
            tmp += (char) Serial.read();
        }
          SMS = tmp;
          Serial.println(tmp);
          tmp = "";
          SendMessage();
        break;
      case 'B':
        ReadMessage();
        break;
      case 'C':
        callNumber();
        break;
      case 'D':
      sim.println("ATA");
        //receiveNumber();
        break;
      case '*':
        number = "+48";
        break;
      case '#':
        RecieveMessage();
        Serial.println(number);
        lcd.clear();
        lcd.setCursor(1, 1); // Set the cursor on the third column and first row.
        lcd.print(number); // Print the string "Hello World!"
        break;
      case '1':
        number = number + "1";
        break;
      case '2':
        number = number + "2";
        break;
      case '3':
        number = number + "3";
        break;
      case '4':
        number = number + "4";
        break;
      case '5':
        number = number + "5";
        break;
      case '6':
        number = number + "6";
        break;
      case '7':
        number = number + "7";
        break;
      case '8':
        number = number + "8";
        break;
      case '9':
        number = number + "9";
        break;
      case '0':
        number = number + "0";
        break;
    }
  }
    
  if (sim.available() > 0)
    Serial.write(sim.read());

}
void SendMessage()
{
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay (200);
  sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
  Serial.write ("Unread Message done");
  sim.println("AT+CPAS");
}

void ReadMessage()
{

  sim.println("AT+CMGL=\"ALL\"");
  var = readSerial();
  lcd.clear();
  lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
  lcd.print(var.substring(0,19));
  lcd.setCursor(0, 1);
  lcd.print(var.substring(20,39));
  lcd.setCursor(0, 2);
  lcd.print(var.substring(40,59));
  lcd.setCursor(0, 3);
  lcd.print(var.substring(60,79));
}

String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}

String readSerial()
{
    int timeOld = millis();

    delay(5000);
    str="";
    
    while(sim.available())
    {
        if (sim.available()>0)
        {
            str += (char) sim.read();
        }
    }

    return str;

}
