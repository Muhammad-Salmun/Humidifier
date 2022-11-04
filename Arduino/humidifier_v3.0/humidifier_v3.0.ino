#include <OneWire.h> 
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2
#define Coil 11
#define Valve 12
#define water_lvl_low A0
#define water_lvl_high A2

// setting up instancefor communication and referencing oneWire
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// intitialising global variables
int room_temp = 0, water_lvl = 0,
water_lvl_snsr_threshold = 400, min_temp = 30, max_temp = 45;


void setup()
{
  pinMode(Valve, OUTPUT);
  pinMode(Coil, OUTPUT);
  pinMode(water_lvl_low, INPUT);
  pinMode(water_lvl_high, INPUT);
  Serial.begin(9600);

  //turns on valve in the begining
  digitalWrite(Valve, LOW);
  digitalWrite(Coil, HIGH);
}

void loop()
{
  vavleControl();
  room_temp = getRoomTemp();
  coilControl();
  delay(500);
}

//////////////////////////////////// void vavleAndCoilControl ////////////////////////
void vavleControl()
{
  // if chamber is filled
  if(WaterPresent(water_lvl_high))  digitalWrite(Valve,HIGH); //stops water
  // if chamber is empty
  else if(!WaterPresent(water_lvl_low)) digitalWrite(Valve,LOW);  // starts water
}

void coilControl(){
  // if room is hot
  if(room_temp > max_temp) digitalWrite(Coil,HIGH); // turns off coil
  // if room is cold
  else if(room_temp < min_temp)digitalWrite(Coil,LOW); // turns on coil
}

// function to read temprature sensor value from ds18b20
int getRoomTemp(){
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.print("room temp: ");
  Serial.println(sensors.getTempCByIndex(0));
  return sensors.getTempCByIndex(0);
}

///////////////////////////////////////// bool WaterLvl ///////////////////////////////
// function that returns true if sensor pin is detects water
bool WaterPresent(int pin)
{
  int val = analogRead(pin);
  Serial.println(val);
  if(val > water_lvl_snsr_threshold) return  true;
  else return false;
}
