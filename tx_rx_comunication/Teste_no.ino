// Este arquivo simula um novo nó para a comunicação RS485

#define PIN_RS485_RX PA_6
#define PIN_RS485_TX PA_7
#define controle PK_4

char no = '2';

void setup()
{
  Serial.begin (115200);
  delay (1000);

  // Serial2.begin (115200, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);
  Serial2.begin (115200);
  delay (1000);

  Serial.println ("RS485-Test");
  delay (1000);

  pinMode(controle, OUTPUT);
  digitalWrite(controle, LOW);
  
}

void loop()
{
  if (Serial2.available())
  {
    Serial.print ("Mensagem RX: ");
    String msg_r = Serial2.readStringUntil ('\n');
    Serial.println (msg_r);

    if (msg_r[0] == no){
      delay(1000);
      digitalWrite(controle, HIGH);
      Serial.println ("Ola 1");
      Serial2.println ("Ola 1");
      delay(1000);
      digitalWrite(controle, LOW);
    }

  }

  if (Serial.available())
  {
    Serial.print ("Mensagem TX: ");
    String msg = Serial.readStringUntil ('\n');
    Serial.println (msg);
    Serial2.print(msg);

    delay(1000);
    digitalWrite(controle, LOW);

    if (msg[0] == 'Z' || msg[0] == 'z')
    { 
      digitalWrite(controle, LOW);
    }
    else if (msg[0] == 'X' || msg[0] == 'x')
    {
      digitalWrite(controle, HIGH);
    }
  }
}
