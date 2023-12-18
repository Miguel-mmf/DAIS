// Apoio
  // Tabela ASCII -> https://www.ime.usp.br/~kellyrb/mac2166_2015/tabela_ascii.html

// Variáveis
const int LEDMensagem = 2; // GPIO2
const int sof = 84; // 'T' start of frame
const int eof = 86; // 'V' end of frame
int message[9] = {0,0,0,0,0,0,0,0,0}; //['.','.','.','.','.','.','.','.','.'];
int addess = 0;
int command = '.';
int data = '.';
int incomingByte = 0;
int messageIndex = 0;

// Configurações
void setup() {
    // A taxa de comunicação utilizada deve ser 19200 bps
    Serial.begin(19200);
    // LED para indicar a chegada de informação
    pinMode(LEDMensagem, OUTPUT);
}

// Execução
void loop() {

    // if (Serial.available() > 0)
    // {
    //   incomingByte = Serial.read();
    // };
    incomingByte = readSerialData();

    if (incomingByte == sof)
    {
      message[messageIndex] = sof;
      messageIndex++;
      incomingByte = 0;

      while (incomingByte != sof)
      {
        incomingByte = readSerialData();

        // Sempre que receber um novo caractere de início deve-se reiniciar 
        // o armazenamento da mensagem independente de ter finalizado o anterior.
        if (incomingByte == sof)
        {
          messageIndex = 0;
          message[messageIndex] = sof;
          messageIndex++;
          incomingByte = 0;
          continue;
        }

        // para endereço pode ser qualquer número entre 0 e 9.
        if (incomingByte >= 0 && incomingByte <= 9)
        {
          message[messageIndex] = incomingByte;
          messageIndex++;
          showMessage(message);
          // continue;
        }

        // O campo referente ao comando especifica qual ação
        // deve ser realizada pelo nó ao receber a mensagem.
        if (messageIndex == 2)
        {
          if(incomingByte == 82) // R
          {
            message[messageIndex] = incomingByte;
            messageIndex++;
            showMessage(message);

            incomingByte = readSerialData();

            if (incomingByte == 70) // F
            {
              message[messageIndex] = incomingByte;
              messageIndex++;

              // if (incomingByte == 49)
            }

            if (incomingByte == 76) // L
            {
              message[messageIndex] = incomingByte;
              messageIndex++;
            }
          }

          continue;
        }
      }

    }
    // digitalWrite(LEDMensagem, HIGH);
    // delay(1000);
    // digitalWrite(LEDMensagem, LOW);
    // delay(1000);
}

// Funções de apoio
int readSerialData()
{
  int value;

  if (Serial.available() > 0)
  { 
    value = Serial.read();
    Serial.println(value);
  }
  
  return value;
}

void showMessage(int m[9])
{

  Serial.println(sizeof(m)/sizeof(m[0]));

  for(int i = 0; i < sizeof(m)/sizeof(m[0]); i++)
  {
    Serial.print((char)m[i]);
    // if (m[i] == 0)
    // {
    //   break;
    // }
    // else
    // {
    //   Serial.print((char)m[i]);
    // }
  }
}