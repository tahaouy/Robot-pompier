#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ent = 3, sor = 2, devg = 4, devd = 5, derg = 7, derd = 6;
const int baseTemp = 50; 
const int baseGas = 35;
const int maxSpeed = 255;

float peakTemp = -1000; 
float peakGas = 0;
int reached = 0;

void setup() {
  pinMode(sor, OUTPUT);
  pinMode(ent, INPUT);
  pinMode(devg, OUTPUT);
  pinMode(devd, OUTPUT);
  pinMode(derg, OUTPUT);
  pinMode(derd, OUTPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("DETECTEUR ");
  lcd.setCursor(0, 1);
  lcd.print("D'INCENDIE ");
  delay(5000);
}

void loop() {
  
  
  
  
  float temperatureC = readTemperature();
  float gasConcentration = readGasConcentration();
  gasConcentration *=5;
  
  lcd.clear();
  
  
  
  
  if (reached==0) {
    if (temperatureC > baseTemp && temperatureC - baseTemp > 5) {
      moveForward(maxSpeed);
      
      printStatus("Temp élevée");
      
      delay(1000);
      if (temperatureC >= peakTemp) {
        peakTemp = temperatureC; 
      }else{
        reached = 2;
      }
    } else if (gasConcentration > baseGas && gasConcentration - baseGas > 5) {
      moveForward(maxSpeed);
     
      printStatus("gaz élevée");
       
      delay(1000);
      if (gasConcentration >= peakGas) {
        peakGas = gasConcentration; 
      }else{
        reached = 1;
      }
    } else {
      stopRobot(); 
    }
  }
  
   if(reached>0) {
    if(reached==1){
    stopRobot();
      printStatus("Pic de gaz atteint, arrêt du robot");
      lcd.setCursor(0, 0);
      lcd.print("Peak Temp:");
      lcd.print(temperatureC);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Peak Gas:");
      lcd.print(peakGas);
      lcd.print("%");
      rotateAroundFire();
      while (true) {} 

    }else if (reached==2){
     stopRobot();
      printStatus("Pic de température atteint, ");
      lcd.setCursor(0, 0);
      lcd.print("Peak Temp:");
      lcd.print(peakTemp);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Peak Gas:");
      lcd.print(gasConcentration);
      lcd.print("%");
      rotateAroundFire();
      
      while (true) {} 
    }
    
    }
    if (temperatureC<peakTemp)
    {
        stopRobot();
      printStatus("Pic de température atteint, arrêt du robot");
      lcd.setCursor(0, 0);
      lcd.print("Peak Temp:");
      lcd.print(peakTemp);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Peak Gas:");
      lcd.print(gasConcentration);
      lcd.print("%");
      rotateAroundFire();
      while (true) {} 
    }else if (gasConcentration<peakGas)
    {
       stopRobot();
      printStatus("Pic de température atteint, arrêt du robot");
      lcd.setCursor(0, 0);
      lcd.print("Peak Temp:");
      lcd.print(temperatureC);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Peak Gas:");
      lcd.print(peakGas);
      lcd.print("%");
      rotateAroundFire();
      while (true) {} 
    }
    
      
   
  

  if(readdist()<30){
    stopRobot();
    lcd.print("mur proche");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.print(temperatureC);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Gas:");
      lcd.print(gasConcentration);
      lcd.print("%");
    while (true) {}

  }
  
  lcd.setCursor(0, 0);
  lcd.print("gaz: ");
  lcd.print(gasConcentration);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatureC);
  lcd.print(" °C");
  delay(1000);
  delay(1000);
}

void moveForward(int speed) {
  analogWrite(devd, speed);
  analogWrite(devg, speed);
  digitalWrite(derd, LOW);
  digitalWrite(derg, LOW);
}

void stopRobot() {
  analogWrite(devd, 0);
  analogWrite(devg, 0);
  digitalWrite(derd, LOW);
  digitalWrite(derg, LOW);
}

float readTemperature() {
  int lec = analogRead(0);
  float vol = lec * 5.0 / 1024;
  return (vol - 0.5) * 100;
}

float readGasConcentration() {
  int lecc = analogRead(1);
  return map(lecc, 0, 1023, 0, 100);
}


void printStatus(String message) {
  Serial.println(message);
}
float readdist(){
    digitalWrite(sor, HIGH);
  digitalWrite(sor, LOW);
  float dist = (pulseIn(ent, HIGH)) / 58;
  return dist;
}
void rotateAroundFire() {

  analogWrite(devd, maxSpeed);
  digitalWrite(derd, LOW);
  analogWrite(devg, LOW);
  digitalWrite(derg, LOW);
  printStatus("rotation vers l'incendie et/ou le gaz pour l'etouffer");
  delay(4000);
  printStatus("Operation effectué avec succès");
  

  
  stopRobot();
}
