// https://energia.nu/reference/en/language/functions/external-interrupts/attachinterrupt/

#define button PJ_0 // button to be used
#define button2 PJ_1
#define led PN_1
#define led2 PN_0
int buttonClick = 0;
bool buttonFlag = true;

void setup() {

  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
  // Create a Interrupt Service Routines (ISR)
  // Generally, an ISR should be as short and fast as possible.
  attachInterrupt(button, intBot1, FALLING);
  attachInterrupt(button2, changeFlag, FALLING);
  
}

void loop(){

  if(buttonClick == 1)
  {
    if (buttonFlag){
      digitalWrite(led2, HIGH);
      Serial.print(buttonFlag);
      Serial.print(" | Acionado por meio da interrupção! Valor: ");
      Serial.println(buttonClick);
    }
    digitalWrite(led, HIGH);
  }
  else if (buttonClick == 2)
  {
    if (buttonFlag){
      digitalWrite(led2, LOW);
      Serial.print(buttonFlag);
      Serial.print(" | Acionado por meio da interrupção! Valor: ");
      Serial.println(buttonClick);
    }
    buttonClick = 0;
    digitalWrite(led, LOW);
  }
}


void intBot1 (void){ buttonClick++;}

void changeFlag(void){ buttonFlag = !buttonFlag;}
/*
 * An ISR cannot have any parameters, and they shouldn’t return anything.
 * Inside the attached function, delay() won’t work and the value returned by millis() will not increment.
 * Serial data received while in the function may be lost.
 * You should declare as volatile any variables that you modify within the attached function. 
*/
