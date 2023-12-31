//IR sensor
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;
 
void isr() //interrupt service routine
{
rev++;
}
 

// Define the pin connected to the IR sensor output
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows


const int trigPin = 3;
const int echoPin = 4;
const int ledPin = 8;
const int ledPin2 = 9;
const int ledPin3 = 10;
const int ledPin4 = 11;
// defines variables

int distance;
int safetyDistance;
unsigned long duration;
//voltage sensor
#define ANALOG_IN_PIN A0
 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value = 0; 
////////////////////////////////
// current sensor 
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;



float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float power_mW = 0;


void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  attachInterrupt(0,isr,RISING);
  
  // Attach the interrupt to the sensor pin
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

pinMode(ledPin, OUTPUT);
 // Starts the serial communication
 
  lcd.init(); // initialize the lcd
  lcd.backlight();

   ina219.begin(); // current
}

void loop() {
  // Calculate the time elapsed since the previous blade detection
  delay(1000);
detachInterrupt(0); //detaches the interrupt
time=millis()-oldtime; //finds the time
rpm=(rev/time)*60000/3; //calculates rpm for blades
oldtime=millis(); //saves the current time
rev=0;
attachInterrupt(0,isr,RISING);
Serial.print("rpm: ");
Serial.println(rpm);



   // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;

safetyDistance = distance;
if (safetyDistance <= 10){
 
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, HIGH);
}
else if (safetyDistance > 10 && safetyDistance <= 20 )
{
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, LOW);
}
else if (safetyDistance > 20 && safetyDistance <= 30 )
{
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin4, LOW);
}
else if (safetyDistance > 30 && safetyDistance <= 44 )
{
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
} else {
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin4, LOW);
}
// Prints the distance on the Serial Monitor



  Serial.print("Distance: ");
Serial.println(distance);
  //////////////////////////////////////////////////////////////////////////////////// LCD Display

                // clear display
   // print message at (2, 1)
                   // display the above for two seconds
// voltage sensor
 // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);
  
  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 1024.0;
  
  // Calculate voltage at divider input
  in_voltage = adc_voltage*(R1+R2)/R2;
  
  // Print results to Serial Monitor to 2 decimal places
  Serial.print("Input Voltage = ");
  Serial.println( in_voltage ,2);
  //////////////////////////////
  // current sensor //////
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
 Serial.print("shunt Voltage:   "); 
  Serial.print(shuntvoltage); 
  Serial.println(" V");  

 Serial.print("Bus Voltage:   "); 
  Serial.print(busvoltage); 
  Serial.println(" V");  
  
  Serial.print("Current:       "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  
  Serial.print("Power:         "); 
  Serial.print(power_mW); 
  Serial.println(" mW");  
  
  Serial.println(""); 

lcd.clear();
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("W=");        // print message at (0, 0)
                            // lcd.setCursor(0, 5);         // move cursor to   (2, 1)
  lcd.print(rpm);
   lcd.print("   V=");
   lcd.print(in_voltage);
  
    lcd.setCursor(0, 1);
    lcd.print("I=");
    lcd.print(current_mA);
    lcd.print(" P=");
    lcd.print(power_mW);

    Serial.println(vol);
    Serial.println(curr);
    Serial.println(pwr);
  delay(500); 

  
}

