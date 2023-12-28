void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.println("Comunicação Serial a todo vapor!");
}

void loop() {
  // put your main code here, to run repeatedly: 
  if (Serial.available() > 0)  {
    Serial.println((char)Serial.read());
  }}
