#include "mbed.h"
#include"QEI.h"
#include"I2CSlave.h"

#define ROTATE_PER_REVOLUTIONS  600 //エンコーダ分解能

// エンコーダ読み取りピン
#define CHANNNEL1_A D2
#define CHANNNEL1_B D3

#define CHANNNEL2_A D7
#define CHANNNEL2_B D6

#define CHANNNEL3_A D8
#define CHANNNEL3_B D9

#define CHANNNEL4_A D10
#define CHANNNEL4_B D11

//i2c
#define SDA D4
#define SCL D5

#define ADDR 0x02 //i2cアドレス

//inputにセット
DigitalIn Ch1A(CHANNNEL1_A),Ch1B(CHANNNEL1_B);
DigitalIn Ch2A(CHANNNEL2_A),Ch2B(CHANNNEL2_B);
DigitalIn Ch3A(CHANNNEL3_A),Ch3B(CHANNNEL3_B);
DigitalIn Ch4A(CHANNNEL4_A),Ch4B(CHANNNEL4_B);

Serial pc(USBTX, USBRX);
I2CSlave slave(SDA,SCL);

QEI enc1(CHANNNEL1_A,CHANNNEL1_B,NC,ROTATE_PER_REVOLUTIONS);
QEI enc2(CHANNNEL2_A,CHANNNEL2_B,NC,ROTATE_PER_REVOLUTIONS);
QEI enc3(CHANNNEL3_A,CHANNNEL3_B,NC,ROTATE_PER_REVOLUTIONS);
QEI enc4(CHANNNEL4_A,CHANNNEL4_B,NC,ROTATE_PER_REVOLUTIONS);

void setup(){ 
    
    enc1.reset();
    enc2.reset();
    enc3.reset();
    enc4.reset();
    
    Ch1A.mode(PullUp);
    Ch1B.mode(PullUp);
    Ch2A.mode(PullUp);
    Ch2B.mode(PullUp);
    Ch3A.mode(PullUp);
    Ch3B.mode(PullUp);
    Ch4A.mode(PullUp);
    Ch4B.mode(PullUp);
}
int main(){
    
    setup();
    slave.frequency(400000);
    slave.address(ADDR);
    
    static long enc1Data,enc2Data,enc3Data,enc4Data;
    
    char buf[16];
    
    while(1) {
      
    int  i=slave.receive();
      
        enc1Data=enc1.getPulses();
        enc2Data=-enc2.getPulses();
        enc3Data=enc3.getPulses();
        enc4Data=enc4.getPulses();
                  
          buf[0]=enc1Data>>24 & 0x00ff;
          buf[1]=enc1Data>>16 & 0x00ff;
          buf[2]=enc1Data>>8 & 0x00ff;
          buf[3]=enc1Data & 0x00ff;
          
          buf[4]=enc2Data>>24 & 0x00ff;
          buf[5]=enc2Data>>16 & 0x00ff;
          buf[6]=enc2Data>>8 & 0x00ff;
          buf[7]=enc2Data & 0x00ff;
          
          buf[8]=enc3Data>>24 & 0x00ff;
          buf[9]=enc3Data>>16 & 0x00ff;
          buf[10]=enc3Data>>8 & 0x00ff;
          buf[11]=enc3Data & 0x00ff;
          
          buf[12]=enc4Data>>24 & 0x00ff;
          buf[13]=enc4Data>>16 & 0x00ff;
          buf[14]=enc4Data>>8 & 0x00ff;
          buf[15]=enc4Data & 0x00ff;
          
        pc.printf("%d %d %d %d\n",enc1Data,enc2Data,enc3Data,enc4Data);
        
        //マスターからのリクエストが来た際にデータを送信
        if(i==I2CSlave::ReadAddressed){
        slave.write(buf,16);      
        }
    }
}

