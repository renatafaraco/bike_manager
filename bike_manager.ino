#include <EEPROM.h>

#define EEPROM_time 0
#define EEPROM_bike 2
#define EEPROM_list 10
#define tamanho_bike 4

// Preparação
int openedTime = 500;

typedef struct {
  int id = 0;
  bool isOn = 0;
  bool isConnected = 0;
  bool isOpened = 0;
  
} Bike;

byte serialNumber[4] = {0x01, 0x02, 0x03, 0x04};


int okLed = 13;
int notOkLed = 14;

Bike bikeList[8];
unsigned int remainingTime = 0;
unsigned int bikeNumber = 0;


// Programa principal
void setup() {
  Serial.begin (9600);
  
  // set status led
  setupStatusLed();
  //getting persisted info
  //remainingTime = readTimeRemaining();
  //bikeNumber = readBikeNumber();
  //readBikeList(bikeNumber);
  
}

void loop() {
  //verify TCP comunication
  //verify Wire connection
  //remaining time manager
  char Token[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
  loadTime(Token);
  delay(10000);
  delay(10);
  everythingIsOk();
  delay(500);
  weHaveProblem();
  delay(500);
}


/*
 * funções de status de led
 */
void setupStatusLed(void) {
  pinMode(okLed, OUTPUT);
  pinMode(notOkLed, OUTPUT);
  everythingIsOk();
}

void everythingIsOk(void) {
  digitalWrite(okLed, HIGH);
  digitalWrite(notOkLed, LOW);
}

void weHaveProblem(void) {
  digitalWrite(okLed, LOW);
  digitalWrite(notOkLed, HIGH);
}

// funções de comunicação TCP


// funcoes de comunicação i2c


// gerenciamento de tempo
void loadTime(char* token) {
  int key = int(token[0]);//a     0a 1b 2c 3d 4e 5f 6g 7h 8i 9j
  
  byte serialNumberRaw[4] = {byte(token[1]), byte(token[3]), byte(token[6]), byte(token[8])};//bdgi
  byte timeToLoad[4] = {byte(token[2]), byte(token[4]), byte(token[5]), byte(token[9])}; //cefj 
  //long int serialN = long int(serialNumber);
  long int primeiroInt = int(serialNumberRaw[0]) + 256*int(serialNumberRaw[1]) + 65536*int(serialNumberRaw[2]) + 16777216*(serialNumberRaw[3]);
  Serial.print("Token: ");
  Serial.println(token);
  Serial.print("Key: ");
  Serial.println(key);
  Serial.print("Serial number Raw: ");
  Serial.println(int(serialNumberRaw));
  Serial.print("Time to load: ");
  Serial.println(int(timeToLoad));
  Serial.print("Teste: ");
  Serial.println(primeiroInt);
  Serial.print("Tamanho do int: ");
  Serial.println(sizeof(long int));
   
}

int getTimeRemainig(void) {
  return readTimeRemaining();
}

/* 
* gerenciamento de bicicletas 
*/
 
void addBike (Bike* bikeList, Bike bike, int numberBike) {
    bikeList[numberBike] = bike;
    numberBike++;
}


void removeBike(Bike *bikeList, int numberBike, int id) {
  bool success = false;
  for (int i = 0; i < numberBike; i++) {
    if (bikeList[i].id == id) {
      for (int j = i; j < numberBike-1; j++) {
        bikeList[j] = bikeList[j+1];
      }
      numberBike--;
      success = true;
    }
  }
  if (!success) {
    weHaveProblem();
    Serial.print("Nao foi possivel deletar bike id: ");
    Serial.println(id);
  }
}

Bike* getBike(Bike* bikeList, int id, int numberBike) {
  bool success = false;
  for (int i = 0; i < numberBike; i++) {
    if (bikeList[i].id == id) {
      return &bikeList[i];
      success = true;
    }
  }
  if (!success) {
    weHaveProblem();
    Serial.print("Nao foi possivel achar bike id: ");
    Serial.println(id);
  }
  return NULL;
}

/*
 * funcoes de persistência
 */

int readTimeRemaining(void) {
  int value = EEPROM.read(EEPROM_time);
  return value;   
}

void writeTimeRemaining(int time) {
  EEPROM.write(EEPROM_time, time);
}


int readBikeNumber(void) {
  int value = EEPROM.read(EEPROM_bike);
  return value;
}

void writeBikeNumber(int bike) {
  EEPROM.write(EEPROM_bike, bike);
}

void readBikeList(Bike* bikeList, int bikeNumber) {
  for (int i = 0; i < bikeNumber; i++) 
  {
     readBike(&bikeList[i],i);
  }   
}

void writeBikeList(Bike* bikeList, int numberBike) {
  for (int i = 0; i < bikeNumber; i++) 
  {
     writeBike(&bikeList[i],i);
  }
}

void readBike(Bike* bike, int pos) {
  bike->id = EEPROM.read(EEPROM_list + pos*tamanho_bike);
  bike->isOn = EEPROM.read(EEPROM_list + pos*tamanho_bike + 1);
  bike->isConnected = EEPROM.read(EEPROM_list + pos*tamanho_bike + 2);
  bike->isOpened = EEPROM.read(EEPROM_list + pos*tamanho_bike + 3);
}

void writeBike(Bike* bike, int pos) {
  EEPROM.write(EEPROM_list + pos*tamanho_bike, bike->id);
  EEPROM.write(EEPROM_list + pos*tamanho_bike + 1,  int(bike->isOn));
  EEPROM.write(EEPROM_list + pos*tamanho_bike + 2, int(bike->isConnected));
  EEPROM.write(EEPROM_list + pos*tamanho_bike + 3, int(bike->isOpened));
}

