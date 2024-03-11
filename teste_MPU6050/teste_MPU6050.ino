/*  Sistema de Aquisição de Dados e Interface - 2023.2
 *  Código MPU6050 (Tiva129)
 *  Maria Paula M., Miguel Marques, Rafael Pereira e Rodrigo Ernesto
 *  Versão 2.0
 */

//  MPU | Tiva
//  VCC | +3V3
//  GND | GND
//  SCL | PB2
//  SDA | PB3

#include <Wire.h>
#define endereco 0x68 // Endereço I2C (0x68) do MPU6050 pq tá no nível lógico baixo no pino AD0 (tópico 9.2 no datasheet)

// Informações Importantes:
// Register 27 – Gyroscope Configuration (GYRO_CONFIG)      Valor = 1B
// Register 28 – Accelerometer Configuration (ACCEL_CONFIG) Valor = 1C
// Register 36 - I2C Master Control (I2C_MST_CTRL)          Valor = 24

long accelX, accelY, accelZ;     // Variáveis para armazenamento dos valores brutos dos registradores de aceleração nos eixos X, Y e Z
float gForceX, gForceY, gForceZ; // Variáveis para armazenamento da aceleração em função da aceleração gravitacional nos eixos X, Y e Z

long gyroX, gyroY, gyroZ;        // Variáveis para armazenamento dos valores brutos dos registradores de rotação nos eixos X, Y e Z
float rotX, rotY, rotZ;          // Variáveis para armazenamento da rotação em °/s(graus por segundo) nos eixos X, Y e Z

void setup() {
  Wire.setModule(0);
  Serial.begin(115200);
  Serial.println("******************** MPU6050 ********************");
  Wire.begin(); 
  Serial.println("Passei pelo Wire.begin()");
  setupMPU(); // Função para config. do modo de energia, da escala de leitura do giroscópio e da escala de leitura do acelerômetro
}

void loop() {
  Serial.println("Tô no loop");
  recordAccelRegisters();        // Função para leitura dados do acelerômetro nos registradores
  recordGyroRegisters();         // Função para leitura dados do giroscópio nos registradores
  printData();                   // Função para visualização dos dados obtidos no monitor serial
  delay(1000);
}

void setupMPU(){
  Wire.beginTransmission(endereco);  
  Wire.write(0x6B);                  // Acessando o registrador 6B(PWR_MGMT_1) (tópico 4.30 do Register Map)
  Wire.write(0x0);                   // Alterando o bit SLEEP para 0 no registrador (pg. 9 do Register Map) 
  Wire.endTransmission(true);  
  Wire.beginTransmission(endereco);  
  Wire.write(0x1B);                  // Acessando o registrador 1B(GYRO_CONFIG) (tópico 4.4 do Register Map)
  Wire.write(0x18);                  // Alterando os bits de FS_SEL para 0 = config. o giroscópio em uma escala de +/- 250°/s (tópico 4.4 do Register Map)
  Wire.endTransmission(true); 
  Wire.beginTransmission(endereco); 
  Wire.write(0x1C);                  // Acessando o registrador 1C(ACCEL_CONFIG) (tópico 4.5 do Register Map)
  Wire.write(0x18);                  // Alterando os bits de AFS_SEL para 0 = config. o acelerômetro em uma escala de +/- 2g (tópico 4.5 do Register Map)
  Wire.endTransmission(true); 
  Serial.println("Passei pelo setupMPU");
}

void recordAccelRegisters() {
  Wire.beginTransmission(endereco);    
  Wire.write(0x3B);                    // Acessando o reg. 3B p/ leitura dos dados do acelerômetro, os dados começam no reg. 3B e terminam no reg. 40 (tópico 4.18 do Register Map)
  Wire.endTransmission(false);
  Wire.requestFrom(endereco, 6, true); // Requerimento de 6 bytes dos registradores com os dados do acelerômetro (3B - 40, sendo 1 byte por reg.) (tópico 4.18 do Register Map)
  while(Wire.available() < 6);         // Aguarda o recebimento dos dados solicitados
  // Armezena os primeiros 2 bytes: 
  accelX = Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  accelY = Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  accelZ = Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  processAccelData();                  // Função para correção dos valores brutos (tópico 4.18 do Register Map)
  Serial.println("Li o acelerômetro");
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(endereco);    
  Wire.write(0x43);                    // Acessando o reg. 43 p/ leitura dos dados do giroscópio, os dados começam no reg. 43 e terminam no reg. 48 (tópico 4.20 do Register Map)
  Wire.endTransmission(false);
  Wire.requestFrom(endereco, 6, true); // Requerimento de 6 bytes dos registradores com os dados do giroscópio (43 - 48, sendo 1 byte por reg.) (tópico 4.20 do Register Map)
  while(Wire.available() < 6);         // Aguarda o recebimento dos dados solicitados
  // Armazena os primeiros 2 bytes:
  gyroX = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyroY = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyroZ = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  processGyroData();                   // Função para correção dos valores brutos (tópico 4.20 do Register Map)
  Serial.println("Li o giroscópio");
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X="); Serial.print(rotX); Serial.print(" Y="); Serial.print(rotY); Serial.print(" Z="); Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X="); Serial.print(gForceX); Serial.print(" Y="); Serial.print(gForceY); Serial.print(" Z="); Serial.println(gForceZ);
}
