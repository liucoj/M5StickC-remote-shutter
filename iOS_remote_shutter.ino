#include <BleKeyboard.h>

BleKeyboard bleKeyboard("BLEshutter");

// check battery
unsigned long nextVbatCheck = 0;
// check connection
bool Connected = false;
// Get Battery level
int getVlevel() {
  float vbat = M5.Axp.GetBatVoltage();
  int vlevel = ( vbat - 3.2 ) / 0.8 * 100;
  if ( vlevel < 0 ) {
    vlevel = 0;
  }
  if ( 100 < vlevel ) {
    vlevel = 100;
  }
  return vlevel;
}

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(9);
  setCpuFrequencyMhz(80);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.println("BLE Shutter");
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.println("BtnA :Shutter");
  M5.Lcd.println("     LiuC0j");

  // Battery level can only be set at startup
  bleKeyboard.setBatteryLevel(getVlevel());
  bleKeyboard.begin();
}

void loop() {
  // Button state update
  M5.update();
  M5.Lcd.setCursor(0, 0);
  // キー操作
  if (bleKeyboard.isConnected()) {
    if (Connected==false){
      M5.Lcd.fillScreen(GREEN);
      M5.Lcd.printf("Connected");
      Connected = true;      
    }
    if ( M5.BtnA.wasPressed() ) {
      // Send volume up key
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }
  } else {
    if (Connected){
      Connected = false;
      M5.Lcd.fillScreen(RED);
      M5.Lcd.printf("Disconnect");
    }
  }

  // Battery level update (every minute)
  if (nextVbatCheck < millis()) {
    M5.Lcd.setCursor(112, 0);
    M5.Lcd.printf("%3d%%", getVlevel());

    nextVbatCheck = millis() + 60000;
  }
  // Wait
  delay(100);
}
