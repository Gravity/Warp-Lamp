#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MenuSystem.h>

#define NEO_PIN 6

#define BUTTON_UP_PIN 7
#define BUTTON_LEFT_PIN 8
#define BUTTON_DOWN_PIN 12
#define BUTTON_RIGHT_PIN 13

#define DELAY_DECLARE(name) volatile unsigned long delay_variable_##name  
#define DELAY_SET(name, time) delay_variable_##name = millis() + time  
#define DELAY_DONE(name) (delay_variable_##name <= millis())  

#define OLED_RESET_PIN 4

Adafruit_SSD1306 display(OLED_RESET_PIN);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(18, NEO_PIN, NEO_GRB + NEO_KHZ800);

uint32_t warpColor = strip.Color(0,255,240);
uint32_t alertColor = strip.Color(255,0,0);

uint32_t mainColor = warpColor;
uint32_t injectorColor = strip.Color(255,255,255);

DELAY_DECLARE(warp);
uint16_t warpOffset = 0;
uint16_t warpDelay = 500;

// ClickButton upButton(BUTTON_DOWN_PIN, LOW, CLICKBTN_PULLUP);

 MenuSystem ms;
 Menu mm("Warp Lamp");

 Menu mw("Engine Speed");
 MenuItem mw_impulse("Impulse");
 MenuItem mw_warp1("Warp 1");
 MenuItem mw_warp2("Warp 2");
 MenuItem mw_warp3("Warp 3");
 MenuItem mw_warp4("Warp 4");
 MenuItem mw_warp5("Warp 5");
 MenuItem mw_warp6("Warp 6");
 MenuItem mw_warp7("Warp 7");
 MenuItem mw_warp8("Warp 8");
 MenuItem mw_warp9("Warp 9");

 // Menu ms("Settings");
 
//  MenuSystem ms;
// Menu mm("");
// MenuItem mm_mi1("Level 1 - Item 1 (Item)");
// MenuItem mm_mi2("Level 1 - Item 2 (Item)");
// Menu mu1("Level 1 - Item 3 (Menu)");
// MenuItem mu1_mi1("Level 2 - Item 1 (Item)");

//buttons
long time = 0;

int upButtonState;            
int downButtonState;
int leftButtonState;
int rightButtonState;
int lastUpButtonState = HIGH;   
int lastDownButtonState = HIGH;   
int lastLeftButtonState = HIGH;   
int lastRightButtonState = HIGH;  

long lastUpDebounceTime = 0;
long lastDownDebounceTime = 0;
long lastLeftDebounceTime = 0;
long lastRightDebounceTime = 0;

long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  mm.add_menu(&mw);
  mw.add_item(&mw_impulse, &sel_impulse);
  mw.add_item(&mw_warp1, &sel_warp_1);
  mw.add_item(&mw_warp2, &sel_warp_2);
  mw.add_item(&mw_warp3, &sel_warp_3);
  mw.add_item(&mw_warp4, &sel_warp_4);
  mw.add_item(&mw_warp5, &sel_warp_5);
  mw.add_item(&mw_warp6, &sel_warp_6);
  mw.add_item(&mw_warp7, &sel_warp_7);
  mw.add_item(&mw_warp8, &sel_warp_8);
  mw.add_item(&mw_warp9, &sel_warp_9);

  // mm.add_item(&mm_mi1, &on_item1_selected);
  // mm.add_item(&mm_mi2, &on_item2_selected);
  // mm.add_menu(&mu1);
  // mu1.add_item(&mu1_mi1, &on_item3_selected);

  ms.set_root_menu(&mm);
  displayMenu();
}

void loop() {
  checkButtonPresses();

  strip.setPixelColor(5, injectorColor);
  strip.setPixelColor(6, injectorColor);
  strip.setPixelColor(7, injectorColor);
  warp();
  //allOn();
}

void displayMenu() {
  display.clearDisplay();
  display.setCursor(0,0);
  // Display the menu
  Menu const* cp_menu = ms.get_current_menu();

  display.println(cp_menu->get_name());
  
  display.println(cp_menu->get_selected()->get_name());
  display.display();
}

void sel_impulse(MenuItem* p_menu_item)
{
  warpDelay = 600;
}

void sel_warp_1(MenuItem* p_menu_item)
{
  warpDelay = 325;
}

void sel_warp_2(MenuItem* p_menu_item)
{
  warpDelay = 275;
}

void sel_warp_3(MenuItem* p_menu_item)
{
  warpDelay = 240;
}

void sel_warp_4(MenuItem* p_menu_item)
{
  warpDelay = 215;
}

void sel_warp_5(MenuItem* p_menu_item)
{
  warpDelay = 175;
}

void sel_warp_6(MenuItem* p_menu_item)
{
  warpDelay = 140;
}

void sel_warp_7(MenuItem* p_menu_item)
{
  warpDelay = 100;
}

void sel_warp_8(MenuItem* p_menu_item)
{
  warpDelay = 50;
}

void sel_warp_9(MenuItem* p_menu_item)
{
  warpDelay = 25;
}

void checkButtonPresses()
{
// If we have gone on to the next millisecond
  if(millis() != time)
  {

    int upCurrentState = digitalRead(BUTTON_UP_PIN);
    int downCurrentState = digitalRead(BUTTON_DOWN_PIN);
    int leftCurrentState = digitalRead(BUTTON_LEFT_PIN);
    int rightCurrentState = digitalRead(BUTTON_RIGHT_PIN);

    if (upCurrentState != lastUpButtonState) {
      lastUpDebounceTime = millis();
    } 
    if (downCurrentState != lastDownButtonState) {
      lastDownDebounceTime = millis();
    } 
    if (leftCurrentState != lastLeftButtonState) {
      lastLeftDebounceTime = millis();
    } 
    if (rightCurrentState != lastRightButtonState) {
      lastRightDebounceTime = millis();
    } 
    
    if ((millis() - lastUpDebounceTime) > debounceDelay) {
      if (upCurrentState != upButtonState) {
        upButtonState = upCurrentState;

        if (upButtonState == LOW) {
          upButtonClick();
        }
      }
    }
    lastUpButtonState = upCurrentState;
    
    if ((millis() - lastDownDebounceTime) > debounceDelay) {
      if (downCurrentState != downButtonState) {
        downButtonState = downCurrentState;

        if (downButtonState == LOW) {
          downButtonClick();
        }
      }
    }
    lastDownButtonState = downCurrentState;
    
    if ((millis() - lastLeftDebounceTime) > debounceDelay) {
      if (leftCurrentState != leftButtonState) {
        leftButtonState = leftCurrentState;

        if (leftButtonState == LOW) {
          leftButtonClick();
        }
      }
    }
    lastLeftButtonState = leftCurrentState;
    
    if ((millis() - lastRightDebounceTime) > debounceDelay) {
      if (rightCurrentState != rightButtonState) {
        rightButtonState = rightCurrentState;

        if (rightButtonState == LOW) {
          rightButtonClick();
        }
      }
    }
    lastRightButtonState = rightCurrentState;

    time = millis();
  }
}

void upButtonClick()
{
  ms.prev();
  Serial.println("up");
  displayMenu();
}

void leftButtonClick()
{
  ms.back();
  Serial.println("left");
  displayMenu();
}

void downButtonClick()
{
  ms.next();
  Serial.println("down");
  displayMenu();
}

void rightButtonClick()
{
  ms.select();
  Serial.println("right");
  displayMenu();
}

void allOn()
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, mainColor);
  }

  strip.show();
}

void warp()
{

    if(DELAY_DONE(warp)){

        if(warpOffset != 5){
            strip.setPixelColor(warpOffset, mainColor);
            strip.setPixelColor(17 - warpOffset, mainColor);
            strip.setPixelColor(12 - warpOffset, mainColor);
        }
        for(uint16_t j=0; j<5; j++) {
            if(warpOffset != j){
                strip.setPixelColor(j, 0);
            }
        }

        for(uint16_t j=0; j<5; j++) {
            if(17 - warpOffset != 17-j){
                strip.setPixelColor(17-j, 0);
            }
        }

        for(uint16_t j=0; j<5; j++) {
            if(12 - warpOffset != 12-j){
                strip.setPixelColor(12-j, 0);
            }
        }

        strip.show();
        // delay(300);

        DELAY_SET(warp, warpDelay);

        if(warpOffset < 5){
            warpOffset++;
        } else {
            warpOffset = 0;
        }
    }
    // }
}
