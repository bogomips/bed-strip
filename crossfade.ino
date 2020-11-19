void crossFade(uint32_t color,uint32_t current_color) {

  int oR = current_color >> 16;
  int oG = (current_color & 0x00ff00) >> 8;
  int oB = (current_color & 0x0000ff);
  
  int nR = color >> 16;
  int nG = (color & 0x00ff00) >> 8;
  int nB = (color & 0x0000ff);
  
  int tR = oR;
  int tG = oG;
  int tB = oB;
  
  int stepR = calculateStep(oR, nR);
  int stepG = calculateStep(oG, nG); 
  int stepB = calculateStep(oB, nB);
  
  for (int i = 0; i <= 600; i++) {
   
    tR = calculateVal(stepR, tR, i);
    tG = calculateVal(stepG, tG, i);
    tB = calculateVal(stepB, tB, i);

    for (int y=0; y < 144; y++) {
      stripConfig.strip.setPixelColor(y, tR, tG ,tB);
    }
    stripConfig.strip.show(); 

    //delay(1);

  }

    
   //Serial.printf("RGB : %i %i %i\n",  nR, nG ,nB);
  

  //ColorConverter::HslToRgb(H/360, S/100, L/100, red,green,blue);
  //whole_strip(color)

  current_color = color;
  
}

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 600/step;              //   divide by 1020
  } 
  return step;
}

/* The next function is calculateVal. When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1. 
*  (R, G, and B are each calculated separately.)
*/

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}