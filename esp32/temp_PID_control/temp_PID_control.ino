//Inputs and outputs
int firing_pin = 3;
int zero_cross = 8;

//Variables
int last_CH1_state = 0;
bool zero_cross_detected = false;
int firing_delay = 7400;

//////////////////////////////////////////////////////
int maximum_firing_delay = 7400;
/*Later in the code you will se that the maximum delay after the zero detection
 * is 7400. Why? Well, we know that the 220V AC voltage has a frequency of around 50-60HZ so
 * the period is between 20ms and 16ms, depending on the country. We control the firing
 * delay each half period so each 10ms or 8 ms. To amke sure we wont pass thsoe 10ms, I've made tests
 * and the 7400us or 7.4ms was a good value. Measure your frequency and chande that value later */
//////////////////////////////////////////////////////

unsigned long previousMillis = 0; 
unsigned long currentMillis = 0;
int real_temperature = 0;
int setpoint = 31;

//PID variables
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
int PID_value = 0;
//PID constants
int kp = 203;   int ki= 7.2;   int kd = 1.04;
int PID_p = 0;    int PID_i = 0;    int PID_d = 0;






void setup() {
  //Define the pins
  pinMode (firing_pin,OUTPUT); 
  pinMode (zero_cross,INPUT); 
  pinMode (increase_pin,INPUT); 
  pinMode (decrease_pin,INPUT);   
}


void loop() {    
  currentMillis = millis();           //Save the value of time before the loop
   /*  We create this if so we will read the temperature and change values each "temp_read_Delay"
    *  value. Change that value above iv you want. The MAX6675 read is slow. Tha will affect the
    *  PID control. I've tried reading the temp each 100ms but it didn't work. With 500ms worked ok.*/
  if(currentMillis - previousMillis >= temp_read_Delay){
    previousMillis += temp_read_Delay;              //Increase the previous time for next loop
    real_temperature = thermocouple.readCelsius();  //get the real temperature in Celsius degrees

    PID_error = setpoint - real_temperature;        //Calculate the pid ERROR
    
    if(PID_error > 30) PID_i = 0; //integral constant will only affect errors below 30ºC             
    
    PID_p = kp * PID_error;                         //Calculate the P value
    PID_i = PID_i + (ki * PID_error);               //Calculate the I value
    timePrev = Time;                    // the previous time is stored before the actual time read
    Time = millis();                    // actual time read
    elapsedTime = (Time - timePrev) / 1000;   
    PID_d = kd*((PID_error - previous_error)/elapsedTime);  //Calculate the D value
    PID_value = PID_p + PID_i + PID_d;                      //Calculate total PID value

    //We define firing delay range between 0 and 7400. Read above why 7400!!!!!!!
    if(PID_value < 0){
      PID_value = 0;
    }
    if(PID_value > 7400){
      PID_value = 7400;
    }
 
    previous_error = PID_error; //Remember to store the previous error.
  }

  //If the zero cross interruption was detected we create the 100us firing pulse  
  if (zero_cross_detected)     
    {
      delayMicroseconds(maximum_firing_delay - PID_value); //This delay controls the power
      digitalWrite(firing_pin,HIGH);
      delayMicroseconds(100);
      digitalWrite(firing_pin,LOW);
      zero_cross_detected = false;
    } 
}

//This is the interruption routine (pind D8(zero cross), D11(increase) and D12(decrease))
//----------------------------------------------
void IRAM_ATTR zeroCross(){
  Serial.println("cross");
}
