#include <Arduino.h>
volatile int state = LOW; // This is the variable that will be changed if an interrupt is triggered
volatile int button1 = LOW; 
volatile int button2 = LOW;
volatile int button3 = LOW;
volatile int button4 = LOW;
const int dtime = 15000;
int inputPin1 = 4; // Polling pin 4
int inputPin2 = 5; // Polling pin 5
int inputPin3 = 6; // Polling pin 6
int inputPin4 = 7; // Polling pin 7
long startTime; // Starting time of the program
long endTime; // Ending time of the program, calculated during the beginning of the void loop
long time; // Time elasped since beginning of program
long duration1 = 5000; // Timer duration of first patient
long duration2 = 11000; // Timer duration of second patient
long duration3 = 7000; // Timer duration of third patient
long duration4 = 19000; // Timer duration of fourth patient
const int ledPin1 = 8; // Input pin of patient 1's LED light
const int ledPin2 = 9; // Input pin of patient 2's LED light
const int ledPin3 = 10; // Input pin of patient 3's LED light
const int ledPin4 = 11; // Input pin of patient 4's LED light
int valinputPin; // Variable used to store polling states
double temp1; // Temperature of patient 1
double temp2; // Temperature of patient 2
double temp3; // Temperature of patient 3
double temp4; // Temperature of patient 4
int thermistor = 13;
const double fevertemp = 28.00;
int enable = LOW;
int read1;
int read2;
int read3;
int read4;

void setup()
{
  attachInterrupt(0, routine, CHANGE); // Attach interrupt
  Serial.begin(9600); // Outputs to serial monitor
  startTime = millis(); // Calculates time at the beginning of program
  pinMode(inputPin1, INPUT);// Sets digital pins connected to buttons as input
  pinMode(inputPin2, INPUT);
  pinMode(inputPin3, INPUT);
  pinMode(inputPin4, INPUT);
  pinMode(ledPin1, OUTPUT); // Sets digital pins connected to LEDs as output
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  digitalWrite(inputPin1, LOW); //intializes all inputpins(buttons) to low > good programming practice
  digitalWrite(inputPin2, LOW);
  digitalWrite(inputPin3, LOW);
  digitalWrite(inputPin4, LOW);
}

void loop()
{

  endTime = millis(); // Calculates time at the exact moment during the program
  time = endTime - startTime; // Calculates total elasped time since the program started
  
  if (time%duration1 == 0) //when time is a multiple of duration that each LED is set to, the condition is TRUE
{
  activate1(); //executes activate function
 // Serial.print(test);
}
  else if (time%duration2 == 0)
{  
  activate2();
}

  else if (time%duration3 == 0)
{
  activate3();
}
  else if (time%duration4 == 0) 
{
  activate4();
}

 
}

void routine() //interrupt handler subroutine
{
  state =! state;
}

void activate1()
{
   valinputPin = digitalRead(inputPin1); 
   while (valinputPin == LOW) //when button is not pressed
  {
   
   tone(12, 800); // Speaker sounds and beeps (alarm)
   digitalWrite(ledPin1, HIGH); // LED blinks (alarm)
   
   if(digitalRead(inputPin1)==HIGH) //when button is pressed
   {
     valinputPin = HIGH; 
   }
   }
   noTone(12); //speaker stops beeping 
   digitalWrite(ledPin1, LOW); //LED stops blinking 
  
  calculate(temp1, duration1, read1); //readings from the thermistor are converted to Celcius 
}

void activate2()
{
   valinputPin = digitalRead(inputPin2);
   while (valinputPin == LOW)
  {
   tone(12, 800); // Speaker sounds and beeps
   digitalWrite(ledPin2, HIGH); // LED blinks
   
   if(digitalRead(inputPin2)==HIGH)
   {
     valinputPin = HIGH;
   }
  }
   noTone(12);
   digitalWrite(ledPin2, LOW);
  
  calculate(temp2, duration2, read2);
}

void activate3()
{
   valinputPin = digitalRead(inputPin3);
   while (valinputPin == LOW)
  {
   tone(12, 800); // Speaker sounds and beeps
   digitalWrite(ledPin3, HIGH); // LED blinks
   
   if(digitalRead(inputPin3)==HIGH)
   {
     valinputPin = HIGH;
   }
   }
   noTone(12);
   digitalWrite(ledPin3, LOW);
  
  calculate(temp3, duration3, read3);
}

void activate4()
{
   valinputPin = digitalRead(inputPin4);
   while (valinputPin == LOW)
   {
   tone(12, 800); // Speaker sounds and beeps
   digitalWrite(ledPin4, HIGH); // LED blinks
   
   if(digitalRead(inputPin4)==HIGH)
   {
     valinputPin = HIGH;
   }
   }
   noTone(12);
   digitalWrite(ledPin4, LOW);
  
   calculate(temp4, duration4, read4);
}

double calculate(double temp, long duration, int read)
{
  digitalWrite (thermistor, HIGH);
	read=analogRead(0);      //fetches reading from thermistor and sends it to pin A1
    temp=Thermistor(read);   //Uses Steinhart-Hart equation to convert the raw analog value to celcius
    Serial.println(temp);   //Displays Celcius temperature on the serial monitor
	 
	 if (temp >= fevertemp) //condition to check if the patient has fever or not
	 {
	   duration = (duration/2.0);
           Serial.println("Body temperature suggests a fever. Timer will be adjusted to take temperature more often.");
	 }
	 else if (temp < fevertemp)
	 {
	   duration = dtime;
           Serial.println("Body temperature is normal.");
	 }
}

double Thermistor(int RawADC) 
{ 
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp ); //Converts analog value to Kelvin
 Temp = Temp - 273.15;              // Converts Kelvin to Celsius
 return Temp; //returns the Celcius value to the calculate function
}
