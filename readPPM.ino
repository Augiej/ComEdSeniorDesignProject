//Reads PPM signals from 6 channels of an RC reciever, translates the values to
//PWM and prints the values to serial port.
//Works with Spectrum DX7 (haven't tested anything else)

//Create variables for 6 channels
int RXCH[6];
volatile int RXSG[6];
int RXOK[6];
int PWMSG[6];
int dir1, dir2, pwm1, pwm2;
int pinDir1, pinDir2, pinPwm1, pinPwm2;
int go = 0;
void setup() {
  pinDir1 = 8;
  pinDir2 = 10;
  pinPwm1 = 9;
  pinPwm2 = 11;
  pinMode(pinDir1, OUTPUT);
  pinMode(pinDir2, OUTPUT);
  pinMode(pinPwm1, OUTPUT);
  pinMode(pinPwm2, OUTPUT);
 //Start communication to serial port
 Serial.begin(115200);  
 Serial.print("Hello\n");
 //Assign PPM input pins. The receiver output pins are conected as below to non-PWM Digital connectors:
 RXCH[0] = 4;  //Throttle
 RXCH[1] = 5;  //Aile / Yaw
 RXCH[2] = 6;  //Elev. / Pitch
 RXCH[3] = 7;  //Rudd. / Roll
 /*RXCH[4] = 8;  //Gear
 RXCH[5] = 9;  //Aux / Flt Mode*/
 
 for (int i = 1; i < 7; i++){
   pinMode(RXCH[i], INPUT);
 }
 
}

void loop() {
 
// Read RX values
 for (int i = 0; i < 4; i++){                                        //for each of the 6 channels:
 RXSG[i] = pulseIn(RXCH[i], HIGH, 20000);                            //read the receiver signal
 if (RXSG[i] == 0) {RXSG[i] = RXOK[i];} else {RXOK[i] = RXSG[i];}    //if the signal is good then use it, else use the previous signal
 PWMSG[i] = map(RXSG[i], 1000, 2000, 0, 511);                        //substitute the high values to a value between 0 and 511
 constrain (PWMSG[i], 0, 511);                                     //make sure that the value stays within the disired boundries
 RXSG[i] = pulseIn(RXCH[i], HIGH, 20000);                            //read the receiver signal
 if (RXSG[i] == 0) {RXSG[i] = RXOK[i];} else {RXOK[i] = RXSG[i];}    //if the signal is good then use it, else use the previous signal
 PWMSG[i] = map(RXSG[i], 1000, 2000, 0, 511);                        //substitute the high values to a value between 0 and 511
 constrain (PWMSG[i], 0, 511);
 
// Print RX values

 Serial.print(" ||   Ch: ");
 Serial.print(i);
 Serial.print(" / PWMSG: ");
 

 PWMSG[0] = abs(PWMSG[0] / 2);
 if(PWMSG[0] < 30){
  PWMSG[0] = 0;
 }
 
 if(PWMSG[1] < 150){
  go = 1;
  dir1 = 0;
  dir2 = 1;
 }
 else if(PWMSG[1] > 350){
  go = 1;
  dir1 = 1;
  dir2 = 0;
 }
 else if(PWMSG[2] < 150) {
  go = 1;
  dir1 = 0;
  dir2 = 0;
 }
 else if(PWMSG[2] > 300){
  go = 1;
  dir1 = 1;
  dir2 = 1;
 }
 else{
  go = 0;
 }
 if(go == 1){
  digitalWrite(pinDir1, dir1);
  digitalWrite(pinDir2, dir2);
  analogWrite(pinPwm1, PWMSG[0]);
  analogWrite(pinPwm2, PWMSG[0]);
 }
 else{
  analogWrite(pinPwm1, 0);
  analogWrite(pinPwm2, 0);
 }
 
 Serial.print(PWMSG[i]);
 delay(1); 
}
Serial.print(" ");
Serial.print(dir1);
Serial.print(" "); 
Serial.print(dir2);
Serial.print(" ");
Serial.print(go);
Serial.print(" ");
Serial.println();
}
