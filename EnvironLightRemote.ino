// This #include statement was automatically added by the Particle IDE.
#include "Libraries/neopixel.h"
#include "application.h"

//SerialDebugOutput debugOutput;          // default is 9600 and log everything


SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

#define PIXEL_PIN D6
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


int gotMessage;

double tmp = 0.0;

struct setLeds;

struct LedLevels{
  float tmp;
  int stripPercentage;
  byte red;
  byte green;
  byte blue;
};


LedLevels ledLevels[]{
  { 32.5 , 100, 255,   0,   0}, // C to F 90.5, RGB: RED
  { 26.69,  90, 255, 128,   0}, // C to F 80.042, RGB: ORANGE
  { 21.38,  80, 204, 255,   0}, // C to F 70.484, RGB: YELLOW
  { 15.83,  70,  0, 255,   0}, // C to F 90.5, RGB: GREEN
  { 10.27,  60,  0,   0, 255}, // C to F 90.5, RGB: BLUE
  {  4.47,  50,  0, 255, 255}, // C to F 90.5, RGB: CYAN
  {  0.27,  40, 153,  51, 255}, // C to F 90.5, RGB: MAGENTA
  {-99.0 ,  30, 255, 255, 255}  // CAPTURE ALL THE REST RGB: WHITE
};

void myHandler(const char *event, const char *data)
{
    tmp = atof(data);  // convert the incoming string literal to float and put the value in the global variable
    gotMessage = true; // a global semaphore to set true when you get a new light value from master Particle.subscribe() function
    Particle.publish("GotIt", String(tmp));

}


bool gotTemperature = false; // a global semaphore to set true when you get a new light value from master Particle.subscribe() function...

void setup()
{
  Serial.begin(9600);
  Particle.variable("TempCpub", &tmp, DOUBLE);
  Particle.subscribe("TempCpub", myHandler, MY_DEVICES);
  while(!Serial.available() && millis() < 60000)  Particle.process();    // wait for serial terminal or 60sec timeout


  strip.begin(); // Initialize the strip
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(15); // Set brightness limit (value between 0-255)
  setLeds(tmp);
}

void loop()
{
  if(gotMessage)
  {
    setLeds(tmp);
    gotMessage = false;
  }
}


void setLeds(double tmp)
{
  for(int i = 0; i < sizeof(ledLevels)/sizeof(ledLevels[0]); i++)
  {
    if(tmp > ledLevels[i].tmp)
    {
      for (int j = 0; j < PIXEL_COUNT; j++)
      {
        if(j < (int(ledLevels[i].stripPercentage  / 100.0 * PIXEL_COUNT)))
        {
          strip.setPixelColor(j, ledLevels[i].red, ledLevels[i].green, ledLevels[i].blue);
        }
        else
        {
          strip.setPixelColor(j, 0, 0, 0);
        }
      }
      strip.show();
      return;
    }
  }
}
