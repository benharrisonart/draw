/* 
 * Project Chromesthesia
 * Author: BH
 * Date: 10-28-2024
 */

#include "Particle.h"
#include <Encoder.h>
#include "neopixel.h"
#include "Colors.h"//included in IoTClassroom_CNM so Don't Install
#include "IoTClassroom_CNM.h"// includes Button.h and Colors.h and wemo.h
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h" //dont install .. part of SSD1306


SYSTEM_MODE(SEMI_AUTOMATIC);
int wavMax = 100;
int wavMin = 0;
//const int array[5];
// int Random;
// int randomWav;
int bri = 25;
float t;
const float A(127.5);
const float B(127.5);
const float v(1/5.0);
const int LEDONE = D1;
//const int LEDONE = D2;
int pospix = 0;
int i;

Adafruit_NeoPixel pixel(60, D3, WS2812B);//Argon
//Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);//Photon

//ENCODER
//ARGON
Encoder encoderA(D9,D10);
Encoder encoderB(D13,D14);
Encoder encoderC(D15,D16);
//PHOTON
// Encoder encoderA(D8,D9);
// Encoder encoderB(D16,D15);
// Encoder encoderC(D17,D18);

int vEncA;//value from encoderA
int vEncB;//value from encoderB
int vEncC;//value from encoderC
int prevEncA;//to hold current value of encoderA
int prevEncB;//to hold current value of encoderB
int prevEncC;//to hold current value of encoderC
int eMax = 95;//max for encoder input
int eMin = -95;// min for encoder input
int vAdjA; int vAdjB; int vAdjC;//adjusted values from encoder for conversion
int eAdj = 95; // add to values to remove all negative numbers

int colorCount;// not used yet
int colorAdjust;// not used yet
int vColorA;// not used yet
//FREQUENCIES
int vFreqA; int vFreqB; int vFreqC; //frequencies converted from encoders ABC
int arrFreq[7] = {220, 251, 282, 313, 344, 375, 406};
int tempArray;
char arrNote[7] = {'A','B','C','D','E','F','G'};
const int sPINA = 19;//speakerA
const int sPINB = 8;//speakerB
const int sPINC = 7;//speakerC
//char cNote;

//ENCODER BUTTON
const int BTNA = D4;//encoder button 
Button buttEncA(4);
bool buttonEncStateA;//on or off
const int BTNB = D5;//encoder button 
Button buttEncB(5);
bool buttonEncStateB;//on or off
const int BTNC = D6;//encoder button 
Button buttEncC(6);
bool buttonEncStateC;//on or off

// HUE BULB
const int BULB=5; 
int color;
int vColor = 0;

//OLED
const int OLED_RESET=-1;
Adafruit_SSD1306 display(-1);
// WEMO
//const int MYWEMO=4; 
int WEMO5=5;
int WEMO4=4;

//BUTTON
bool buttonState;//on or off
const int BTNPIN = 17;
Button btnVolumeOff(BTNPIN);

//const int PIXELCOUNT = 3;


void setup() {
    pinMode(sPINA,OUTPUT);//speaker 1
    pinMode(sPINB,OUTPUT);//speaker 2
    pinMode(sPINC,OUTPUT);//speaker 3
    pinMode(6,INPUT_PULLDOWN);//encoder btn
// LED PINMODES
  // pinMode(D1,OUTPUT);
  // pinMode(D16,OUTPUT);
  // pinMode(D15,OUTPUT);
  Serial.begin (9600); waitFor (Serial.isConnected, 10000);
  //WIFI 
    // WiFi.on();WiFi.clearCredentials();WiFi.setCredentials("IoTNetwork");WiFi.connect();
    // while(WiFi.connecting()) {Serial.printf(".");}Serial.printf("\n\n");
    //ENCODER
    pixel.begin();
    pixel.setBrightness(bri);
    pixel.show();
//OLED
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.setTextColor(WHITE);
    //display.printf("Hello World!\n");
    //display.printf("Note: %c\n",arrNote[3]);
    display.setTextSize(2);
    display.setCursor(0,0);
    display.printf("P1 Ready");
    display.setCursor(0,23);
    display.printf("P2 Ready");
    display.setCursor(0,46);
    display.printf("P3 Ready");
    display.display();

      for(i=0;i<60;i++){pixel.setPixelColor(i,122,66,200);}pixel.show();


}

void loop() {


//ENCODER CODE
      vEncA = encoderA.read(); 
      vEncB = encoderB.read();
      vEncC = encoderC.read();
      if (vEncA <= eMin){encoderA.write(eMin); vEncA = eMin; /* limit encoder to eMin*/}
      if (vEncA >= eMax){encoderA.write(eMax); vEncA = eMax; /* limit encoder to eMax*/}
      if (vEncB <= eMin){encoderB.write(eMin); vEncB = eMin; /* limit encoder to eMin*/}
      if (vEncB >= eMax){encoderB.write(eMax); vEncB = eMax; /* limit encoder to eMax*/}
      if (vEncC <= eMin){encoderC.write(eMin); vEncC = eMin; /* limit encoder to eMin*/}
      if (vEncC >= eMax){encoderC.write(eMax); vEncC = eMax; /* limit encoder to eMax*/}
      if (prevEncA != vEncA){Serial.printf ("Encoder A position: %i\n",vEncA);
            vAdjA = vEncA + eAdj;// raise baseline to remove negative numbers
            vAdjA = vAdjA/27.5;// convert encoderA level to color bracket
            Serial.printf ("Encoder vAdjA: %i\n",vAdjA);
            vFreqA = arrFreq[vAdjA];//vFreqA will be the NOTE frequency
            //Serial.printf ("A Frequency is : %i\n",vFreqA);
            Serial.printf ("vAdjA is: %i\n",vAdjA);
            
            display.setCursor(0,0);display.fillRect(0, 0, 128, 22, BLACK);display.display();
            display.setCursor(0,0);display.printf("Note 1: %c",arrNote[vAdjA]);
            display.display();
            pixel.setPixelColor(0,rainbow[vAdjA]);
            for(i=0;i<20;i++){pixel.setPixelColor(i,rainbow[vAdjA]);}pixel.show();
            pixel.show();
            noTone(sPINA);
            tone(sPINA,vFreqA);
      }
      if (prevEncB != vEncB){Serial.printf ("Encoder B position: %i\n",vEncB);
            vAdjB = vEncB + eAdj;
            vAdjB = vAdjB/27.5;// convert encoderB level to color bracket
            Serial.printf ("Encoder vAdjB: %i\n",vAdjB);
            vFreqB = arrFreq[vAdjB];//vFreqB will be the NOTE frequency for nob2
            Serial.printf ("B Frequency is : %i\n",vFreqB);
            display.fillRect(0, 23, 128, 22, BLACK);display.display();
            display.setCursor(0,23);
            display.printf("Note 2: %c",arrNote[vAdjB]);
            display.display();
            pixel.setPixelColor(1,rainbow[vAdjB]);
            for(i=20;i<40;i++){pixel.setPixelColor(i,rainbow[vAdjB]);}pixel.show();
            pixel.show();
            noTone(sPINB);
            tone(sPINB,vFreqB);
      }
      if (prevEncC != vEncC){Serial.printf ("Encoder C position: %i\n",vEncC);
          vAdjC = vEncC + eAdj;
            vAdjC = vAdjC/27.5;// convert encoderC level to color bracket
            Serial.printf ("Encoder vAdjC: %i\n",vAdjC);
            vFreqC = arrFreq[vAdjC];//vFreqC will be the NOTE frequency for nob3
            Serial.printf ("C Frequency is : %i\n",vFreqC);
            display.fillRect(0, 46, 128, 22, BLACK);
            display.display();
            display.setCursor(0,46);
            display.printf("Note 3: %c",arrNote[vAdjC]);
            display.display();
            pixel.setPixelColor(2,rainbow[vAdjC]);
            for(i=40;i<60;i++){pixel.setPixelColor(i,rainbow[vAdjC]);}pixel.show();
            pixel.show();
            noTone(sPINC);
            tone(sPINC,vFreqC);
      }
     // delay(300);
      prevEncA = vEncA; prevEncB = vEncB; prevEncC = vEncC;// set to current value to check for interaction
// if (buttEnc.isClicked()){
//   Serial.printf ("Clicked \n");
// }
      //tone(8,440);
      //delay(1000);
      //noTone(8);
          // if (buttEncA.isClicked()){buttonEncStateA = !buttonEncStateA;
          //   if (buttonEncStateA==TRUE){tone(19,vFreqA); }else{noTone(19);}
          // }
    if (buttEncA.isClicked()){buttonEncStateA = !buttonEncStateA;
            if (buttonEncStateA==TRUE){
              tone(sPINA,vFreqA);
              wemoWrite(WEMO5,HIGH); 
              wemoWrite(WEMO4,LOW); 
              }else{
                noTone(19);
                wemoWrite(WEMO5,LOW);
                wemoWrite(WEMO4,HIGH); 
                }
    }
     if (buttEncB.isClicked()){buttonEncStateB = !buttonEncStateB;
            if (buttonEncStateB==TRUE){
              tone(sPINB,vFreqB); 
              }else{noTone(sPINB);
                }
    }
     if (buttEncC.isClicked()){buttonEncStateC = !buttonEncStateC;
            if (buttonEncStateC==TRUE){
              tone(sPINC,vFreqC); 
              }else{noTone(sPINC);
                }
    }

    if(btnVolumeOff.isClicked()) {
        Serial.printf ("Button clicked \n");
        noTone(sPINA);noTone(sPINB);noTone(sPINC);
    }

    // if (vAdjA == 3 || vAdjB == 5 || vAdjC == 7){
    //   Serial.printf ("Chord struck \n");
    // }
      // pixel.clear();
      // pixel.show();
      // //pospix = posenc*ratiopix;
      // pixel.setPixelColor(2,blue);
      // pixel.show();

//LED TESTING
      // t = millis()/1000.0;
      // bright = A * sin(2 * M_PI * v * t) + B;
      // analogWrite(D1, bright);
      // analogWrite(D16, bright);
      // analogWrite(D15, bright);
  }









//JUNKBOX

// /* 
//  * Project SmartRoom Controller
//  * Author: BH
//  * Date: 10-28-2024
//  */

// #include "Particle.h"
// #include <Encoder.h>
// #include "neopixel.h"
// #include "Colors.h"
// #include "IoTClassroom_CNM.h"
// #include "Adafruit_SSD1306.h"
// #include "Adafruit_GFX.h" //dont install .. part of SSD1306

// //#include "bitmap_anim.h"
// //#include "bitmapImage.h"
// //#include "butterfly_anim.h"

// SYSTEM_MODE(SEMI_AUTOMATIC);
// //ENCODER
// Encoder myEnc(D8,D9);
// int posEnc;//encoder position from read
// int posPix;//pixel position from rise over run equation
// float ratioPix = .474;// derived from rise/run of 45/95
// int currEnc = 0;
// //ENCODER BUTTON
// const int BUTTENCODER = D17;//encoder button 
// Button buttEnc(BUTTENCODER);
// bool buttonEncState;//on or off

// //BUTTONS
// const int PIN = D3;
// const int LEDPIN = D6;
// int inputValue;
// int buttonState;
// const int CHILLDELAY = 1000;
// //BRIGHTNESS
// int currBright;
// int prevBright;
// int bulbBright = 100;
// // HUE BULB
// const int BULB=5; 
// int color;
// int vColor = 0;
// // ANIMATION 
// int anim;
// int sqAnim;
// int bfAnim;
// int animDelay = 7;
// Adafruit_SSD1306 display(-1);


// void setup() {
//   pinMode(PIN,INPUT); pinMode(LEDPIN,OUTPUT);
//   Serial.begin (9600); waitFor (Serial.isConnected, 10000);
//   //WIFI 
//     WiFi.on();WiFi.clearCredentials();WiFi.setCredentials("IoTNetwork");WiFi.connect();
//     while(WiFi.connecting()) {Serial.printf(".");}Serial.printf("\n\n");
//     //DISPLAY SCREEN
//       //display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
//       //display.clearDisplay();
// }

// void loop() {
//   inputValue = digitalRead(PIN);
//   delay(CHILLDELAY);

//  if (inputValue == 1) {
//   digitalWrite(LEDPIN, HIGH); Serial.printf ("Button state is ON \n");
//  }
//  else {
//   digitalWrite(LEDPIN, LOW); Serial.printf ("Button state is OFF \n");
//  }
 
//         if (buttEnc.isClicked()){
//             buttonEncState = !buttonEncState;
//              if (buttonEncState==TRUE){
//               Serial.printf ("ButtEncoder is Clicked\n");
//               //setHue(BULB,true,HueRainbow[color%7],random(32,255),255);
//               setHue(BULB,true,HueGreen,255,255);
//               //color++;
//             }else{
//             //Serial.printf ("Button state is OFF \n");
//             setHue(BULB,false,16383,255,255);
//           }
//           }
         
//       posEnc = myEnc.read();
//       bulbBright = posEnc;
//       if (posEnc != currEnc){
//           Serial.printf ("Encoder value is %i \n",posEnc);
//           setHue(BULB,true,HueGreen,bulbBright+posEnc,255);
//           //delay(3000);
          
//       }
//       currEnc = posEnc;


//  }

