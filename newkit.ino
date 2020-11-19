void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  //RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  //OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  //CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  //LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  //RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  //OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  //CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i =((NUMPIXELS-EyeSize)/2); i>=0; i--) {
    whole_strip(0x000000);
    
    stripConfig.strip.setPixelColor(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      stripConfig.strip.setPixelColor(i+j, red, green, blue); 
    }
    stripConfig.strip.setPixelColor(i+EyeSize+1, red/10, green/10, blue/10);
    
    stripConfig.strip.setPixelColor(NUMPIXELS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      stripConfig.strip.setPixelColor(NUMPIXELS-i-j, red, green, blue); 
    }
    stripConfig.strip.setPixelColor(NUMPIXELS-i-EyeSize-1, red/10, green/10, blue/10);
    
    stripConfig.strip.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i<=((NUMPIXELS-EyeSize)/2); i++) {
    whole_strip(0x000000);
    
    stripConfig.strip.setPixelColor(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      stripConfig.strip.setPixelColor(i+j, red, green, blue); 
    }
    stripConfig.strip.setPixelColor(i+EyeSize+1, red/10, green/10, blue/10);
    
    stripConfig.strip.setPixelColor(NUMPIXELS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      stripConfig.strip.setPixelColor(NUMPIXELS-i-j, red, green, blue); 
    }
    stripConfig.strip.setPixelColor(NUMPIXELS-i-EyeSize-1, red/10, green/10, blue/10);
    
    stripConfig.strip.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i < NUMPIXELS-EyeSize-2; i++) {
    whole_strip(0x000000);
    stripConfig.strip.setPixelColor(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      stripConfig.strip.setPixelColor(i+j, red, green, blue); 
    }
    stripConfig.strip.setPixelColor(i+EyeSize+1, red/10, green/10, blue/10);
    stripConfig.strip.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUMPIXELS-EyeSize-2; i > 0; i--) {
    whole_strip(0x000000);
    stripConfig.strip.setPixelColor(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      stripConfig.strip.setPixelColor(i+j, red, green, blue); 
    }
    stripConfig.strip.setPixelColor(i+EyeSize+1, red/10, green/10, blue/10);
    stripConfig.strip.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}