void setup() {
  // put your setup code here, to run once:
  pinMode(13,INPUT);
  pinMode(25,OUTPUT); 
  Serial.begin(9600);
  Serial.println("beginning");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(13));
  delay(250);
  
}
