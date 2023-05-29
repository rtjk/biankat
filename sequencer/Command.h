
#include "Network.h"

// reactions with high priority
#define M1_LENGHT 24 //                             SMS                                                                                                             IMPORTANT         CONTROL
String messages_1[M1_LENGHT] =    {"GF",    "GG",   "G1",   "G2",   "G3",   "G4",   "G5",   "G6",   "G7",   "G8",   "G9",   "GA",   "GB",   "GC",   "GD",   "GE",   "7G",     "7D",   "X1",   "X2",       "X3",    "X4",     "X5",           "X6"   };
String reactions_1[M1_LENGHT] =   {"",      "",     "face", "face", "face", "face", "face", "face", "face", "face", "face", "face", "smsa", "smsb", "face", "face", "angry",  "sad",  "stop", "",         "happy", "angry",  "embarrassed", "sad"   };
int who_1[M1_LENGHT] =            {6,       6,      1,       1,      2,      2,      3,      3,      4,      4,      5,      5,      6,      6,      7,      7,      7,       7,      6,      6,          6,       6,         6,             6};

// reactions with low priority
#define M2_LENGHT 25 //            SOMEONE IS HAPPY                                  SOMEONE IS SAD                        ROCCO                                             SOMEONE IS ANGRY
String messages_2[M2_LENGHT] =    {"1B","2B","3B","4B","5B","7B",                    "1E","2E","3E","4E","5E","7E",        "1B","1C","1E","1F","1H","1K",                    "1C","2C","3C","4C","5C","7C"};
String reactions_2[M2_LENGHT] =   {"happy","happy","happy","happy","happy","happy",  "sad","sad","sad","sad","sad","sad",  "angry","angry","angry","angry","angry","angry",  "embarrassed","embarrassed","embarrassed","embarrassed","embarrassed","embarrassed"};
int who_2[M2_LENGHT] =            {1,2,3,4,5,7,                                      1,2,3,4,5,7,                          1,1,1,1,1,1,                                      1,2,3,4,5,7};

int facing = 6;
int facing_next = 6;
int ys[7] = {96, 112, 129, 146, 163, 90, 17}; // angles of the other robots

bool performing_emotion = false;
String next_emotion = "";
String command = "";
int who_to_face = 6;

int pause_reception_flag = 0;
int pause_reception_delay = 5000;
int pause_reception_previous_millis = 0;

int reading_flag = 0;
String emotion_after_reading = "";

int send_message_toward = 0;


void pause_reception(int new_pause_reception_delay) {
  pause_reception_delay = new_pause_reception_delay;
  pause_reception_flag = 1;
}

void parse_message(String new_message) {
  if(new_message.length() == 4) {
    new_message.remove(2);
  }
  if(new_message.length() == 2) {
    for(int i=0; i<M1_LENGHT; i++) {
      if(new_message == messages_1[i]) {
        next_emotion = reactions_1[i];
        send_message_toward = who_1[i];
        facing_next = who_1[i];
        performing_emotion = false;
        Serial.print("> ");
        Serial.println(new_message);
        return;
      }
    }
    for(int i=0; i<M2_LENGHT; i++) {
      if(new_message == messages_2[i]) {
        next_emotion = reactions_2[i];
        send_message_toward = who_2[i];
        facing_next = who_2[i];
        Serial.print("> ");
        Serial.println(new_message);
        return;
      }
    }
  }
  Serial.print("? ");
  Serial.println(new_message);
}

void receive_message() {
  if (client.available()) {
    parse_message(client.readStringUntil('\n'));
  }
  if (Serial.available()) {
    parse_message(Serial.readStringUntil('\n'));
  }
}

void send_message(String new_message) {
  if(new_message.length() == 4 && send_message_toward != 6) {
    new_message[2] = String(send_message_toward)[0];
    send_message_toward = 0;
  }
  new_message += '\n';
  client.print(new_message);
  Serial.print("< ");
  Serial.println(new_message);
}

////////////////////////////////////////////////////////////////////////////////
//////////NETWORK///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup_network() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && connection_attempts < CONNECTION_ATTEMPTS_MAX) {
    delay(1000);
    connection_attempts++;
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  connection_attempts = 0;
  Serial.println("Connecting to server");
  while (!client.connect(server_ip, server_port) && connection_attempts < CONNECTION_ATTEMPTS_MAX) {
    delay(1000);
    connection_attempts++;
    Serial.print(".");
  }
  Serial.println();
}

void check_connection() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("connection lost");
    setup_network();
  }
  if(!client.connected()) { //////////////////////////////////////////
    Serial.println("connection lost");
    setup_network();
  }
}

