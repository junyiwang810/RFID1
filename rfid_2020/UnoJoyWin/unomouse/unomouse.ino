/* Arduino USB Mouse HID demo */
 
/* Author: Darran Hunt
 * Release into the public domain.
 */
 
struct {
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t wheel;   /* Not yet implemented */
} mouseReport;
 
uint8_t nullReport[4] = { 0, 0, 0, 0 };
 int up ;
 int down ;
 int left ;
 int right ;
void setup();
void loop();
 
void setup()
{
    Serial.begin(9600);
    delay(200);
    for (int i = 2; i <= 6; i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
   
  }
  
}
 
/* Move the mouse in a clockwise square every 5 seconds */
void loop()
{
  up = digitalRead(2);
  down = digitalRead(3);
  left = digitalRead(4);
  right = digitalRead(5);
    int ind;
    

    mouseReport.buttons = 0;
    mouseReport.x = 0;
    mouseReport.y = 0;
    mouseReport.wheel = 0;
  if (down == LOW){
    mouseReport.x = -2;
    for (ind=0; ind<20; ind++) {
    Serial.write((uint8_t *)&mouseReport, 4);
    Serial.write((uint8_t *)&nullReport, 4);
    }
  }
   if (left == LOW){
    mouseReport.x = 0;
    mouseReport.y = -2;
    for (ind=0; ind<20; ind++) {
    Serial.write((uint8_t *)&mouseReport, 4);
    Serial.write((uint8_t *)&nullReport, 4);
    }
   }
     if (up == LOW){
    mouseReport.x = 2;
    mouseReport.y = 0;
    for (ind=0; ind<20; ind++) {
    Serial.write((uint8_t *)&mouseReport, 4);
    Serial.write((uint8_t *)&nullReport, 4);
    }
     }
      if (right == LOW){
    mouseReport.x = 0;
    mouseReport.y = 2;
    for (ind=0; ind<20; ind++) {
    Serial.write((uint8_t *)&mouseReport, 4);
    Serial.write((uint8_t *)&nullReport, 4);
    }}
}
