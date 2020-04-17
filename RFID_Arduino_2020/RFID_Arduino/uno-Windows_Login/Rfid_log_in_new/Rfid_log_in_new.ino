//=====================================Walid Mafuj===================================//
uint8_t buf[8] = { 0 };
#include <SPI.h>
#include <MFRC522.h> // RFID library
#define SS_PIN 10 //RX slave select
#define RST_PIN 9
int gled = 7; // optional
int rled = 4;// optional
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
String card_ID=""; // 
String password="!qaZxsw2072832" ; // Change It To Your Windows / fb / any Account's Password
String rfid="165202200230";// UID (unique Id Code Of Your Rfid Tag)
void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  pinMode(gled,OUTPUT);
  pinMode(rled,OUTPUT);
}

void loop() {
  //look for new card
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }
 
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID += mfrc522.uid.uidByte[i];
  }
 
  if(card_ID==rfid){
    digitalWrite(gled,HIGH);
    typeLiteralString(password);
    pressKey("enter"); releaseKey("enter");
    digitalWrite(gled,LOW); delay(200);digitalWrite(gled,HIGH); delay(200);digitalWrite(gled,LOW);
  }   
  if(card_ID!=password){
    digitalWrite(rled,HIGH);
    digitalWrite(rled,LOW);
    delay(200);
    digitalWrite(rled,HIGH);
    delay(200);
    digitalWrite(rled,LOW);
  }
  else{ goto cont;}   
 
  delay(1000);

cont:
  delay(1000);
  card_ID="";
}

boolean isModifier(int keycode) {
    boolean result = false;
    if (keycode >= 224 && keycode <= 231) { // if the keycode is a modifier key
        result = true;
    }
    return result;
}

void pressModifier(String keyname) {
    pressModifier(getKeycode(keyname));
}
void pressModifier(int keycode) {
    int modifiermask = 0;
    if (isModifier(keycode)) { // if the keycode represents a modifier key
        modifiermask = getModifierMask(keycode);
        buf[0] = buf[0] | modifiermask;
        Serial.write(buf, 8); // Send key report
    }
}

void releaseModifier(String keyname) {
    releaseModifier(getKeycode(keyname));
}
void releaseModifier(int keycode) {
    int modifiermask = 0;
    if (isModifier(keycode)) { // if the keycode represents a modifier key
        modifiermask = getModifierMask(keycode);
        buf[0] = buf[0] & (~modifiermask);
        Serial.write(buf, 8); // Send key report
    }
}
void releaseAllModifiers() {
    buf[0] = B00000000;
    Serial.write(buf, 8); // Send key report
}

void pressKey(String keyname) {
    pressKey(getKeycode(keyname));
}
void pressKey(int keycode) { // TODO: cycle the 6 key spots in the report buffer instead of just using buf[2] each time.
    buf[2] = keycode;
    Serial.write(buf, 8); // Send key report
}

void releaseKey(String keyname) {
    releaseKey(getKeycode(keyname));
}
void releaseKey(int keycode) {
    // find the keycode in the report buffer, then set it to zero.
    int i=0;
    for (i=2; i<8; i++) {
        if (buf[i] == keycode) {
            buf[i] = 0;
        }
    }
    Serial.write(buf, 8); // Send key report
}
void releaseAllKeys() {
    int i=0;
    for (i=2; i<8; i++) {
        buf[i] = 0;
    }
    Serial.write(buf, 8); // Send key report
}

void pressSequenceOfKeys(const char * keySequence[], int numberOfKeys) {
    // This function can be good for pressing a few keys while holding a modifier down for example.

    int i = 0;
    for (i=0; i<numberOfKeys; i++) {
        pressKey(keySequence[i]);
        releaseKey(keySequence[i]);
    }
}

void typeLiteralString(String string) {
    char charArray[string.length()+1];
    string.toCharArray(charArray, string.length()+1);
    typeLiteralString(charArray, string.length());
}
void typeLiteralString(char string[], int stringLength) { // stringLength is the length of the printable string without considering the null byte.
    // This function will type the given string exactly as given, automatically pressing left_shift where necessary for capitals and symbols.

    // just in case:
    releaseAllKeys();
    releaseAllModifiers();

    boolean charNeedsShift = false;
    boolean shiftIsPressed = false;

    int i=0;
    for (i=0; i<stringLength; i++) {
        charNeedsShift = characterNeedsShift(string[i]);
        if (charNeedsShift && !shiftIsPressed) {
            pressModifier("left_shift");
            shiftIsPressed = true;
        }
        else if (!charNeedsShift && shiftIsPressed) {
            releaseModifier("left_shift");
            shiftIsPressed = false;
        }
        pressKey(String(string[i])); // without converting the char in string[i] to a String, arduino would prefer the pressKey(int) function instead of the pressKey(String) function, casting the char to a keycode (int) instead of a keyname (String).
        releaseKey(String(string[i])); // same as previous comment, but with releaseKey().
    }

    releaseAllModifiers();
}

boolean characterNeedsShift(char character) {
    int needsModifier = false;
    if ( // look up an ascii table and this will make sense.
        (character >= 33 && character <= 38)
        || (character >= 40 && character <= 43)
        || (character == 58)
        || (character == 60)
        || (character >= 62 && character <= 90)
        || (character >= 94 && character <= 95)
        || (character >= 123 && character <= 126)
    ) {
        needsModifier = true;
    }
    return needsModifier;
}

int getKeycode(String keyname) {
    String key = String(keyname); // Use a copy so that we don't mutate the user's String. Not sure if this is needed, but just in case. TODO: find out.
//    key.toLowerCase();

    int keycode = 0; // keycode of zero means nothing pressed.

    // non-modifier keys
    if      (key == "a") { keycode =  97; }

    else if (key == "!") { keycode = 33; }
    else if (key == "q") { keycode = 113; }
    else if (key == "a") { keycode = 97; }
    else if (key == "Z") { keycode = 90; }
    else if (key == "x") { keycode = 120; }
    else if (key == "s") { keycode = 115; }
    else if (key == "w") { keycode = 119; }
    else if (key == "2") { keycode = 50; }
    else if (key == "0") { keycode = 48; }
    else if (key == "7") { keycode = 55; }
    else if (key == "8") { keycode = 56; }
    else if (key == "3") { keycode = 51; }

    else if (key == "enter" || key == "return")     { keycode = 10; }

    return keycode;
}

//int getModifierMask(String keyname) {
//    return getModifierMask(getKeycode(keyname));
//}
int getModifierMask(int keycode) { // return value of 0 means key is not a modifier.
    int modifiermask = 0;

    // NOTE: these are not the usage keycodes like for other keys, but rather the bit masks.
    if      (keycode == 224) { modifiermask = B00000001; } // left ctrl
    else if (keycode == 225) { modifiermask = B00000010; } // left shift
    else if (keycode == 226) { modifiermask = B00000100; } // left alt
    else if (keycode == 227) { modifiermask = B00001000; } // left gui
    else if (keycode == 228) { modifiermask = B00010000; } // right ctrl
    else if (keycode == 229) { modifiermask = B00100000; } // right shift
    else if (keycode == 230) { modifiermask = B01000000; } // right alt
    else if (keycode == 231) { modifiermask = B10000000; } // right gui

    return modifiermask;
}
    
