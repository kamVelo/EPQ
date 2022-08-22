int readPin = 13;
void setup() {
  // put your setup code here, to run once:
  pinMode(readPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(readPin));
  delay(1000);
}
