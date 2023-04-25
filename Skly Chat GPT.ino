#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int val;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

int button_1 = 7;
int button_2 = 6;
int button_3 = 5;
int button_4 = 3;

String textBuffer = "";//"For almost any person, there is nothing more important in the world than their family. I love my family too. Today I would like to tell you about them. My mother’s name is Anna, she is a teacher. She has been working in our local school for a long time.";
String textToSend = "";
int textOffset = 0;
int mode = 0; // 0 - чтение текста, 1 - отправка текста
int currentCharIndex = 0;
String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.!? <>";


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT); // 13 пин – светодиод, объявляется как выход
  pinMode(button_1, INPUT); // объявляем пин 2 как вход
  pinMode(button_2, INPUT); // объявляем пин 2 как вход
  pinMode(button_3, INPUT); // объявляем пин 2 как вход
  pinMode(button_4, INPUT); // объявляем пин 2 как вход

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
}

void loop() {

if (Serial.available() > 0) {
      textBuffer += (char)Serial.read();
      testdrawstyles(textBuffer.substring(textOffset, textOffset + 84));
      mode = 0;
    }
 if (digitalRead(button_4) == HIGH) {
    delay(300);
    mode = 1 - mode;
    textOffset = 0;
    currentCharIndex = 0;
    //display.clearDisplay();
    if(mode==0){
      testdrawstyles(textBuffer.substring(textOffset, textOffset + 84));
    } else{
      testdrawstyles(String(textToSend + characters.charAt(currentCharIndex)));
    }
    textBuffer = "";
}

  if (mode == 0) {

    if (digitalRead(button_1) == HIGH) {
      delay(100);
      if (textOffset >= 84) {
        textOffset -= 84;
      }
      testdrawstyles(textBuffer.substring(textOffset, textOffset + 84));
    }

    if (digitalRead(button_2) == HIGH) {
      delay(100);
      if (textOffset < textBuffer.length() - 84) {
        textOffset += 84;
      }
      testdrawstyles(textBuffer.substring(textOffset, textOffset + 84));
    }
    //lcd.setCursor(0, 0);
    
    //delay(500);
  } else {
    if (digitalRead(button_1) == HIGH) {
      delay(100);
      currentCharIndex--;
      if (currentCharIndex < 0) {
        currentCharIndex = characters.length() - 1;
      }
    }

    if (digitalRead(button_2) == HIGH) {
      delay(100);
      currentCharIndex++;
      if (currentCharIndex >= characters.length()) {
        currentCharIndex = 0;
      }
    }

    if (digitalRead(button_3) == HIGH) {
      delay(300);
      if (currentCharIndex == characters.length() - 1) {
        Serial.print(textToSend+'\r');
        textToSend = "";
      } else if (currentCharIndex == characters.length() - 2) {
        if(currentCharIndex>0){
          textToSend = textToSend.substring(0, textToSend.length() - 1);
          //currentCharIndex--;
        }

      } 
      else {
        textToSend += characters.charAt(currentCharIndex);
      }
    }

    //lcd.setCursor(0, 0);
    //testdrawstyles("To send: ");
    //lcd.setCursor(0, 1);
    testdrawstyles(String(textToSend + characters.charAt(currentCharIndex)));
  }

  delay(100);
 
}

void testdrawstyles(String str) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(str);

  display.display();
  
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world my name is Skly, How are you?! I am fine, what about you? I am okay, can you told me about yourself?"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}
