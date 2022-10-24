#define Coil 11
#define Valve 12
#define water_lvl_low A0
#define water_lvl_high A2

int temp_val = 0, water_lvl = 0,
water_lvl_snsr_threshold = 400;

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
  // checks if chamber is filled
  if(WaterLvl(water_lvl_high)){
    digitalWrite(Valve,HIGH);
    delay(500);
    digitalWrite(Coil,LOW);
  }
  // checks if chamber is empty
  else if(!WaterLvl(water_lvl_low)){
      digitalWrite(Valve,LOW);
      delay(500);
      digitalWrite(Coil,HIGH);
  }
  delay(500);
}

// function that returns true if sensor pin is detects water
bool WaterLvl(int pin)
{
  int val = analogRead(pin);
  Serial.println(val);
  if(val > water_lvl_snsr_threshold) return  true;
  else return false;
}
