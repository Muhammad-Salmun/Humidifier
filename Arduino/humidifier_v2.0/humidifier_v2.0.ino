#define Buzzer 5
#define Coil 11
#define Valve 12
#define water_lvl_low A0
#define water_lvl_high A2

int 
counter = 0,
water_lvl_snsr_threshold = 400,
time_sec = 300;

//////////////////////////////////////// void setup ///////////////////////////////////
void setup()
{
  pinMode(Valve, OUTPUT);
  pinMode(Coil, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(water_lvl_low, INPUT);
  pinMode(water_lvl_high, INPUT);
  Serial.begin(9600);

  //turns on valve in the begining
  digitalWrite(Valve, LOW);
  digitalWrite(Coil, HIGH);
}

//////////////////////////////////////// void loop /////////////////////////////////////
void loop()
{
  if(counter < (2*time_sec)){
    vavleAndCoilControl();
    counter ++;
  }
  else endSession();  
}

//////////////////////////////////// void vavleAndCoilControl ////////////////////////
void vavleAndCoilControl()
{
  // checks if chamber is filled
  if(WaterPresent(water_lvl_high)){
    digitalWrite(Valve,HIGH);
    delay(500);
    digitalWrite(Coil,LOW);
  }
  // checks if chamber is empty
  else if(!WaterPresent(water_lvl_low)){
      digitalWrite(Valve,LOW);
      delay(500);
      digitalWrite(Coil,HIGH);
  }
  delay(500);  
}

//////////////////////////////////////// void endSession //////////////////////////////
void endSession()
{
  digitalWrite(Valve, HIGH);
  digitalWrite(Coil, HIGH);
  //buzzer
  while(1){
    digitalWrite(Buzzer, HIGH);
    delay(200);
    digitalWrite(Buzzer, LOW); 
    delay(200);
  }
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
