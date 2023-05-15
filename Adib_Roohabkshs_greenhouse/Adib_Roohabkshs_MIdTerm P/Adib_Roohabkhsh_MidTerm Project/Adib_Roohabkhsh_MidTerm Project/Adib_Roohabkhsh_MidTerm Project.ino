#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(20, 4);
  pinMode(3,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
}

int temp1 = EEPROM.read(0);
int min = EEPROM.read(1);
int max = EEPROM.read(2);
unsigned long time1 = 2000;

int tempLogs[8];
int runningLogs[8];

void loop() {
    // put your main code here, to run repeatedly:

  int c = (long)analogRead(A1) * 0.5;
  tempLogs[0]=temp1;
  runningLogs[0]=c;
  menu();
}

int getKeyState(){

  int aRead =analogRead(A0);
  delay(100);
  if(aRead  < 70){
    return 1; 
  }
  else if(aRead  < 235){
    return 2; 
  }
  else if(aRead  < 415){
    return 3; 
  }
  else if(aRead  < 620){
    return 4; 
  }
  else if(aRead  < 880){
    return 5; 
  } 
  else if(aRead  < 1023){
    return 6; 
  } 
  return 0; 
}


void menu(){
  int temp = 0;
  if(temp1 == 255){
    temp1 = 0;  
  }
  if(min == 255){
    min = 0;  
  }
  if(max == 255){
    max = 0;  
  }
  printmenu();

  lcd.setCursor(1,temp);
  lcd.print('>');
  lcd.setCursor(18,temp);
  lcd.print('<');
  
  while(true){

    check();
    saveLogs();
    
    switch(getKeyState()){
      case 1: 
        if(temp > 0){
          temp = temp - 1;
        }
        lcd.clear();
        printmenu();
        lcd.setCursor(1,temp);
        lcd.print('>');
        lcd.setCursor(18,temp);
        lcd.print('<');
        
        waitForKey();
        break;

      case 2: 
        if(temp < 3){
          temp = temp + 1;
        }
        lcd.clear();
        printmenu();
        lcd.setCursor(1,temp);
        lcd.print('>');
        lcd.setCursor(18,temp);
        lcd.print('<');

        waitForKey();
        break;

      case 3: 
        switch(temp){
          case 0:
              delay(300);
              setTemp();
              break;
          case 1: 
              delay(300);
              setRange();              
              break;
          case 2:
             delay(300);
             printTempLog();
             break;
          case 3: 
             delay(300);
             printCurrentLog();
             break;
        }
      case 4:   
        break;
      case 0:
        check();
        break;
    }
  }
  delay(50);
}



void setTemp(){
  temp1 = EEPROM.read(0);
  if(temp1 >= 100){
    temp1 = 0;  
  }
  int temp = 1;
  int temp2 = temp1;

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("SET TEMP");
  lcd.setCursor(3,1);
  lcd.print("           ");
  lcd.setCursor(5,1);
  lcd.print(temp2);

  while(true){  
    saveLogs();  
    switch(getKeyState()){
      case 1: 
      if(temp2<99){
        temp2 = temp2 + 1;
      }
        lcd.setCursor(3,1);
        lcd.print("           ");
        lcd.setCursor(5,1);
        lcd.print(temp2);

        waitForKey();
        break;

      case 2: 
      if(temp2>0){
        temp2 = temp2 - 1;
      }
        
        lcd.setCursor(3,1);
        lcd.print("           ");
        lcd.setCursor(6,1);
        lcd.print(' ');
        
        lcd.setCursor(5,1);
        lcd.print(temp2);

        waitForKey();
        break;

      case 3: 
        EEPROM.write(0, temp2);
        temp1=temp2;
        
        delay(300);
        menu();
        
      case 4: 
        delay(300);
        menu();
      case 0:

        break;
    }
  }
}


void setRange(){
  min = EEPROM.read(1);
  max = EEPROM.read(2);
  int min1 = min;
  int max1 = max;
  
  int temp = 1;
  if(min1 >= 100){
    min1 = 0;  
  }
  if(max1 >= 100){
    max1 = 0;  
  }
  printRangeMenu();
  lcd.setCursor(11,1);
  lcd.print(min1);
  lcd.setCursor(11,2);
  lcd.print(max1);
  
  lcd.setCursor(1,temp);
  lcd.print('>');
  lcd.setCursor(18,temp);
  lcd.print('<');
  while(true){
    saveLogs();
    switch(getKeyState()){
      case 1: 

        if(temp > 1){
          temp = temp - 1;
        }
        lcd.clear();
        printRangeMenu();
        lcd.setCursor(11,1);
        lcd.print(min1);
        lcd.setCursor(11,2);
        lcd.print(max1);
        
        lcd.setCursor(1,temp);
        lcd.print('>');
        lcd.setCursor(18,temp);
        lcd.print('<');
        
        waitForKey();
        break;

      case 2: 

        if(temp < 2){
          temp = temp + 1;
        }
        lcd.clear();
        printRangeMenu();
        lcd.setCursor(11,1);
        lcd.print(min1);
        lcd.setCursor(11,2);
        lcd.print(max1);
        
        lcd.setCursor(1,temp);
        lcd.print('>');
        lcd.setCursor(18,temp);
        lcd.print('<');

        waitForKey();
        break;

      case 3: 

        EEPROM.write(1, min1);
        min = min1;
        EEPROM.write(2, max1);  
        max = max1;
        
        delay(300);
        menu();
        
      case 4:
        delay(300);
        menu();
        
      case 5: 
        if(temp == 1){
          if(min1<99 && min1<max1){
            min1 = min1 + 1;
          }
          lcd.setCursor(11,1);
          lcd.print("     ");
          lcd.setCursor(11,1);
          lcd.print(min1);
        }
        else if(temp == 2){
          if(max1<99 && min1<=max1){
            max1 = max1 + 1;
          }
          lcd.setCursor(11,2);
          lcd.print("     ");
          lcd.setCursor(11,2);
          lcd.print(max1);
        }
        waitForKey();
        break;
        
      case 6: 
        if(temp == 1){
          if(min1>0 && min1<max1){
            min1 = min1 - 1;
          }
          lcd.setCursor(11,1);
          lcd.print("     ");
          lcd.setCursor(11,1);
          lcd.print(min1);
        }
        else if(temp == 2){
          if(max1>0 && min1<=max1){
            max1 = max1 - 1;
          }
          lcd.setCursor(11,2);
          lcd.print("     ");
          lcd.setCursor(11,2);
          lcd.print(max1);
        }
        waitForKey();
        break;

      break;

      case 0:

        break;
    }
  }
  
}


void saveLogs(){
    unsigned long time = millis();
   
    if( time>=time1 ){
      for(int i = 7; i>0 ; i--){        
        runningLogs[i]=runningLogs[i-1];
        tempLogs[i]=tempLogs[i-1];
      }
      tempLogs[0]=temp1;
      int c1 = (long)analogRead(A1) * 0.5;
      runningLogs[0]=c1;
      time1=time1+2000;
      Serial.print("Temp: ");
      Serial.println(temp1);
      Serial.print("Current: ");
      Serial.println(c1);
    }
    delay(1);
}
void printmenu(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Temp: ");
  lcd.setCursor(12,0);
  lcd.print(temp1);
  lcd.setCursor(5,1);
  lcd.print("Range: ");
  lcd.setCursor(12,1);
  lcd.print(min);
  lcd.setCursor(14,1);
  lcd.print(" - ");
  lcd.setCursor(16,1);
  lcd.print(max);
  lcd.setCursor(4,2);
  lcd.print(" LOG MENU(Temp)");
  lcd.setCursor(2,3);
  lcd.print(" LOG MENU(Running)");
  
}

void printRangeMenu(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Range");
  lcd.setCursor(6,1);
  lcd.print("Min");
  lcd.setCursor(6,2);
  lcd.print("Max");
  
}


void printTempLog(){
  lcd.clear();
  for(int i = 0; i<4 ; i++){
    lcd.setCursor(0,i);
    lcd.print(i+1);
    lcd.setCursor(1,i);
    lcd.print(":");
    lcd.setCursor(3,i);
    lcd.print(tempLogs[i]);
  }
  for(int i = 0; i<4 ; i++){
    lcd.setCursor(10,i);
    lcd.print(i+5);
    lcd.setCursor(11,i);
    lcd.print(":");
    lcd.setCursor(13,i);
    lcd.print(tempLogs[i+4]);
  }
  while(true){
    saveLogs();
    switch(getKeyState()){
      case 3: 
        delay(200);
        menu();
        
      case 4: 
        delay(200);
        menu();
      
      case 0:

        break;
    }
  }
}

void printCurrentLog(){
  lcd.clear();
  for(int i = 0; i<4 ; i++){
    lcd.setCursor(0,i);
    lcd.print(i+1);
    lcd.setCursor(1,i);
    lcd.print(":");
    lcd.setCursor(3,i);
    lcd.print(runningLogs[i]);
  }
  for(int i = 0; i<4 ; i++){
    lcd.setCursor(10,i);
    lcd.print(i+5);
    lcd.setCursor(11,i);
    lcd.print(":");
    lcd.setCursor(13,i);
    lcd.print(runningLogs[i+4]);
  }
  while(true){
    saveLogs();
    switch(getKeyState()){
      case 3: 
        delay(200);
        menu();
        
      case 4: 
        delay(200);
        menu();
      
      case 0:

        break;
    }
  }
}


void waitForKey(){
  do{
    while(getKeyState() != 0)
    {}
    delay(20);
  }while(getKeyState() != 0);
}


void check(){
  int c = (long)analogRead(A1) * 0.5;

  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  if(c > temp1){
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    if(temp1 == c-1){
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
    }
    else if(temp1 == c-2){
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
    }
    else if(temp1 < c-2){
      digitalWrite(12,HIGH);
      digitalWrite(13,HIGH);
    }
  }
  else if(c < temp1){
    
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    if(temp1 == c+1){
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
    }
    if(temp1 == c+2){
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
    }
    else if(temp1 > c+2){
      digitalWrite(10,HIGH);
      digitalWrite(11,HIGH);
    }
  }
  else{
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
  }

  if(c >= min && c <= max){
    digitalWrite(3,LOW);
  }
  else if(c < min || c > max){
    digitalWrite(3,HIGH);
  }
  
  
}
