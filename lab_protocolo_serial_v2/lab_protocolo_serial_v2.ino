// Apoio
  // Tabela ASCII -> https://www.ime.usp.br/~kellyrb/mac2166_2015/tabela_ascii.html

// Variáveis
const char sof = 'T'; // 'T' start of frame
const char eof = 'V'; // 'V' end of frame
int message[9] = {0,0,0,0,0,0,0,0,0};
int adress = 0;
int incomingByte = 0;
int messageIndex = 0;

// Configurações
void setup() {
    // A taxa de comunicação utilizada deve ser 19200 bps
    Serial.begin(19200);
}

// Execução
void loop()
{

  // incomingByte = readSerialData();
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();

    if (incomingByte == sof)
    {
      messageIndex = 0;
      message[messageIndex] = incomingByte;
      messageIndex++;
    }
    
    else if (incomingByte == eof)
    {
      message[messageIndex] = incomingByte;
      protocol();
      showMessage();
    }

    else
    {
      message[messageIndex] = incomingByte;
      messageIndex++;
    }
  }
}


// Funções de apoio

void protocol()
{
  //
  // Esta função tem como objetivo implementar um protocolo de comunicação simples
  // baseado em caracteres ASCII.
  // Os caracteres usados para início e fim devem ser T e V. Para endereço pode
  // ser qualquer número entre 0 e 9. O campo referente ao comando especifica qual
  // ação deve ser realizada pelo nó ao receber a mensagem. O campo de Dados
  // contém a informação adicional do valor referente a um comando enviado
  //

  adress = message[1]; // endereço sempre na segunda posição do quadro de menssagem

  // para endereço pode ser qualquer número entre 0 e 9.
  if (adress >= '0' && adress <= '9')
  {

    switch(message[2]){

      case 'R':

        if (message[3] == 'F')
        {
          if (message[4] >= '1' && message[4] <= '2')
          {
            Serial.println("Vazao = ");
          }
        }
        else if (message[3] == 'L')
        {
          Serial.println("Nivel = ");
        }
        else
          Serial.println("Opcao invalida!");
        
        break;
      
      case 'B':

        if (message[3] == '1')
        {
          if (message[4] >= '1')
          {
            Serial.println("Bomba 1 ligada.");
          }
          else if (message[4] <= '0')
          {
            Serial.println("Bomba 1 desligada.");
          }
          else
            Serial.println("Opcao invalida!");
        }
        else if (message[3] == '2')
        {
          if (message[4] >= '1')
          {
            Serial.println("Bomba 2 ligada.");
          }
          else if (message[4] <= '0')
          {
            Serial.println("Bomba 2 desligada.");
          }
          else
            Serial.println("Opcao invalida!");
        }
        else
          Serial.println("Opcao invalida!");
        
        break;
      
      case 'S':

        if (message[3] == 'B')
        {
          if (message[4] >= '1' && message[4] <= '2')
          {
            Serial.print("Rotacao B");
            Serial.print(char(message[4]));
            Serial.print("= ");
            for(int i=5; i < 9; i++)
            {
              if (message[i] != eof)
                Serial.print(char(message[i]));
              else
                break;
            }
            Serial.println("%");
          }
        }
        else if (message[3] == 'R')
        {
          if (message[4] == 'C')
          {
            Serial.print("Referencia= ");
            for(int i=5; i < 9; i++)
            {
              if (message[i] != eof)
                Serial.print(char(message[i]));
              else
                break;
            }
            Serial.println(" ");
          }
        }
        else if (message[3] == 'E')
        {
          if (message[4] == 'M')
          {
            Serial.println("Estado manual.");
          }
          else if (message[4] == 'A')
          {
            Serial.println("Estado automatico.");
          }
          else
            Serial.println("Opcao invalida!");
        }
        else if (message[3] == 'K')
        {
          if (message[4] == 'P')
          {
            Serial.print("Kp = ");
            for(int i=5; i < 9; i++)
            {
              if (message[i] != eof)
                Serial.print(char(message[i]));
              else
                break;
            }
            Serial.println(" ");
          }
        }
        else if (message[3] == 't')
        {
          if (message[4] == 'i')
          {
            Serial.print("Ti = ");
            for(int i=5; i < 9; i++)
            {
              if (message[i] != eof)
                Serial.print(char(message[i]));
              else
                break;
            }
            Serial.println(" ");
          }
          else if (message[4] == 'd')
          {
            Serial.print("Td = ");
            for(int i=5; i < 9; i++)
            {
              if (message[i] != eof)
                Serial.print(char(message[i]));
              else
                break;
            }
            Serial.println(" ");
          }
          else if (message[4] == 'h')
          {
            Serial.print("Th = ");
            for(int i=5; i < 9; i++)
            {
              if (message[i] != eof)
                Serial.print(char(message[i]));
              else
                break;
            }
            Serial.println(" ");
          }
        }
        else
          Serial.println("Opcao invalida!");
        
        break;
    }
  }
}


void showMessage()
{
  //
  // Esta função tem como objetivo apresentar o comando enviado pelo usuário,
  // bem como resetar o quadro de mensagem disponível para envio do comando.
  //

  for (int i=0; i < 9; i++)
  {
    if (message[i] != 0)
    {
      Serial.print(char(message[i]));
      message[i] = 0;
    }
  }
  Serial.println(".");
}


int readSerialData()
{
  //
  // Esta função tem como objetivo ler os dados recebidos na porta serial
  //
  int value = 0;

  if (Serial.available() > 0)
    Serial.read();
  
  return value;
}