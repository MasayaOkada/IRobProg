#include<stdio.h>
#include<math.h>
#include <FlexiTimer2.h>
#define TIME 500    //ms
#define RADIUS 5
#define GEAR_RATIO 9800
float motor_r;
float motor_l;

float kp = 0;
float ki = 0;
float kd = 0;

int adjust_r = kp*diff_r;
int adjust_l = kp*diff_l;

volatile int value1 = 0, value2 = 0;
volatile uint8_t prev1 = 0, prev2 = 0;

unsigned long time;


void setup() {

  pinMode(6,OUTPUT);         //moter
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);

  
  pinMode(2, INPUT); 
  pinMode(3, INPUT);
  pinMode(18,INPUT);
  pinMode(19,INPUT);
  
  attachInterrupt(0, updateEncoder1, CHANGE);
  attachInterrupt(1, updateEncoder1, CHANGE);
  attachInterrupt(4, updateEncoder2, CHANGE);
  attachInterrupt(5, updateEncoder2, CHANGE);
  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(18,HIGH);
  digitalWrite(19,HIGH);

  FlexiTimer2::set(500, motorControl); // 500ms period
  FlexiTimer2::start();
  
  Serial.begin(9600);

}

void updateEncoder1()
{
  uint8_t a = digitalRead(2);
  uint8_t b = digitalRead(3);
 
  uint8_t ab = (a << 1) | b;
  uint8_t encoded  = (prev1 << 2) | ab;

  if(encoded == 0b1101 || encoded == 0b0100 || encoded == 0b0010 || encoded == 0b1011){
    value1 ++;
  } else if(encoded == 0b1110 || encoded == 0b0111 || encoded == 0b0001 || encoded == 0b1000) {
    value1 --;
  }

  prev1 = ab;
}

void updateEncoder2()
{
  uint8_t c = digitalRead(18);
  uint8_t d = digitalRead(19);
 
  uint8_t cd = (c << 1) | d;
  uint8_t encoded  = (prev2 << 2) | cd;

  if(encoded == 0b1101 || encoded == 0b0100 || encoded == 0b0010 || encoded == 0b1011){
    value2 ++;
  } else if(encoded == 0b1110 || encoded == 0b0111 || encoded == 0b0001 || encoded == 0b1000) {
    value2 --;
  }

  prev2 = cd;
}
float deg = 0;           //角度
int rotation = 0;        //回転数
              


void loop() {
  
 analogWrite(6,motor_r+adjust_r);    //motorの定義はしていない
 analogWrite(7,0);
 analogWrite(8,motor_l+adjust_l);
 analogWrite(9,0);

}


void degCalculate(int value){
  
   rotation = value/GEAR_RATIO;
        deg = (value%GEAR_RATIO + rotation)*360;
        
  }

void motorControl(){

int targetSpeed_r = 20;                   //目標速度 cm/s
int targetSpeed_l = 20;

 degCalculate(value1);                  //右のスピード
float currentSpeed_r = PI*RADIUS*deg/(180*TIME);       
int diff_r = (targetSpeed_r   -  currentSpeed_r);

 degCalculate(value2);                  //left speed
float currentSpeed_l = PI*RADIUS*deg/(180*TIME);    
int diff_l = (targetSpeed_l  -  currentSpeed_l);


}



