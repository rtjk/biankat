
////////////////////////////////////////////////////////////////////////////////
//////////NETWORK///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <WiFiClient.h>

#define CONNECTION_ATTEMPTS_MAX 0 //////////////////////////////////////////////

const char* ssid = "Triskarone";
const char* password = "triskarone";
const char* server_ip = "192.168.1.103";
int server_port = 8090;

int connection_attempts = 0;

static WiFiClient client;

////////////////////////////////////////////////////////////////////////////////
//////////FUNCTIONS/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
