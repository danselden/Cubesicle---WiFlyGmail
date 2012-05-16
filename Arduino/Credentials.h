#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

// supply your own Pachube feed ID
//#define PACHUBEFEED "36718"
// this API key will only work from my IP address - you need to supply your own
#define APIKEY "APIKEY FOR PACHUBE"
#define TIMETOUPDATE 15000  // frequency of update - every 15 seconds

// Wifi parameters
char passphrase[] = "PASSWORD";
char ssid[] = "SSID";
boolean mode = WPA_MODE; //or WEP_MODE

#endif
