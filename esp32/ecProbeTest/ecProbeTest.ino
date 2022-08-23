// Software by Pierre Hertzog YouTube channel (AgriTech with Pierre) (2020)
// You are welcome to use my software as long as recognition is given to my YouTube channel
// Connections (Also see Video)
// One pin of EC probe to Arduino Gnd
// One pin of EC probe to 1k resistor and to pin A1 of Arduino
// Other pin of resistor to A0
// One pin of thermistor to A5 and one side of 10k resistor
// Other pin of thermistor to 5V on Arduino
// Other pin of 10k resistor to Gnd

#include <OneWire.h>
#include <DallasTemperature.h>

int R1= 1000; // Value of resistor for EC probe
int EC_Read = 14;
int ECPower = 12;

int Temp_Pin = 13;
OneWire oneWire(Temp_Pin);
DallasTemperature sensors(&oneWire);

float Temp_C; // Do not change
float Temp_F; // Do not change
float Temp1_Value = 0;
float Temp_Coef = 0.019; // You can leave as it is
/////////////////This part needs your attention during calibration only///////////////
float Calibration_PPM =1080 ; //Change to PPM reading measured with a separate meter
float K=1.98; //You must change this constant once for your probe(see video)
float PPM_Con=0.5; //You must change this only if your meter uses a different factor
/////////////////////////////////////////////////////////////////////////////////////
//float CalibrationEC= (Calibration_PPM*2)/1000;
float CalibrationEC = 2.76;
float Temperature;
float EC;
float EC_at_25;
int ppm;
float A_to_D= 0;
float Vin= 3.3;
float Vdrop= 0;
float R_Water;
float Value=0;
//Leave the next 2 lines in if you need help later on///////////////////////////////////
//Ask any questions that you may have in the comment section of this video
//https://youtu.be/-xKIczj9rVA
void setup()
{
  Serial.begin(9600);
  Serial.println("hello");
  sensors.begin();
  pinMode(EC_Read,INPUT);
  pinMode(ECPower,OUTPUT);
  
  
  //Calibrate ();  //After calibration put two forward slashes before this line of code

}
void loop()
{
  GetEC(); 
  delay(6000); //Do not make this less than 6 sec (6000)
}
////////////////////////////////////////////////////////////////////////////////////
void GetEC()
{
  Temp_C = getTemp();
  Temp1_Value = Temp_C;
  Temperature = Temp_C;
  digitalWrite(ECPower,HIGH);
  A_to_D= analogRead(EC_Read);
  A_to_D= analogRead(EC_Read);
  digitalWrite(ECPower,LOW);
  Serial.print("A_to_D: ");
  Serial.println(A_to_D);
  Vdrop= Vin * (A_to_D / 4095.0);

  Serial.print("Vdrop: ");
  Serial.println(Vdrop);
  R_Water = (Vdrop*R1) / (Vin-Vdrop);
  EC = 1000/ (R_Water*K);
  Serial.print("EC at curr temp: ");
  Serial.println(EC);
  EC_at_25 = EC / (1+ Temp_Coef*(Temperature-25.0));
  ppm=(EC_at_25)*(PPM_Con*1000);
  Serial.print(" EC: ");
  Serial.print(EC_at_25);
  Serial.print(" milliSiemens(mS/cm) ");
  Serial.print(ppm);
  Serial.print(" ppm ");
  Serial.print(Temperature);
  Serial.println(" *C ");
}
////////////////////////////////////////////////////////////////////////////////////
void Calibrate ()
{
  Serial.println("Calibration routine started");
  float Temperature_end=0;
  float Temperature_begin=0;
  Temp_C = getTemp();
  Temperature_begin=Temp_C;
  Value = 0;
  int i=1;
  while(i<=10){
    digitalWrite(ECPower,HIGH);
    A_to_D= analogRead(EC_Read);
    A_to_D= analogRead(EC_Read);
    digitalWrite(ECPower,LOW);
    Value=Value+A_to_D;
    i++;
    delay(6000);
  };
  A_to_D=(Value/10);
  Temp_C = getTemp();
  Temp1_Value = Temp_C;
  Temperature_end=Temp_C;
  EC =CalibrationEC*(1+(Temp_Coef*(Temperature_end-25.0)));
  Vdrop= (((Vin)*(A_to_D))/4095.0);
  R_Water=(Vdrop*R1)/(Vin-Vdrop);
  float K_cal= 1000/(R_Water*EC);
  Serial.print("Replace K in line 23 of code with K = ");
  Serial.println(K_cal);
  Serial.print("Temperature difference start to end were = ");
  Temp_C=Temperature_end-Temperature_begin;
  Serial.print(Temp_C);
  Serial.println("*C");
  Serial.println("Temperature difference start to end must be smaller than 0.15*C");
  Serial.println("");
  Calibrate ();
}
int getTemp(){
  sensors.requestTemperatures();
  Temp_C = sensors.getTempCByIndex(0);
  Temp_F = sensors.getTempFByIndex(0);
  return Temp_C;
}
