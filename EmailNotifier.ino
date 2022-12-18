//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "credentials.h"

const char *host = "mail31.lwspanel.com";
const int port = 143;
const char *email = "stephane@delta-consulting.fr";
const char *password = "Pvpo7puQ@hEjKBM";
char ch;
String Reponse;
#define LED_Blue 2
#define ALARM 5
String IMAP_Connected="* OK [CAPABILITY IMAP4rev1 LITERAL+ SASL-IR LOGIN-REFERRALS ID ENABLE IDLE STARTTLS AUTH=PLAIN AUTH=LOGIN] Dovecot ready.";
String IMAP_Login="a OK [CAPABILITY IMAP4rev1 LITERAL+ SASL-IR LOGIN-REFERRALS ID ENABLE IDLE SORT SORT=DISPLAY THREAD=REFERENCES THREAD=REFS THREAD=ORDEREDSUBJECT MULTIAPPEND URL-PARTIAL CATENATE UNSELECT CHILDREN NAMESPACE UIDPLUS LIST-EXTENDED I18NLEVEL=1 CONDSTORE QRESYNC ESEARCH ESORT SEARCHRES WITHIN CONTEXT=SEARCH LIST-STATUS SPECIAL-USE BINARY MOVE QUOTA] Logged in";


unsigned long previousMillis = 0; 
//const long interval = 300000; // 5 minutes
const long interval = 60000; // 5 minutes
WiFiClient client;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    pinMode(LED_Blue, OUTPUT);
    pinMode(ALARM, OUTPUT);
    SendAlarm(2000);
    Blink(5,1000,1);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  



    if (client.connect(host, port) == 1) 
         {
           Serial.println(F("connected"));
          } 
     else 
        {
          Serial.println(F("connection failed"));
         }


    
    //WiFi.println("EHLO www.example.com");
//
//  Serial.println("sending data to server");
//  if (client.connected()) {
//    client.println("a login ");
//  }

  

}

void loop()
{

 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
    {
      previousMillis = currentMillis;
       Serial.println("Connect + Login ");
       if (client.connect(host, port) == 1)    {    Serial.println(F("connected")); } else  { Serial.println(F("connection failed"));   }
    }
      




  
Reponse="";
  // Read all the lines of the reply from server and print them to Serial
 // Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    Serial.print(".");
    ch = static_cast<char>(client.read());
    Reponse += ch;
    //Serial.print(ch);
    
  }
//Serial.println(Reponse.substring( 0,Reponse.length()-2));
//Serial.println(IMAP_Connected);
if (Reponse.substring( 0,Reponse.length()-2)==IMAP_Connected) 
      {
        // envoi login
          Serial.println("envoi LOGIN");
          if (client.connected()) { client.println("a login stephane@delta-consulting.fr Pvpo7puQ@hEjKBM");}
      }
if (Reponse.substring( 0,Reponse.length()-2)==IMAP_Login) 
      {
        // envoi login
          Serial.println("envoi EXAMINE");
          if (client.connected()) { client.println("a EXAMINE inbox");}
      }
// 
//Serial.print("New MAIL : ");
//Serial.println(Reponse.indexOf("RECENT"));
if (Reponse.indexOf("UNSEEN")>0) 
      {
        // analyse new email
          Serial.println(Reponse.substring( 0,Reponse.length()-2));
          Serial.println("Anaylse new EMAIL");
          Serial.print("New MAIL : ");
          String sNbMail=Reponse.substring( Reponse.indexOf("UNSEEN")+7,Reponse.indexOf("UNSEEN")+9);
          Serial.println(sNbMail);
          int NbMail=sNbMail.toInt();
          Serial.println(NbMail);
          if (client.connected()) { client.println("a logout");}
          if (NbMail>0) {        SendAlarm(10000);}

          // envoi logout

      }


delay(2000);

}


void SendAlarm(int Tempo) {

    digitalWrite(ALARM, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(Tempo);                       // wait for a second
    digitalWrite(ALARM, LOW);    // turn the LED off by making the voltage LOW
 }



void Blink(int nb,int Tempo, bool Speed) {

  for (int i=0;i<nb;i++) {
    digitalWrite(LED_Blue, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(Tempo);                       // wait for a second
    digitalWrite(LED_Blue, LOW);    // turn the LED off by making the voltage LOW
    if (Speed==1){delay(Tempo/2);} else { delay(Tempo); }
  }
}
