#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1  2

void setup()
{
  // Set pin as output.
  pinMode(RELAY_1, OUTPUT);  
  // Initialize relay one as off so that on reset it would be off by default
  digitalWrite(RELAY_1, RELAY_OFF);
}

void loop()
{
  // Turn on and wait 10 seconds.
  digitalWrite(RELAY_1, RELAY_ON);
  delay(3000);

  // Turn off and wait 10 seconds.
  digitalWrite(RELAY_1, RELAY_OFF);
  delay(3000);
}


