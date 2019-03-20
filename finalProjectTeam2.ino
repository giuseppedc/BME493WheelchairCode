/* Final Project Code: Wheelchair-Mounted Distance Sensor
 * Giuseppe DeLeers-Certo, Jessica Gray, Payton Rollag, Thu Yein
 */
 #include <Servo.h>
 #include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myServo;
const int music= 3;
const int pingIn= 4;
const int pingOut= 5;
const int target = 45; //45 cm
const int scaleFreq[] = { 523,   // C5
                          494,   // B4
                          440,   // A4
                          392,   // G4
                          349,   // F4
                          330,   // E4
                          294,   // D4
                          262};  // C4
bool onceFlag = true;//flag ensures program only runs once
void setup() {
  myServo.attach(6); //pin assigned to servo operation
  
  pinMode(pingIn, INPUT);//pins assigned for ultrasonic sensors
  pinMode(pingOut,OUTPUT);
  
  pinMode(music,OUTPUT);//pin for making music
   
  pinMode(7,OUTPUT);//The rest of these are for the LCD
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  Serial.begin(9600); //baud rate is initialized to match LCD innate baud rate
  lcd.begin(16,2); //LCD initialized
}


void loop() {
  // put your main code here, to run repeatedly:
  if (onceFlag==true)
    int ref= heightCheck();
    int maxDist=pingCheck();
    int dist=maxDist;
    /*The current distance is measured
     * and given to the user in the loop below.
     */
    while (dist>=target){
      dist=pingCheck();
      int freq=pitchSet(dist,maxDist-target);
      tone(music, freq);
      lcd.print("distance: ");
      lcd.print(dist-target);
      lcd.print("cm");
      delay(200); //This delay makes it easier to read the lcd screen
      lcd.clear();
    }
    /*When target is reached, 
     *user is alerted via buzzer and lcd.
     */
    lcd.print("Stop!!");
    onceFlag=false;
    for (int i=0; i<3; i++){
      tone(music,699); //This frequency is F5, the upper fourth from C5
      delay(30);      //This delay and the one below dictate the length of the beeps
      noTone(music);
      delay(220);
    }
    int diff= heightCheck();
    lcd.clear();
    lcd.print("height: ");
    //The height displayed to the user is calculated using the difference between initial and final height measurements.
    lcd.print(ref-diff);
    lcd.print("cm");
  }
}
/*  function int heightCheck()
 *  Rotates the servo 90 degrees, so the ping sensor is facing the floor...
 *    then measures the vertical distance. The sensor is then rotated -90 degrees...
 *    so it can measure horizontal distance.
 * INPUTS: None
 * OUTPUTS: int height - the vertical distance measured by the ping sensor.
 */
int heightCheck(){
  myServo.write(0);
  delay(500); //These delays help ensure the servo has reached the correct position before measuring.
  int height = pingCheck();
  myServo.write(90);
  delay(500);
  return height;
}
/* function int pingCheck()
 * Activates the ultrasonic sensor, passes resulting microsecond duration to
 *    function microsecondsToCentimeters to convert to distance in cm.
 *    
 *    Inputs: none 
 *    Outputs: int distance - the distance measured in cm.
 */
int pingCheck(){
    long duration, distance;
    digitalWrite(pingOut, LOW);
    delayMicroseconds(2);
    digitalWrite(pingOut, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingOut, LOW);
    duration = pulseIn(pingIn, HIGH);
    distance = microsecondsToCentimeters(duration);
    return distance;
}
/*  function long  mircrosecondsToCentimeters(long microseconds)
 *  Converts incoming time duration to centimeters from microseconds.
 *  Inputs: long microseconds -- time duration of ping in microseconds.
 *  Outputs: long microseconds /29 /2 -- distance in cm.
 */
long microsecondsToCentimeters(long microseconds) {
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the object we
    // take half of the distance travelled.
    return microseconds / 29 / 2;
}
/* function int pitchSet(int dist, int range)
 * Assigns a relevant pitch to a recorded distance. System scales based on initial distance.
 * Pitch is selected from scaleFreq array.
 * 
 * Inputs: int dist - the current horizontal distance recorded by the ping sensor
 *         int range - the intial amount of horizontal distance between the device and the location (maxDist-target)
 * Outputs:int pitch - the buzzer frequency assigned to the current distance.
 */
int pitchSet(int dist, int range){
  int pitch;
  for (int i=1;i<9;i++){
    if ((dist-target)>=ceil(range*0.125*i)){
      pitch=scaleFreq[i-1];
    }
  }
  return pitch;
}
