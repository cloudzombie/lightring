// This #include statement was automatically added by the Particle IDE.
#include "Libraries/neopixel.h"

#include "application.h"


SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);


#define PIXEL_PIN D6
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


double tmp;

int gotMessage;

struct LedLevels{
  float tmp;
  int stripPercentage;
  byte color[3];
};

LedLevels ledLevels[]{
  { 32.5 , 100, 255,   0,   0},
  { 26.69,  90, 255, 128,   0},
  { 21.38,  80, 204, 255,   0},
  { 15.83,  70,   0, 255,   0},
  { 10.27,  60,   0,   0, 255},
  {  4.47,  50,   0, 255, 255},
  {  0.27,  40, 153,  51, 255},
  {-99.0 ,  30, 255, 255, 255}  // capture all the rest
};



void myHandler(const char *event, const char *data)
{

tmp = atof(data);  // convert the incoming string literal to float and put the value in the global variable
gotMessage = true; // a global semaphore to set true when you get a new light value from master Particle.subscribe() function

}

void setup() {

  Serial.begin(9600);
  Particle.variable("TempCpub", &tmp, DOUBLE);
  Particle.subscribe("TempCpub", myHandler, MY_DEVICES);
  while(!Serial.available() && millis() < 60000) Particle.process();    // wait for serial terminal or 60sec timeout

  strip.begin(); // Initialize the strip
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(25); // Set brightness limit (value between 0-255)

}


void loop()
{

  if(gotMessage)
  {
      setLeds(tmp);
        gotMessage = false;
  }
}

void setLeds(float tmp)
{
  for(int i = 0, i < sizeof(ledLevels)/sizeof(ledLevels[0]); i++)
  {
    if(temp > ledLevels[i].tempC)
    {
      for (int j = 0, j < NUM_PIXELS; j++)
      {
        if(j < int(ledLevels[i].stripPercentage  / 100.0 * NUM_PIXELS))
        {
          strip.setPixelColor(j, strip.color(ledLevels[i].color));
        }
        else
        {
          strip.setPixelColor(j, strip.color((0,0,0));
        }
      strip.show();
      return;
      }
    }
  }
}
