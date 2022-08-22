int waterLevelPin = 35;
int relayPin = 33;
int go = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(waterLevelPin, INPUT);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int waterVal = analogRead(waterLevelPin);
  Serial.println(waterVal);
  if(waterVal < 1700){
    digitalWrite(relayPin,HIGH); 
  }else{
    digitalWrite(relayPin, LOW);
  }
  delay(50);
}
