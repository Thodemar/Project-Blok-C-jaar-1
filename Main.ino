const int motorRd = 2;
const int motorRs = 3;
const int motorLd = 4;
const int motorLs = 5;
#define trigPinR 6
#define echoPinR 7
#define trigPinL 8
#define echoPinL 9
#define trigPinF 10
#define echoPinF 11
#define fan 12
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
const int sonsorLeftLeft = 14; // a0
const int sonsorMiddleLeft = 15; // a1
const int sonsorCenter = 16; // a2
const int sonsorMiddleRight = 17; // a3
const int sonsorRightRight = 18; // a4



long duration, distance, RightSensor,BackSensor,FrontSensor,LeftSensor;


void setup() {
  // put your setup code here, to run once:
  pinMode(motorLd, OUTPUT);
  pinMode(motorRd, OUTPUT);
  pinMode(motorLs, OUTPUT);
  pinMode(motorRs, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(fan, OUTPUT);
  Serial.begin(9600);
}



int SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
//Serial.print(distance);
//Serial.print("\n");
delay(100);
return distance;
}



void vooruit(){
  digitalWrite(motorRd, LOW);
  analogWrite(motorRs, 220);
  digitalWrite(motorLd, LOW);
  analogWrite(motorLs, 250);

}



void achterruit(){
  digitalWrite(motorRd, HIGH);
  analogWrite(motorRs, 35);
  digitalWrite(motorLd, HIGH);
  analogWrite(motorLs, 5);

}


void stopgoing(){
  digitalWrite(motorRd, HIGH);
  analogWrite(motorRs, 255);
  digitalWrite(motorLd, HIGH);
  analogWrite(motorLs, 255);

  }


char whatDirection(){
  RightSensor = SonarSensor(trigPinR, echoPinR);
  LeftSensor = SonarSensor(trigPinL, echoPinL);
  if(LeftSensor > RightSensor){
    return 'l';
//    Serial.print("hoi\n");

  }
  if(LeftSensor < RightSensor){
    return 'r';
//      Serial.print("hoi\n");

  }
   if(LeftSensor == RightSensor){
    return 'r';
//    Serial.print("hoi\n");

  }
  
}
   


void turn(int ms){
  char direction = whatDirection();
  delay(100);


  
  if(direction == 'l'){
    digitalWrite(motorRd, LOW);
    analogWrite(motorRs, 220);
    digitalWrite(motorLd, HIGH);
    analogWrite(motorLs, 5);
    delay(ms);
    stopgoing();  
  }
  if(direction == 'r'){
    digitalWrite(motorRd, HIGH);
    analogWrite(motorRs, 35);
    digitalWrite(motorLd, LOW);
    analogWrite(motorLs, 250);
    delay(ms);
    stopgoing();
  }
}


void angel(char direction){
  int ms = 150;
//  char direction = 'r';
  delay(100);


  
  if(direction == 'l'){
    digitalWrite(motorRd, LOW);
    analogWrite(motorRs, 220);
    digitalWrite(motorLd, HIGH);
    analogWrite(motorLs, 5);
    delay(ms);
    stopgoing();  
  }
  if(direction == 'r'){
    digitalWrite(motorRd, HIGH);
    analogWrite(motorRs, 35);
    digitalWrite(motorLd, LOW);
    analogWrite(motorLs, 250);
    delay(ms);
    stopgoing();
  }
}


int infrontFire(){
  int readingLeft = analogRead(sonsorLeftLeft);
  int readingMiddleleft = analogRead(sonsorMiddleLeft);
  int readingMiddle = analogRead(sonsorCenter);
  int readingMiddleright = analogRead(sonsorMiddleRight);
  int readingRight = analogRead(sonsorRightRight);
  if ((readingLeft < 200) and (readingMiddleleft < 200) and (readingRight < 200) and (readingMiddleright < 200 ) and (readingMiddle < 200)){
//    Serial.print("Nee\n");
    return -1;
  }
  else if ((readingLeft > readingMiddle) or (readingMiddleleft > readingMiddle) or (readingRight > readingMiddle) or (readingMiddleright > readingMiddle)){
//    Serial.print("Nee\n");
    return 0;
  } else {
    Serial.print(readingMiddle);
    Serial.print("\n");
    if (readingMiddle > 700){
      return 2;
    } else {
//      Serial.print("Ja\n");
      return 1;
    }
  }
}


void bigFan(){
  digitalWrite(fan, HIGH);
  delay(8000);
  digitalWrite(fan, LOW);

}

void turnToFlame(){
  int readingLeft = analogRead(sonsorLeftLeft);
  int readingMiddleleft = analogRead(sonsorMiddleLeft);
  int readingMiddle = analogRead(sonsorCenter);
  int readingMiddleright = analogRead(sonsorMiddleRight);
  int readingRight = analogRead(sonsorRightRight);
  
  if (readingLeft > readingRight){
    if (readingLeft > readingMiddleleft){
      angel('l');
      angel('l');
    }
    else if (readingLeft < readingMiddleleft){
      angel('l');
    }
  }

  else if (readingRight > readingLeft){
    if (readingRight > readingMiddleright){
      angel('r');
      angel('r');
    }
    else if (readingRight < readingMiddleright){
      angel('r');
    }
  }
}

void loop() {
// put your main code here, to run repeatedly:
//  vooruit();
//  delay(1000);
//  stopgoing();
//  delay(1000);
//SonarSensor(trigPin1, echoPin1);
//RightSensor = distance;
//SonarSensor(trigPin2, echoPin2);
//LeftSensor = distance;
//SonarSensor(trigPinF, echoPinF);
//FrontSensor = distance;

//turn('r', 4);
//delay(1000);





while (((infrontFire() == 1) or (infrontFire() == -1 ) or (infrontFire() == 2)) and (SonarSensor(trigPinF, echoPinF) > 20) ){
    vooruit();
}

stopgoing();

// Is near flame so trigger fan
if((SonarSensor(trigPinF, echoPinF) <= 25) and (infrontFire() == 2)){
  bigFan();
}

// is not going straight at the Flane
else if(infrontFire() == 0){
  turnToFlame();
  
}

// Is close to a object so need to turn otherwise it would hit the object
else if(SonarSensor(trigPinF, echoPinF) <= 20 ){
  achterruit();
  delay(500);
  turn(400);
  stopgoing();
  delay(100);
} 



//angel();
//delay (200);

//if(SonarSensor(trigPinF, echoPinF) <= 20){
//  stopgoing();
//  delay(100);
//  turn(4);
//  delay(1000);
//  Serial.print("hoi\n");
//
//  }else{
//  vooruit();
//  
//  }
//delay(60);
//Serial.print("hoi\n");

}
