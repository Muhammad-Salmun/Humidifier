#include <OneWire.h> 
#include <DallasTemperature.h>

// pins connected on the Arduino 
#define ONE_WIRE_BUS 2
#define Buzzer 3
#define Coil 11
#define Valve 12
#define water_lvl_low A0
#define water_lvl_high A2

// setting up instancefor communication and referencing oneWire
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// intitialising global variables
int   water_lvl = 0, water_lvl_snsr_threshold = 400, 
      room_temp = 0, min_temp = 30, max_temp = 45;

long  session_start_time=0, 
      session_time=1800,  // time in sec 
      fill_start_time=0;

bool  coil_on = false, chamber_empty = false;

/////////////////////////////////void setup ///////////////////////////////
void setup()
{
  pinMode(Valve, OUTPUT);
  pinMode(Coil, OUTPUT);
  pinMode(water_lvl_low, INPUT);
  pinMode(water_lvl_high, INPUT);
  Serial.begin(9600);

  //turns off all relays in the begining
  digitalWrite(Valve, HIGH);
  digitalWrite(Coil, HIGH);

  session_start_time = millis();
}

////////////////////////////////////// void loop ////////////////////////////
void loop()
{
  if((millis() - session_start_time) < (session_time*1000))
  {
    room_temp = getRoomTemp(); //read temp
    if(room_temp > max_temp)  // is room hot
    {
      digitalWrite(Coil,HIGH); //turn OFF coil
      coil_on = false;
    }
    else if(coil_on) fillChamber();
    else if(room_temp < min_temp)  fillChamber();
  }
  else  endSession();
}

////////////////////////////////////// void fillChamber ///////////////////////
// fill water if chamber is empty
void fillChamber()
{
  // if chamber is empty
  if(!WaterPresent(water_lvl_low))
  {
    chamber_empty = true;
    fill_start_time = millis();
    digitalWrite(Valve,LOW);  // starts water
  }
  // starts filling the chamber until full
  while(chamber_empty)
  {
    if((millis() - fill_start_time) > 1000 && coil_on)
    {
      digitalWrite(Coil,HIGH); //turn OFF coil
      coil_on = false;
    }
    if(WaterPresent(water_lvl_high))
    {
      digitalWrite(Valve,HIGH); //stops water
      chamber_empty = false;
    }
  }
  digitalWrite(Coil,LOW); //turn ON coil
  coil_on = true;
}

//////////////////////////////////////// void endSession //////////////////////////////
void endSession()
{
  digitalWrite(Valve, HIGH);
  digitalWrite(Coil, HIGH);
  //buzzer
  for(int i=0;i<10;i++)
  {
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW); 
    delay(500);
  }
}

//////////////////////////////////////////////////// int getRoomTemp //////////////
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
