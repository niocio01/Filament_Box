#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <Button2.h>
#include "esp_adc_cal.h"
//#include "bmp.h"
#include "display.h"

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

char buff[512];
int vref = 1100;
int btnCick = false;


// #define ENABLE_SPI_SDCARD

//Uncomment will use SDCard, this is just a demonstration,
//how to use the second SPI
#ifdef ENABLE_SPI_SDCARD

    #include "FS.h"
    #include "SD.h"

    SPIClass SDSPI(HSPI);

    #define MY_CS       33
    #define MY_SCLK     25
    #define MY_MISO     27
    #define MY_MOSI     26

    void setupSDCard(void)
    {
        SDSPI.begin(MY_SCLK, MY_MISO, MY_MOSI, MY_CS);
        //Assuming use of SPI SD card
        if (!SD.begin(MY_CS, SDSPI)) {
            Serial.println("Card Mount Failed");
            tft.setTextColor(TFT_RED);
            tft.drawString("SDCard Mount FAIL", tft.width() / 2, tft.height() / 2 - 32);
            tft.setTextColor(TFT_GREEN);
        } else {
            tft.setTextColor(TFT_GREEN);
            Serial.println("SDCard Mount PASS");
            tft.drawString("SDCard Mount PASS", tft.width() / 2, tft.height() / 2 - 48);
            String size = String((uint32_t)(SD.cardSize() / 1024 / 1024)) + "MB";
            tft.drawString(size, tft.width() / 2, tft.height() / 2 - 32);
        }
    }
#else
    #define setupSDCard()
#endif

void showVoltage(void)
{
    static uint64_t timeStamp = 0;
    if (millis() - timeStamp > 1000) {
        timeStamp = millis();
        uint16_t v = analogRead(ADC_PIN);
        float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
        String voltage = "Voltage :" + String(battery_voltage) + "V";
        Serial.println(voltage);
        tft.fillScreen(TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.drawString(voltage,  tft.width() / 2, tft.height() / 2 );
    }
}

void button_init(void)
{
    btn1.setLongClickHandler([](Button2 & b) {
        btnCick = false;
        int r = digitalRead(TFT_BL);
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Press again to wake up",  tft.width() / 2, tft.height() / 2 );
        delay(6000);
        digitalWrite(TFT_BL, !r);

        tft.writecommand(TFT_DISPOFF);
        tft.writecommand(TFT_SLPIN);
        //After using light sleep, you need to disable timer wake, because here use external IO port to wake up
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
        // esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);
        delay(200);
        esp_deep_sleep_start();
    });
    btn1.setPressedHandler([](Button2 & b) {
        Serial.println("Detect Voltage..");
        btnCick = true;
    });

    btn2.setPressedHandler([](Button2 & b) {
        btnCick = false;
        Serial.println("btn press wifi scan");
    });
}

void button_loop(void)
{
    btn1.loop();
    btn2.loop();
}

void display_init(void)
{
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_RED);

    if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
         pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
         digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }
    

    
    tft.setTextSize(20);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1); 

    tft.drawString("Hello World!",  tft.width() / 2, tft.height() / 2 );

    Serial.println("display init");

    //showVoltage();
}
