#define sensor PE_0
int valorD;

void setup() {
  Serial.begin(19200);
}

void loop() {

  valorD = analogRead(sensor);
  
  Serial.print("Valor lido= ");
  Serial.print(valorD);
  
  Serial.print(" | Valor em tensao = ");
  Serial.println(map(valorD,0,4093,0,3.3));
  delay(500);
}
