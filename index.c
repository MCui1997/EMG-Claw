/*Four major sections in this code. 
  1. Inputs from the myoware get sent to arduino
  2. Arduino checks to see if threshold value is met for myoware sensor.
  3. Arduino outputs to servos 
  4. Arduino will continously display the data to the display screen.
  5. We will have 4 buttons
    -Toggle Up
    -Toggle Down
    -Troubleshooting
    -Accessing Information

*/
/*Other major section is the calibration which will have it's own section

/*Setup involves connecting all of the components to the arduino. 
  1. Myoware Sensor
  2. Servo 
  3. Display screen to output
*/

//include library for display
#include <LiquidCrystal.h>;
//Initialise the LCD  with the arduino. LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//include library for servos
#include <Servo.h>
//inclue library for serial
#include <SoftwareSerial.h>

//Set up servo object 1
Servo servo;

//Set up myoware
int myoware = 0;

//Threshold variable to be determined from calibration process
int threshold = 0;
int upperThreshold =0;
int lowerThreshold = 0;

//Password
String password= "";

//position values for servos
int pos_1 = 0;

//delaytime is default 15ms
int delayTime = 15;

//Set up the buttons we will use with their corresponding pins
const int toggleUp = 6;
const int toggleDown = 7;
const int troubleShoot = 8;
const int accessInfo = 13;

//determine buttonstates for each button
int toggleUpState = 0;  
int toggleDownState = 0;  
int troubleShootState = 0;  
int accessInfoState = 0;  

void setup() {
//initialize serial communication at 9600 bits per sec
  Serial.begin(9600); 
  
//attach servos to pins 9 
  servo.attach(9);
  
// initialize the button pins as inputs:
  pinMode(toggleUp, INPUT);
  pinMode(toggleDown, INPUT);
  pinMode(troubleShoot, INPUT);
  pinMode(accessInfo, INPUT); 
  
/////Connected to Computer/Password Setup/////////////////////////////////////////////////
//checks if serial port is connected
if (Serial.available()){

//If password is default, blank, you know it's first time
if (password == ""){
//Prints to the computer screen
Serial.println("Please setup your password");

//Gets the password set up
if(Serial.available()){
        password = Serial.readString();
        
    }

//Calibration Process happens only on first time use///////////////////////////////////////////////////

  //set up all variables
  int thresholdWalk[] = {};
  int thresholdJump[] = {};
  int thresholdSquat[] = {};
  int thresholdRaise[] = {};
  int thresholdRotate[] = {};
  int thresholdFlex[] = {};
  
  int thresholdWalkSum = 0;
  int thresholdJumpSum = 0;
  int thresholdSquatSum = 0;
  int thresholdRaiseSum = 0;
  int thresholdRotateSum = 0;
  int thresholdFlexSum = 0;
  
  int thresholdWalkAvg = 0;
  int thresholdJumpAvg = 0;
  int thresholdSquatAvg = 0;
  int thresholdRaiseAvg = 0;
  int thresholdRotateAvg = 0;
  int thresholdFlexAvg = 0;
  
  int thresholdWalkSE = 0;
  int thresholdJumpSE = 0;
  int thresholdSquatSE =0;
  int thresholdRaiseSE = 0;
  int thresholdRotateSE = 0;
  int thresholdFlexSE = 0;
  
  
  int upperThresholdWalk = 0;
  int upperThresholdJump = 0;
  int upperThresholdSquat = 0;
  int upperThresholdRaise = 0;
  int upperThresholdRotate = 0;
  int upperThresholdFlex = 0;
  
  int lowerThresholdWalk = 0;
  int lowerThresholdJump = 0;
  int lowerThresholdSquat = 0;
  int lowerThresholdRaise = 0;
  int lowerThresholdRotate = 0;
  int lowerThresholdFlex = 0;
  
  
  
    //Prompt start of calibration process
    Serial.println("Please start the calibration process");
    
    //This is the normal value myoware sensor detects when completely still
    int normal = analogRead(myoware);
    
    //First exercise is walking for 30 seconds
    Serial.println("Please walk for 30 seconds");
     unsigned long currentMillis = millis();
     int i =0;
      while((millis() - currentMillis) <=30000){
          thresholdWalk[i]= analogRead(myoware);
          i++;
      }
      for (int i =0; i<sizeof(thresholdWalk);i++){
      thresholdWalkSum = thresholdWalk[i] + thresholdWalkSum;
       
      }
      //Average
      thresholdWalkAvg = thresholdWalkSum / sizeof(thresholdWalk);
      //SEM
      for (int i =0; i<sizeof(thresholdWalk); i++){
        
        thresholdWalkSE = sqrt(((thresholdWalk[i] - thresholdWalkAvg)^2)/(sizeof(thresholdWalk)-1));
        
      }
      //UpperThreshold
        upperThresholdWalk = thresholdWalkAvg + thresholdWalkSE;
      //LowerThreshold
        lowerThresholdWalk = thresholdWalkAvg - thresholdWalkSE;
      
    //Second exercise is 10 jumps
    Serial.println("Please jump 10 times");
   
    
    for(int i =0; i<10;){
    if(analogRead(myoware)-normal >= 20){
      
      thresholdJump[i] = analogRead(myoware);
      i = i+1;
      
    }
    for(int i =0; i<sizeof(thresholdJump);i++){
      thresholdJumpSum = thresholdJumpSum + thresholdJump[i];
    }
    //Avg
    thresholdJumpAvg = thresholdJumpSum/(sizeof(thresholdJump));
    }
      //SEM
      for (int i =0; i<sizeof(thresholdJump); i++){
        
        thresholdJumpSE = sqrt(((thresholdJump[i] - thresholdJumpAvg)^2)/(9));
        
      }
       //UpperThreshold
        upperThresholdJump = thresholdJumpAvg + thresholdJumpSE;
      //LowerThreshold
        lowerThresholdJump = thresholdJumpAvg - thresholdJumpSE;
      
    
    
    
    //Next exercise is 10 squats
    Serial.println("Please squat 10 times");
    
    for(int i =0; i<10;){
    if(analogRead(myoware)-normal >= 20){
      
      thresholdSquat[i] = analogRead(myoware);
      i = i+1;
      
    }
    for(int i =0; i<sizeof(thresholdJump);i++){
      thresholdSquatSum = thresholdSquatSum + thresholdSquat[i];
    }
    //Avg
    thresholdSquatAvg = thresholdSquatSum/(sizeof(thresholdSquat));
    }
      //SEM
      for (int i =0; i<sizeof(thresholdSquat); i++){
        
        thresholdSquatSE = sqrt(((thresholdSquat[i] - thresholdSquatAvg)^2)/(9));
        
      }
      //UpperThreshold
        upperThresholdSquat = thresholdSquatAvg + thresholdSquatSE;
      //LowerThreshold
        lowerThresholdSquat = thresholdSquatAvg - thresholdSquatSE;
    
    
    //Next exercise is 10 arm raises
    Serial.println("Please raise both arms 10 times");
     for(int i =0; i<10;){
    if(analogRead(myoware)-normal >= 20){
      
      thresholdRaise[i] = analogRead(myoware);
      i = i+1;
      
    }
    for(int i =0; i<sizeof(thresholdRaise);i++){
      thresholdRaiseSum = thresholdRaiseSum + thresholdRaise[i];
    }
    //Avg
    thresholdRaiseAvg = thresholdRaiseSum/(sizeof(thresholdRaise));
    }
      //SEM
      for (int i =0; i<sizeof(thresholdRaise); i++){
        
        thresholdRaiseSE = sqrt(((thresholdRaise[i] - thresholdRaiseAvg)^2)/(9));
        
      }
      //UpperThreshold
        upperThresholdRaise = thresholdRaiseAvg + thresholdRaiseSE;
      //LowerThreshold
        lowerThresholdRaise = thresholdRaiseAvg - thresholdRaiseSE;
    
    
    
    //Next exercise is 10 arm rotates
    Serial.println("Please rotate both arms 10 times");
     for(int i =0; i<10;){
    if(analogRead(myoware)-normal >= 20){
      
      thresholdRotate[i] = analogRead(myoware);
      i = i+1;
      
    }
    for(int i =0; i<sizeof(thresholdRotate);i++){
      thresholdRotateSum = thresholdRotateSum + thresholdRotate[i];
    }
    //Avg
    thresholdRotateAvg = thresholdRotateSum/(sizeof(thresholdRotate));
    }
      //SEM
      for (int i =0; i<sizeof(thresholdRotate); i++){
        
        thresholdRotateSE = sqrt(((thresholdRotate[i] - thresholdRotateAvg)^2)/(9));
        
      }
      //UpperThreshold
        upperThresholdRotate = thresholdRotateAvg + thresholdRotateSE;
      //LowerThreshold
        lowerThresholdRotate = thresholdRotateAvg - thresholdRotateSE;
    
    //Next exercise is 10 arm flex
    Serial.println("Please flex arm 10 times");
     for(int i =0; i<10;){
    if(analogRead(myoware)-normal >= 20){
      
      thresholdFlex[i] = analogRead(myoware);
      i = i+1;
      
    }
    for(int i =0; i<sizeof(thresholdJump);i++){
      thresholdFlexSum = thresholdFlexSum + thresholdFlex[i];
    }
    //Avg
    thresholdFlexAvg = thresholdFlexSum/(sizeof(thresholdFlex));
    }
      //SEM
      for (int i =0; i<sizeof(thresholdFlex); i++){
        
        thresholdFlexSE = sqrt(((thresholdFlex[i] - thresholdFlexAvg)^2)/(9));
        
      }
      //UpperThreshold
        upperThresholdFlex = thresholdFlexAvg + thresholdFlexSE;
      //LowerThreshold
        lowerThresholdFlex = thresholdFlexAvg - thresholdFlexSE;
  
  
  //SOLVE FOR THRESHOLD VALUES THAT WILL BE USED IN MAIN PROGRAM
  upperThreshold = (upperThresholdWalk + upperThresholdJump + upperThresholdSquat + upperThresholdRaise + upperThresholdRotate + upperThresholdFlex)/6;
  lowerThreshold = (lowerThresholdWalk + lowerThresholdJump + lowerThresholdSquat + lowerThresholdRaise + lowerThresholdRotate + lowerThresholdFlex)/6;
  threshold = (upperThreshold + lowerThreshold) / 2;
  Serial.println("Calibration is complete.");
}

} 
//End of Calibration Section////////////////////////////////////////////////////


}

//End of Set up Section/////


//the main program that continuously loops
void loop() {

//initalize troubleshoot info
int troubleshoot[] = {};  
//initalize actuation info
int minstrength =0;
int medstrength =0;
int maxstrength = 0;
int actuation = 0;

//Only happens in main program if serial connected
if (Serial.available()){
//If the correct password is not given, then the program will not continue
int check =0;


while (check == 0){
//Prompts for actual password
Serial.println("Would you like to access admin operations? If so, please enter password if not, please disconnect serial port");
if (Serial.readString() == password){
    check = 1;
    }
    
    ///Actual code beings for entering ADMIN OPERATIONS//
    Serial.println("Here are your options: Access stored information, Recalibrate, Reset, Display Sampled Signal, Toggle Graded Actuation");
    Serial.println("Please type exactly the option you would like to choose");
    
    //Access stored information
    if(Serial.readString() == "Access stored information"){
      
      for(int i = 0; i<30; i++){
      Serial.print(troubleshoot[i]);
      }
      
    }
    
    
    //To recalibrate, simply the password will be reset so upon restarting the device, it will act like first time 
    if(Serial.readString() == "Recalibrate"){
    password = "";
    Serial.println("Please Restart device");
    }
    
    //Also resets password but also erases all stored info
    if(Serial.readString() == "Reset"){
    password = "";
    Serial.println("Please Restart Device");
    for(int i=0; i<sizeof(troubleshoot); i++){
      
      troubleshoot[i] =0;
    }
    
    
      
    
    //Display the signal
    }
    if(Serial.readString() == "Display Sampled Signal"){
    Serial.println(threshold);
      
    }
    
    //Toggle Graded Actuation
    if(Serial.readString() == "Toggle Graded Actuation"){
      
      Serial.println("Flex your arm minimum strength");
      minstrength = analogRead(myoware);
      Serial.println("Flex your arm with medium strength");
      medstrength = analogRead(myoware);
      Serial.println("Flex your arm with maximumum stregnth");
      maxstrength = analogRead(myoware);
      
      toggleUpState = 0;
      toggleDownState =0;
      
      actuation = 1;
      
    }
    
  }
}


//Main program that functions without serial plugin
//Initialize myoware sensor
  int myoware= 0;
  
  
//check to see if buttons are pushed
// read the state of the pushbutton value:
  toggleUpState = digitalRead(toggleUp);
  toggleDownState = digitalRead(toggleDown);
  troubleShootState = digitalRead(troubleShoot);
  accessInfoState = digitalRead(accessInfo);
  
  //When graded actuation is turned on
  if (actuation ==1){
    if(analogRead(myoware)>minstrength && analogRead(myoware)<medstrength){
      delayTime = 10;
    }
    if(analogRead(myoware)>medstrength && analogRead(myoware)<maxstrength){
      delayTime = 7.5;
    }
    if(analogRead(myoware)>maxstrength){
      delayTime = 5;
    }
  }
  
  // check if the toggle up is pressed. If it is, the buttonState is HIGH:
  if (toggleUpState == HIGH) {
    // change delay time to be shorter
    delayTime = 10;
  } else {
    // change delay time to default
    delayTime = 15;
  }
  
  // check if the toggledown is pressed. If it is, the buttonState is HIGH:
  if (toggleDownState == HIGH) {
    // 
    delayTime = 20;
  } else {
    // 
    delayTime = 15;
  }
  
   // check if the troubleshoot is pressed only if Serial port isn't connected. If it is, the buttonState is HIGH:
  if (troubleShoot == HIGH and !Serial) {
    lcd.print("Please submit errors for troubleshooting");
    //Store your information
    for(int i=0; i<30; i++){
    troubleshoot[i] = analogRead(myoware);
    }
  }
  
   // check if the accessInfo is pressed only if Serial port isn't connected. If it is, the buttonState is HIGH:
  if (accessInfoState == HIGH and !Serial) {
    //display for info
    lcd.print("Please contact your physician");
    
  } 

//determine if value has met threshold value in myoware sensor
if(myoware < upperThreshold && myoware >lowerThreshold){
//activate servo 
  for (pos_1 = 0; pos_1 <= 180; pos_1 += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos_1);              // tell servo to go to position in variable 'pos'
    delay(delayTime);                       // waits 15ms for the servo to reach the position
  } 
} else {
  
   for (pos_1 = 180; pos_1 >= 0; pos_1 -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos_1);              // tell servo to go to position in variable 'pos'
    delay(delayTime);                       // waits 15ms for the servo to reach the position
  }
}
}
