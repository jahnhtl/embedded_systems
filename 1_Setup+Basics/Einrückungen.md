# Übungsblatt: Einrückungen von C-Code

## Ziel
In dieser Übung lernst du, wie man C-Code **richtig einrückt** und formatiert.  
Eine saubere Formatierung ist **nicht nur schöner**, sondern macht Programme **leichter verständlich, einfacher zu debuggen** und **professioneller**.

---

## Regeln für das Einrücken

### Grundregeln
1. **Jeder neue Block** (gekennzeichnet durch `{` und `}`) wird **eine Ebene eingerückt**.  
   → z. B. mit **4 Leerzeichen** oder **1 Tabulator**.
2. **Klammern gehören in eigene Zeilen oder klar erkennbar hinter Anweisungen.**
3. Innerhalb von **`if`, `for`, `while`, `setup()`, `loop()`** usw. sind die **Anweisungen eingerückt**.
4. Zwischen logischen Codeabschnitten (z. B. Setup und Loop) **eine Leerzeile** lassen.
5. **Kommentare** können zur Orientierung beitragen.

Beispiel:
```c
void loop() {
    if (digitalRead(2) == HIGH) {
        digitalWrite(13, HIGH);
    } else {
        digitalWrite(13, LOW);
    }
}
```

---

## Übungsaufgaben

Kopiere jede der folgenden Aufgaben **in eine neue C- oder Arduino-Datei (.ino)**  
und **rücke den Code sauber ein**.

---

### Aufgabe 1

```c
int potPin=A0;void setup(){Serial.begin(9600);}void loop(){int value=analogRead(potPin);if(value>512){Serial.println("Wert ist hoch!");}else{Serial.println("Wert ist niedrig!");}delay(500);}
```

---

### Aufgabe 2

```c
int ledPins[]={3,5,6};void setup(){for(int i=0;i<3;i++){pinMode(ledPins[i],OUTPUT);}}void loop(){for(int i=0;i<3;i++){digitalWrite(ledPins[i],HIGH);delay(200);if(i==1){delay(400);}digitalWrite(ledPins[i],LOW);}delay(500);}
```

---

### Aufgabe 3

```c
#include <DHT.h> #define DHTPIN 7 #define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);int ledCold=3;int ledOk=4;int ledHot=5;void setup(){Serial.begin(9600);dht.begin();pinMode(ledCold,OUTPUT);pinMode(ledOk,OUTPUT);pinMode(ledHot,OUTPUT);}void loop(){float t=dht.readTemperature();if(isnan(t)){Serial.println("Fehler beim Lesen des Sensors!");return;}if(t<20){digitalWrite(ledCold,HIGH);digitalWrite(ledOk,LOW);digitalWrite(ledHot,LOW);}else if(t<27){digitalWrite(ledCold,LOW);digitalWrite(ledOk,HIGH);digitalWrite(ledHot,LOW);}else{digitalWrite(ledCold,LOW);digitalWrite(ledOk,LOW);digitalWrite(ledHot,HIGH);}Serial.print("Temperatur: ");Serial.print(t);Serial.println(" °C");delay(1000);}
```

---

### Aufgabe 4

```c
int tempPin=A0;int lightPin=A1;int led=9;void setup(){pinMode(led,OUTPUT);Serial.begin(9600);}void loop(){float temp=readTemp();int light=readLight();controlLed(temp,light);printStatus(temp,light);delay(1000);}float readTemp(){int val=analogRead(tempPin);float voltage=val*5.0/1023;return (voltage-0.5)*100;}int readLight(){return analogRead(lightPin);}void controlLed(float temp,int light){if(temp>25&&light<300){digitalWrite(led,HIGH);}else{digitalWrite(led,LOW);}}void printStatus(float temp,int light){Serial.print("Temperatur: ");Serial.print(temp);Serial.print(" °C, Licht: ");Serial.println(light);}
```

---

### Aufgabe 5 - Mo-Mo-Mo-Mo-Monsterbeispiel

```c
#include <Servo.h> #include <DHT.h> #define DHTPIN 7 #define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE)
Servo servo1;Servo servo2;int ledPins[]={2,3,4,5,6,8,9,10};int buttonPins[]={A0,A1,A2};int potPins[]={A3,A4};int tempSensor=A5;int lightSensor=A6;int buzzer=11;int servo1Pin=12;int servo2Pin=13;int numLeds=8;int numButtons=3;int numPots=2;int lastButtonState[]={0,0,0};int ledState[]={0,0,0,0,0,0,0,0};unsigned long lastDebounceTime[]={0,0,0};unsigned long debounceDelay=50;float temp=0;int lightLevel=0;void setup(){Serial.begin(9600);dht.begin();servo1.attach(servo1Pin);servo2.attach(servo2Pin);pinMode(buzzer,OUTPUT);for(int i=0;i<numLeds;i++){pinMode(ledPins[i],OUTPUT);}for(int i=0;i<numButtons;i++){pinMode(buttonPins[i],INPUT_PULLUP);}randomSeed(analogRead(0))}void loop(){readSensors();handleButtons();updateLeds();controlServos();playBuzzer();displayStatus();delay(100)}void readSensors(){temp=dht.readTemperature();if(isnan(temp)){temp=-1}lightLevel=analogRead(lightSensor)}void handleButtons(){for(int i=0;i<numButtons;i++){int reading=digitalRead(buttonPins[i]);if(reading!=lastButtonState[i]){lastDebounceTime[i]=millis()}if((millis()-lastDebounceTime[i])>debounceDelay){if(reading!=ledState[i]){ledState[i]=reading;performButtonAction(i)}}lastButtonState[i]=reading}}void performButtonAction(int btn){if(btn==0){blinkAll(3,100)}else if(btn==1){chaseForward(100)}else if(btn==2){chaseBackward(100)}}void updateLeds(){for(int i=0;i<numLeds;i++){if(ledState[i]==LOW){digitalWrite(ledPins[i],HIGH)}else{digitalWrite(ledPins[i],LOW)}}}void controlServos(){int potVal1=analogRead(potPins[0]);int potVal2=analogRead(potPins[1]);int angle1=map(potVal1,0,1023,0,180);int angle2=map(potVal2,0,1023,0,180);servo1.write(angle1);servo2.write(angle2)}void playBuzzer(){if(temp>30||lightLevel<200){tone(buzzer,1000,200)}else{noTone(buzzer)}}void displayStatus(){Serial.print("Temperatur: ");Serial.print(temp);Serial.print(" °C, Licht: ");Serial.print(lightLevel);Serial.print(", Servos: ");Serial.print(analogRead(potPins[0]));Serial.print(",");Serial.println(analogRead(potPins[1]))}void blinkAll(int times,int delayTime){for(int i=0;i<times;i++){for(int j=0;j<numLeds;j++){digitalWrite(ledPins[j],HIGH)}delay(delayTime);for(int j=0;j<numLeds;j++){digitalWrite(ledPins[j],LOW)}delay(delayTime)}}void chaseForward(int delayTime){for(int i=0;i<numLeds;i++){digitalWrite(ledPins[i],HIGH);delay(delayTime);digitalWrite(ledPins[i],LOW)}}void chaseBackward(int delayTime){for(int i=numLeds-1;i>=0;i--){digitalWrite(ledPins[i],HIGH);delay(delayTime);digitalWrite(ledPins[i],LOW)}}void complexPattern(){for(int i=0;i<5;i++){chaseForward(50);blinkAll(2,100);chaseBackward(50)}for(int i=0;i<numLeds;i++){if(i%2==0){digitalWrite(ledPins[i],HIGH)}else{digitalWrite(ledPins[i],LOW)}}delay(200);for(int i=0;i<numLeds;i++){digitalWrite(ledPins[i],LOW)}}void safetyCheck(){if(temp>40){for(int i=0;i<numLeds;i++){digitalWrite(ledPins[i],HIGH)}tone(buzzer,2000,500);delay(500);for(int i=0;i<numLeds;i++){digitalWrite(ledPins[i],LOW)}}if(lightLevel<100){blinkAll(5,50)}}void combinedActions(){complexPattern();safetyCheck();controlServos();playBuzzer()}void loop2(){combinedActions();displayStatus();delay(100)}
