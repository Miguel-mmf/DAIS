#define PIN_RS485_RX  PA_6
#define PIN_RS485_TX  PA_7
#define controle      PK_4

char end_no = '1';
String defaut_msg[3] = {
  "Mensagem RX: ",
  "Mensagem TX: ",
  "Meu Endereco = "
};


void setup()
{
  Serial.begin (115200);
  delay (1000);

  // Serial2.begin (115200, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);
  Serial2.begin (115200);
  delay (1000);

  Serial.println("RS485-Test");
  delay (1000);

  pinMode(controle, OUTPUT);
  digitalWrite(controle, LOW);
}

void loop()
{

  if (Serial2.available())
  {
    Serial.print(defaut_msg[0]);
    String msg2 = Serial2.readStringUntil('\n');
    Serial.println(msg2);
    
    if (msg2[0] == end_no)
    {
      delay(1000); // necessário para a correta recepção da mensagem
      digitalWrite(controle, HIGH);     // Habilita
      Serial.print(defaut_msg[2]); // Imprime a mensagem no Serial Monitor
      Serial.println(end_no); // Imprime a mensagem no Serial Monitor
      Serial2.print(defaut_msg[2]); // Envia a mensagem para o outro nó
      Serial2.println(end_no); // Envia a mensagem para o outro nó
      delay(1000); // necessário para o envio correto da mensagem

      digitalWrite(controle, LOW);      // Desabilita Novamente
    }  
  }

  if (Serial.available())
  {
    Serial.print(defaut_msg[1]);
    String msg = Serial.readStringUntil('\n');
    Serial.println (msg); // Imprime a mensagem no Serial Monitor
    Serial2.print(msg); // Envia a mensagem para o outro nó

    delay(1000);
    digitalWrite(controle, LOW);

    if (msg[1] == 'z' || msg[0] == 'Z')
      digitalWrite(controle, LOW);
    else if (msg[1] == 'x' || msg[0] == 'X')
      digitalWrite(controle, HIGH);
  }

  delay (100);
}