#include <C:\Users\miguel\AppData\Local\Energia15\packages\energia\hardware\tivac\1.0.4\cores\tivac\wiring_analog.c>
#define bomba1 PL_4
#define bomba2 PL_5

int valorSerial = 0;
int duty = 122;

void setup()
{

  Serial.begin(115200);
  pinMode(bomba1, OUTPUT);
  pinMode(bomba2, OUTPUT);
  
  // usando a função PWMWrite
  PWMWrite(bomba2, 4096, 2048, 1000);

  // usando o analogWrite
  analogWrite(bomba1, duty);
  /*
  void analogWrite(uint8_t pin, int val) {
    //
    //  duty cycle(%) = val / 255;
    //  Frequency of 490Hz specified by Arduino API
    //
    PWMWrite(pin, 255, val, 490);
  }
  */
}

void loop()
{

  if (Serial.available() > 0)
  {
    valorSerial = Serial.read();
    Serial.print("Valor recebido: ");
    Serial.println(valorSerial);

    analogWrite(bomba1, valorSerial);
  }
}
