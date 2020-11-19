void disco(int delayms) {

    uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;

    //whole_strip(0x000000);
    for(double hue = 0; hue <= 360; hue+=30) {

        ColorConverter::HslToRgb(hue/360, 1, 0.5, red, green, blue);

        for (int i=0; i <= NUMPIXELS; i++) {
            stripConfig.strip.setPixelColor(i,red, green, blue);     
        }
        stripConfig.strip.show(); 
        whole_strip(0x000000);
        //stripConfig.strip.show();  

        delay(delayms);

    }
}

void strobo(int delayms) {

        whole_strip(0xFFFFFF);
        delay(delayms);
        whole_strip(0x000000);
        delay(delayms);
}

