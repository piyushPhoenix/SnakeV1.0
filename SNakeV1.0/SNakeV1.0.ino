//We always have to include the library
#include "LedControlMS.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                  variables
int x0; int y0; //food
int x1; int y1;
int x2; int y2;
int x3; int y3;
int x4; int y4;
char data;
unsigned long counter;
boolean game_over_flag = 0;
int score = 0;
void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
   randomSeed(analogRead(0));
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
  lc.writeString(0, "Snake V1.0");
  init_snake();
 
  counter = millis();
  food();
  lc.setLed(0,x0,y0,true);
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte r[5]={B00111110,B00010000,B00100000,B00100000,B00010000};
  byte d[5]={B00011100,B00100010,B00100010,B00010010,B11111110};
  byte u[5]={B00111100,B00000010,B00000010,B00000100,B00111110};
  byte i[5]={B00000000,B00100010,B10111110,B00000010,B00000000};
  byte n[5]={B00111110,B00010000,B00100000,B00100000,B00011110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};
 
  /* now display them one by one with a small delay */
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  delay(delaytime);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  delay(delaytime);
  lc.setRow(0,0,d[0]);
  lc.setRow(0,1,d[1]);
  lc.setRow(0,2,d[2]);
  lc.setRow(0,3,d[3]);
  lc.setRow(0,4,d[4]);
  delay(delaytime);
  lc.setRow(0,0,u[0]);
  lc.setRow(0,1,u[1]);
  lc.setRow(0,2,u[2]);
  lc.setRow(0,3,u[3]);
  lc.setRow(0,4,u[4]);
  delay(delaytime);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  delay(delaytime);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  delay(delaytime);
  lc.setRow(0,0,o[0]);
  lc.setRow(0,1,o[1]);
  lc.setRow(0,2,o[2]);
  lc.setRow(0,3,o[3]);
  lc.setRow(0,4,o[4]);
  delay(delaytime);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
  delay(delaytime);
}

/*
  This function lights up a some Leds in a row.
 The pattern will be repeated on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(delaytime);
    lc.setRow(0,row,B10100000);
    delay(delaytime);
    lc.setRow(0,row,(byte)0);
    for(int i=0;i<row;i++) {
      delay(delaytime);
      lc.setRow(0,row,B10100000);
      delay(delaytime);
      lc.setRow(0,row,(byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(delaytime);
    lc.setColumn(0,col,B10100000);
    delay(delaytime);
    lc.setColumn(0,col,(byte)0);
    for(int i=0;i<col;i++) {
      delay(delaytime);
      lc.setColumn(0,col,B10100000);
      delay(delaytime);
      lc.setColumn(0,col,(byte)0);
    }
  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void single() {
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime);
      lc.setLed(0,row,col,true);
      delay(delaytime);
      for(int i=0;i<col;i++) {
        lc.setLed(0,row,col,false);
        delay(delaytime);
        lc.setLed(0,row,col,true);
        delay(delaytime);
      }
    }
  }
}

void loop() {
if(x1 == x0 && y1 == y0){food(); score++;}
int y = analogRead(A1);
int x = analogRead(A2);
if (x > 650){data = 'a';}
if (x < 350){data = 'd';}
if (y > 650){data = 's';}
if (y < 350){data = 'w';}

if(data == 'w'){up();}
if(data == 's'){down();}
if(data == 'd'){right();}
if(data == 'a'){left();}

lc.setLed(0,x1,y1,true);
lc.setLed(0,x2,y2,true);
lc.setLed(0,x3,y3,true);
lc.setLed(0,x4,y4,true);
lc.setLed(0,x0,y0,true);
}

void init_snake(){
x1 = random(3,7);
y1 = random(7);
x2 = x1 - 1;
x3 = x2 - 1;
x4 = x3 - 1;
y4 = y3 = y2 = y1;
Serial.print(x1); Serial.println(y1);
lc.setLed(0,x1,y1,true);
lc.setLed(0,x2,y2,true);
lc.setLed(0,x3,y3,true);
lc.setLed(0,x4,y4,true);
}

void right(){
if(millis() - counter >= 400){
counter = millis();
lc.setLed(0,x4,y4,false);
x4 = x3;
x3 = x2;
x2 = x1;
x1++;
y4 = y3;
y3 = y2;
y2 = y1;
while(x1 == 8){lc.clearDisplay(0); if(game_over_flag == 0){game_over_flag = 1; lc.writeString(0,"Game Over!!"); lc.clearDisplay(0); lc.writeString(0,score); }}
} }

void up(){

if(millis() - counter >= 400){
counter = millis();
lc.setLed(0,x4,y4,false);
y4 = y3;
y3 = y2;
y2 = y1;
y1++;
x4 = x3;
x3 = x2;
x2 = x1;
while(y1 == 8){lc.clearDisplay(0); if(game_over_flag == 0){game_over_flag = 1; lc.writeString(0,"Game Over!!"); lc.clearDisplay(0); lc.writeString(0,score); }} 
}}

void down(){
if(millis() - counter >= 400){
counter = millis();
lc.setLed(0,x4,y4,false);
y4 = y3;
y3 = y2;
y2 = y1;
y1--;
x4 = x3;
x3 = x2;
x2 = x1;
while(y1 == -1){lc.clearDisplay(0); if(game_over_flag == 0){game_over_flag = 1; lc.writeString(0,"Game Over!!"); lc.clearDisplay(0); lc.writeString(0,score); }}
}}

void left(){

if(millis() - counter >= 400){
counter = millis();
lc.setLed(0,x4,y4,false);
x4 = x3;
x3 = x2;
x2 = x1;
x1--;
y4 = y3;
y3 = y2;
y2 = y1;
while(x1 == -1){lc.clearDisplay(0); if(game_over_flag == 0){game_over_flag = 1; lc.writeString(0,"Game Over!!"); lc.clearDisplay(0); String s = String(score); lc.writeString(0,score); }}
}}

void food(){
while((x0 == x1 && y0 == y1)||(x0 == x2 && y0 == y2)||(x0 == x3 && y0 == y3)||(x0 == x4 && y0 == y4)){x0 = random(0,7); y0 = random(0,7);}
}


