#include <Adafruit_GFX.h>    // include Adafruit graphics library
#include <Adafruit_ST7735.h> // include Adafruit ST7735 display library
#include <SPI.h>             // include Arduino SPI library
#include <SD.h>              // include Arduino SD library
 
////////////////TFT initialisation//////////////////////////////////
#define TFT_RST  15//8   // (RESET) reset line (-1 if not used)
#define TFT_CS   4//10  // (CS) tft chip select
#define TFT_DC   17//9   // (A0) tft data/command
#define TFT_MOSI 23//11  // (SDA) tft mosi
#define TFT_SCLK 18//13  // (SCK) tft clk
#define CS_SD    21//4   // SD chip select

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define led_verte 32//A0
#define led_rouge 33//A1
#define led_blanche 25//A2

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x37E6//0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void TFT_init(){
  digitalWrite(led_blanche, HIGH);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  digitalWrite(led_blanche, LOW);
}

void SD_init(){
  digitalWrite(led_blanche, HIGH);
  Serial.println(""); Serial.print("Initializing SD card ");
  if (!SD.begin(CS_SD)) {
    Serial.println("FAILED");
    digitalWrite(led_rouge, HIGH);
  }
  else{
    Serial.println("OK. Every available file :");
    File root = SD.open("/");  // open SD card main root
    printDirectory(root, 0);   // print all files names and sizes
    root.close();              // close the opened root
  }
  digitalWrite(led_blanche, LOW);
}

void setup(void) {
  Serial.begin(115200);
  pinMode(led_verte, OUTPUT);
  pinMode(led_rouge, OUTPUT);
  pinMode(led_blanche, OUTPUT);
    
  TFT_init();
  SD_init();
}
 
void loop() {
  Serial.println("");
  bmpDraw("/mod1G.bmp",0,0);
  tft.drawRect(12,114, 104,34,BLACK);
  tft.drawRect(13,115, 102,32,BLACK);
  tft.drawRect(14,116, 100,30,BLACK);
  delay(250);
  for(int i=0; i<=10; i++){
    tft.fillRect(14,116, 10*i,30, GREEN);
    tft.drawRect(14,116, 100,30,BLACK);
    delay(250);
  }
  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  
  long int t=millis();
  draw_mode(1, BLUE);
  draw_mode(2, BLUE);
  draw_mode(3, BLUE);
  draw_mode(4, BLUE);
  Serial.println(millis()-t);
  delay(500);
  draw_mode(1, GREEN);
  delay(500);
  draw_mode(1, BLUE);
  draw_mode(2, GREEN);
  delay(500);
  draw_mode(2, BLUE);
  draw_mode(3, GREEN);
  delay(500);
  draw_mode(3, BLUE);
  draw_mode(4, GREEN);
  delay(500);
  
  //TEST 0
  Serial.println("");
  t=millis();
  tft.fillScreen(ST77XX_BLACK);
  Serial.println(millis()-t);
  
  //TEST01
  t=millis();
  tft.fillRect(14, 5 , 99, 5+32, ST77XX_RED);
  tft.drawRect(14, 5 , 99, 5+32, ST77XX_BLUE);
  
  tft.fillRect(14, 43 , 99, 43+32, ST77XX_RED);
  tft.drawRect(14, 43 , 99, 43+32, ST77XX_BLUE);
  
  tft.fillRect(14, 83 , 99, 83+32, ST77XX_RED);
  tft.drawRect(14, 83 , 99, 83+32, ST77XX_BLUE);
  
  tft.fillRect(14, 121 , 99, 121+32, ST77XX_RED);
  tft.drawRect(14, 121 , 99, 121+32, ST77XX_BLUE);
  Serial.println(millis()-t);
  delay(2500); 
  //TEST 1
  bmpDraw("Wallee.bmp",0,0);
  delay(500);
  bmpDraw("MOD1G.bmp",14,5);
  delay(1000);
  bmpDraw("MOD2G.bmp",14,43);
  delay(1000);
  bmpDraw("MOD3G.bmp",14,83);
  delay(1000);
  bmpDraw("MOD4G.bmp",14,121);
  delay(1000);
  
  //TEST 2
  bmpDraw("menu.bmp",0,0);
  delay(1000);
  bmpDraw("MOD1B.bmp",14,5);
  bmpDraw("MOD2G.bmp",14,43);
  delay(1000);
  bmpDraw("MOD2B.bmp",14,43);
  bmpDraw("MOD3G.bmp",14,83);
  delay(1000);
  bmpDraw("MOD3B.bmp",14,83);
  bmpDraw("MOD4G.bmp",14,121);
  delay(1000);
  
}

void draw_mode(int numero, int couleur){
  int y1=4;
  int y2=43;
  int y3=82;
  int y4=121;
  tft.setTextSize(2);
  if(couleur==GREEN){ tft.setTextColor(BLACK); }
  else {              tft.setTextColor(WHITE); }
  switch(numero){
    case 1:
      tft.setCursor(29,y1+11);
      tft.fillRoundRect(14,y1,  100,37,  10, couleur);
      tft.drawRoundRect(14,y1,  100,37,  10, WHITE);
      tft.print("Mode 1");
      break;
    case 2:
      tft.setCursor(29,y2+11); //16+38=54   
      tft.fillRoundRect(14,y2,  100,37,  10, couleur);
      tft.drawRoundRect(14,y2,  100,37,  10, WHITE);
      tft.print("Mode 2");      
      break;  
    case 3:
      tft.setCursor(29,y3+11);
      tft.fillRoundRect(14,y3,  100,37,  10, couleur);
      tft.drawRoundRect(14,y3,  100,37,  10, WHITE);
      tft.print("Mode 3");      
      break;  
    case 4:
      tft.setCursor(29,y4+11);
      tft.fillRoundRect(14,y4,  100,37,  10, couleur);
      tft.drawRoundRect(14,y4,  100,37,  10, WHITE);
      tft.print("Mode 4");      
      break;  
  }
}

 
// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.
 
#define BUFFPIXEL 50
 
void bmpDraw(char *filename, uint8_t x, uint16_t y) {
 
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
 
  if((x >= tft.width()) || (y >= tft.height())) return;
 
  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
 
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }
 
  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
 
        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);
 
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;
 
        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }
 
        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;
 
        // Set TFT address window to clipped image bounds
        tft.startWrite();
        tft.setAddrWindow(x, y, w, h);
 
        for (row=0; row<h; row++) { // For each scanline...
 
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            tft.endWrite();
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }
 
          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
              tft.startWrite();
            }
 
            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        tft.endWrite();
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }
 
  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}
 
 
// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
 
uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}
 
uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
 
 
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
 
