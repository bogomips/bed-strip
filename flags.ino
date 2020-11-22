void italy() {
     
    long int flagColors[3] = { 0x00ff00,  0xFFFFFF, 0xFF0000 };
    int index = 0;    

    for(int i=0; i< NUMPIXELS; i++) { 
        
        if (i > 0 && i % (NUMPIXELS/3) == 0) {
            index++;
            //Serial.print(i);Serial.print(" --- ");Serial.println(index);Serial.print(" --- ");Serial.println(flagColors[index]);
        }
        
        stripConfig.strip.setPixelColor(i, flagColors[index]);
    }

    stripConfig.strip.show();
    
}

void greece() {
     
    long int flagColors[3] = { 0xFFFFFF, 0x0D5EAF };
    int index = 0;    

    for(int i=0; i< NUMPIXELS; i++) { 
        
        if (i > 0 && i % (NUMPIXELS/2) == 0) {
            index = (index > 0) ? 0 : 1;
            //Serial.print(i);Serial.print(" --- ");Serial.println(index);Serial.print(" --- ");Serial.println(flagColors[index]);
        }
        
        stripConfig.strip.setPixelColor(i, flagColors[index]);
    }

    stripConfig.strip.show();
    
}

void germany() {
     
    long int flagColors[3] = { 0x000000, 0xDD0000, 0xFFCE00 };
    int index = 0;    

    for(int i=0; i< NUMPIXELS; i++) { 
        
        if (i > 0 && i % (NUMPIXELS/3) == 0) {
            index++;
            //Serial.print(i);Serial.print(" --- ");Serial.println(index);Serial.print(" --- ");Serial.println(flagColors[index]);
        }
        
        stripConfig.strip.setPixelColor(i, flagColors[index]);
    }

    stripConfig.strip.show();
    
}

