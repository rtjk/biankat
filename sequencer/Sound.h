
////////////////////////////////////////////////////////////////////////////////
//////////SOUND/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define DFPLAYER_PIN_RX 17
#define DFPLAYER_PIN_TX 16

#define VOLUME 30 // 0 -> 30

HardwareSerial my_serial(1);
DFRobotDFPlayerMini player;

void setup_sound() {
  my_serial.begin(9600, SERIAL_8N1, DFPLAYER_PIN_RX, DFPLAYER_PIN_TX); // baud rate, mode, RX, TX
  Serial.print("Connecting to dfplayer");
  while (!player.begin(my_serial)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to dfplayer");
  player.setTimeOut(500);
  player.volume(VOLUME);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.outputDevice(DFPLAYER_DEVICE_SD);
  player.play(1);
}
