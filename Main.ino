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
//  Serial.begin(9600);
}


// function to read a distance from a HC-SR04
int SonarSensor(int trigPin,int echoPin){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    delay(100);
    return distance;
}


// Makes to power over the H Bridge go forwards
void vooruit(){
    digitalWrite(motorRd, LOW);
    analogWrite(motorRs, 220);
    digitalWrite(motorLd, LOW);
    analogWrite(motorLs, 250);

}


// Makes to power over the H Bridge go backwards
void achterruit(){
    digitalWrite(motorRd, HIGH);
    analogWrite(motorRs, 35);
    digitalWrite(motorLd, HIGH);
    analogWrite(motorLs, 5);

}

// Makes the moters go stop
void stopgoing(){
    digitalWrite(motorRd, HIGH);
    analogWrite(motorRs, 255);
    digitalWrite(motorLd, HIGH);
    analogWrite(motorLs, 255);

}

// Returns a 'r' or 'l' depending on which side there is the most room
char whatDirection(){
    RightSensor = SonarSensor(trigPinR, echoPinR);
    LeftSensor = SonarSensor(trigPinL, echoPinL);
    if(LeftSensor > RightSensor){
        return 'l';
    }

    if(LeftSensor < RightSensor){
        return 'r';
    }

    if(LeftSensor == RightSensor){
        return 'r';
    }

}


// Turns 1 moter backwards and the other one forwards based on what direction it is safe to go towards
void turn(int ms){
    //  Gets the safest direction to turn to from the function whatDirection()
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

// Angles the robot towards the heat source
void angel(char direction){
    int ms = 150;
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

// Checks if the heat source is infront of the robot and if it is close to the source
int infrontFire(){
    int readingLeft = analogRead(sonsorLeftLeft);
    int readingMiddleleft = analogRead(sonsorMiddleLeft);
    int readingMiddle = analogRead(sonsorCenter);
    int readingMiddleright = analogRead(sonsorMiddleRight);
    int readingRight = analogRead(sonsorRightRight);

    // if it cannot find a heat source
    if ((readingLeft < 200) and (readingMiddleleft < 200) and (readingRight < 200) and (readingMiddleright < 200 ) and (readingMiddle < 200)){
        return -1;
    }

        // if the middle is not the highest reading
    else if ((readingLeft > readingMiddle) or (readingMiddleleft > readingMiddle) or (readingRight > readingMiddle) or (readingMiddleright > readingMiddle)){
        return 0;
    }

        // the middle is the highest reading
    else {
        // the heat source is nearby
        if (readingMiddle > 700){
            return 2;
        }
            // heat source is not nearby
        else {
            return 1;
        }
    }
}

// Fan goes on for 8 seconds
void bigFan(){
    digitalWrite(fan, HIGH);
    delay(8000);
    digitalWrite(fan, LOW);

}

// decides which way the car needs to angle and how much
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

    // if all is good car can drive
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

}