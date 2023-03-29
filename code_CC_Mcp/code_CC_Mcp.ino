#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>


// constantes(pin) pour les  capteurs ultrasons
#define trigger1 22
#define trig2 23
#define ultra1 17
#define ultra2 18

//pin pour l'ecran
#define rs 2
#define es 3
#define d4 4
#define d5 5
#define d6 6
#define d7 7

//pin  capteur de vibration
#define vib1 20
#define vib2 21

//constantes(pin) pour les les RXD et TXD
#define RXD 0
#define TXD 1

/* pin pour le pave numerique */
 // lignes
#define row1 8
#define row2 9
#define row3 10
#define row4 11

//colonnes
#define col1 12
#define col2 13
#define col3 14

//pin pour le capteur de gas
#define gas 16

//pin du capteur de flamme
#define flamme 15

//pin pour le capteur de  presence
#define pir 19

//alarme
#define alarme 24

const String PHONE = "+237682309580";
//SoftwareSerial GSM(RXD,TXD);
LiquidCrystal lcd(rs, es, d4, d5, d6, d7);

const byte rows=4;
const byte cols=3;

float distance;

char key[rows][cols]={
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[rows]={8, 9, 10, 11};
byte colPins[cols]={12, 13 ,14};
Keypad keypad= Keypad(makeKeymap(key),rowPins,colPins,rows,cols);
char* password="2345";
int currentposition=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigger1, OUTPUT);
  pinMode(ultra1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(ultra2, INPUT);
  pinMode(pir, INPUT_PULLUP);

  //vibration
  pinMode(vib1, INPUT);
  pinMode(vib2, INPUT);

  //gas
  pinMode(gas, INPUT);

  //flamme
  pinMode(flamme, INPUT);

  //alarme
  pinMode(alarme, OUTPUT);
  Serial.begin(9600);        
  //GSM.begin(9600);
  Serial.println("SIM900D initialiser");
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Bonjour Mr");
}

void sendSMS(String number, String msg){
  Serial.println("Envoi du message...");
  Serial.println("AT+CMGS=1");
  delay(100);
  Serial.print("AT+CMGS=\"");
  Serial.println(number);
  delay(500);
  Serial.println(msg); // Message contents
  delay(500);
  Serial.write(byte(26)); //Ctrl+Z  send message command (26 in decimal).
  delay(5000);  
}

void affichage(String message){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  }

void alarme2(){
  digitalWrite(alarme, HIGH);
  }

void vibrer_fenetre(String message){
  int vibrer=digitalRead(vib2);
  if (vibrer == HIGH){
    alarme2();
    String message1 = "Presence detecte"; 
    affichage(message1);
    sendSMS(PHONE, message);
    }
}

void vibrer_porte(String message){
  int vibrer=digitalRead(vib1);
  if (vibrer == HIGH){
    alarme2();
    String message1 = "Presence detecte"; 
    affichage(message1);
    sendSMS(PHONE, message);
    }
}

void ultrason_fenetre(){
  digitalWrite(trig2, LOW);
  delayMicroseconds(5);

  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);

  int duree=pulseIn(ultra2, HIGH);
  distance = (duree/2)/29;
  if (distance <= 150){
    alarme2();
    String message = "Presence dectete a : ";
    String mess = message + distance + " cm";
    String message1 = "Presence dectete"; 
    affichage(message1);
    sendSMS(PHONE, mess);
    Serial.println("SIM900D initialiser");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.setCursor(10, 1);
    lcd.print("cm");
    }
}

void ultrason_porte(){
  digitalWrite(trigger1, LOW);
  delayMicroseconds(5);

  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);

  int duree=pulseIn(ultra1, HIGH);
  distance = (duree/2)/29;
  if (distance <= 150){
    alarme2();
    String message = "Presence dectete a : ";
    String mess = message + distance + " cm";
    String message1 = "Presence dectete"; 
    affichage(message1);
    sendSMS(PHONE, mess);
    Serial.println("SIM900D initialiser");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.setCursor(10, 1);
    lcd.print("cm");
    }
}

void presence(){
   int detect = digitalRead(pir);
    
  if(detect == 1){
    Serial.println("Alerte presence detectee");
    digitalWrite(alarme, HIGH);
    
  if( currentposition==0)
{
    displayscreen();
 
} 

 int l ;
char code=keypad.getKey();
if(code!= NO_KEY)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PASSWORD:");
  lcd.setCursor(7,1);
  lcd.print(" ");
  lcd.setCursor(7,1);
for(l=0;l<=currentposition;++l)
{
   lcd.print("*");
  //keypress();
}
 
    if (code == password[currentposition]) {
      ++currentposition;
      if(currentposition == 4){
       
      unlockdoor();
      currentposition=0;
       
      } 
    }
     
    else{
      incorrect();
      currentposition=0;
     }
}
  }
  else{
    digitalWrite(alarme, LOW);
  }

}

void unlockdoor()
{
delay(900);
 
lcd.setCursor(0,0);
lcd.println(" ");
lcd.setCursor(1,0);
lcd.print("Access Permis");
lcd.setCursor(7,1);
lcd.println("  ");
lcd.setCursor(15,1);
lcd.println(" ");
lcd.setCursor(16,1);
lcd.println(" ");
lcd.setCursor(14,1);
lcd.println(" ");
lcd.setCursor(13,1);
lcd.println(" ");

digitalWrite(alarme,LOW);
delay(1000);
currentposition=0;
 
lcd.clear();
}
 
//--------------------Function 2- Wrong code--------------//
 
void incorrect()
{
delay(500);
lcd.clear();
lcd.setCursor(1,0);
lcd.print("CODE");
lcd.setCursor(6,0);
lcd.print("INCORRECTE");
lcd.setCursor(15,1);
lcd.println(" ");
lcd.setCursor(4,1);
lcd.println("ESSAYER ENCORE !!!");
 
lcd.setCursor(13,1);
lcd.println(" ");
Serial.println("CODE INCORRECTE VOUS N'AVEZ PAS LA PERMISSION");
lcd.clear();
displayscreen();
}
//-------Function 3 - CLEAR THE SCREEN--------------------/
void clearscreen()
{
lcd.setCursor(0,0);
lcd.println(" ");
lcd.setCursor(0,1);
lcd.println(" ");
lcd.setCursor(0,2);
lcd.println(" ");
lcd.setCursor(0,3);
lcd.println(" ");
}

//------------Function 4 - DISPLAY FUNCTION--------------------//
void displayscreen()
{
 
lcd.setCursor(0,0);
lcd.println("*ENTRER LE CODE*");
lcd.setCursor(0 ,1);
 
lcd.println("POUR ARRETER!!");
}

void readFlameSensor() {
  //lire l'etat du capteur
   int digitalVal = digitalRead(flamme);
  if(digitalVal == 1){
    digitalWrite(flamme, HIGH);
    alarme2();
    String message1 = "Flamme dectete"; 
    affichage(message1);
    sendSMS(PHONE,"Flamme detectee....!!!");
  }  
}
void readGasSensor() {
  //lire l'etat du capteur
   int digitalVal = digitalRead(gas);
  if(digitalVal == 1){
    digitalWrite(gas, HIGH);
    alarme2();
    String message1 = "Gaz dectete"; 
    affichage(message1);
    sendSMS(PHONE,"Gaz detecté....!!!");
  }
} 
void cuisine(){ //capteur de flamme et capteur de gaz
    readFlameSensor();
    readGasSensor();
}

void porte(){
  vibrer_porte("Ouverture de la porte forcee");
  ultrason_porte();
  presence();
  }

void fenetre(){
  vibrer_fenetre("Ouverture de la fenetre forcee");
  ultrason_fenetre();
}

void loop() {
  lcd.setCursor(0, 2);
  lcd.print("System in operation");
  fenetre();
  cuisine();
  porte();
}
