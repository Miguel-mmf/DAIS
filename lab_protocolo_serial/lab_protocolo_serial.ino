// Apoio
  // Tabela ASCII -> https://www.ime.usp.br/~kellyrb/mac2166_2015/tabela_ascii.html

// Variáveis
// const int LEDMensagem = 2; // GPIO2
const int sof = 84; // 'T' start of frame
const int eof = 86; // 'V' end of frame
int message[9] = {0,0,0,0,0,0,0,0,0};
// int addess = 0;
// int command = '.';
// int data = '.';
// int incomingByte = 0;
int messageIndex = 0;

// Configurações
void setup() {
    // A taxa de comunicação utilizada deve ser 19200 bps
    Serial.begin(19200);
    // LED para indicar a chegada de informação
    // pinMode(LEDMensagem, OUTPUT);
}

// Execução
void loop()
{
  if (message[messageIndex] != sof)
    message[messageIndex] = readSerialData();


  if (message[messageIndex] == sof)
  {
    messageIndex++;

    while (message[messageIndex] != sof)
    {
      message[messageIndex] = readSerialData();

      // para endereço pode ser qualquer número entre 0 e 9.
      if (message[messageIndex] >= 48 && message[messageIndex] <= 57)
      {
        message[messageIndex] = message[messageIndex]; // Precisa transformar o valor para a faixa de 0-9?
        messageIndex++;
        continue;
      }

      // O campo referente ao comando especifica qual ação
      // deve ser realizada pelo nó ao receber a mensagem.
      if (messageIndex == 2)
      {
        if(message[messageIndex] == 82) // R
        {
          messageIndex++;

          while(message[messageIndex] != sof)
          {
            message[messageIndex] = readSerialData();
            
            if (message[messageIndex] == 70) // F
            {
              messageIndex++;

              while(message[messageIndex] != sof)
              {
                message[messageIndex] = readSerialData();

                if(message[messageIndex] >= 49 && message[messageIndex] <= 50)
                {
                  messageIndex++;

                  while(message[messageIndex] != sof)
                  {
                    message[messageIndex] = readSerialData();

                    if (message[messageIndex] == eof) // end of frame
                    {
                      Serial.print("Vazao=");
                      Serial.print(" valor para x igual a ");
                      Serial.println(char(message[messageIndex-1]));
                      break;
                    }
                  }
                }
                if (message[messageIndex] == eof) break;
              }
            }

            if (message[messageIndex] == 76) // L
            {
              messageIndex++;

              while(message[messageIndex] != sof)
              {
                message[messageIndex] = readSerialData();

                if (message[messageIndex] == eof) // end of frame
                {
                  Serial.print("Nivel=");
                  Serial.println(" valor lido.");
                  for (int i=0; i < 9; i++){
                    if (message[i] != 0)
                    {
                      Serial.print(char(message[i]));
                      message[i] = 0;
                    }
                  }
                  Serial.println(".");
                }

                if (message[messageIndex] == eof) break;
              }
            }

            if (message[messageIndex] == eof) break;
          }
        }

        if(message[messageIndex] == 66) // B
        {
          messageIndex++;

          while(message[messageIndex] != sof)
          {
            message[messageIndex] = readSerialData();
            
            if (message[messageIndex] >= 49 && message[messageIndex] <= 50) // 1 ou 2
            {
              messageIndex++;

              while(message[messageIndex] != sof)
              {
                message[messageIndex] = readSerialData();

                if(message[messageIndex] >= 48 && message[messageIndex] <= 49)
                {
                  messageIndex++;

                  while(message[messageIndex] != sof)
                  {
                    message[messageIndex] = readSerialData();

                    if (message[messageIndex] == eof) // end of frame
                    {
                      if (message[messageIndex-1] == 48)
                      {
                        Serial.print("Bomba ");
                        Serial.print(char(message[messageIndex-2]));
                        Serial.println(" desligada.");
                      }
                      else
                      {
                        Serial.print("Bomba ");
                        Serial.print(char(message[messageIndex-2]));
                        Serial.println(" acionada.");
                      }
                      break;
                    }
                  }
                }
                if (message[messageIndex] == eof) break;
              }
            }
            if (message[messageIndex] == eof) break;
          }
        }

        if(message[messageIndex] == 83) // S
        {
          messageIndex++;
        }
      }
    }

    // Sempre que receber um novo caractere de início deve-se reiniciar 
    // o armazenamento da mensagem independente de ter finalizado o anterior.
    if (message[messageIndex] == sof)
    {
      messageIndex = 0;
      message[messageIndex] = sof;
      for (int i=0; i < 9; i++){
        if (message[i] != 0)
        {
          Serial.print(char(message[i]));
          message[i] = 0;
        }
      }
      Serial.println(".");
    }
  }
}


// Funções de apoio
void showMessage(int m[9])
{

  Serial.println(sizeof(m)/sizeof(m[0]));

  for(int i = 0; i < 9; i++) //sizeof(m)/sizeof(m[0])
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


int readSerialData()
{
  int value;

  if (Serial.available() > 0)
  { 
    value = Serial.read();
    // Serial.println(char(value));
  }
  
  return value;
}