
#ifndef ESP8266_H_
#define ESP8266_H_

#include <avr/pgmspace.h>  // Needed for PROGMEM stuff

#define ALPHA   ((float)1.0)
#define BG_R    (50)
#define BG_G    (130)
#define BG_B    (170)
#define Z_ALPHA(fg, bg, alpha)     ((float)fg) * alpha + (1.0f - alpha) * ((float)bg)
// bg color --> R: 53 G: 133 B: 166 --> 0x3585A6
// selected --> 23, 66,83 - 0x174253

// -----------------------  UI Elements for weather ------------------------------

#define CMD_BEG "AT+"
#define CMD_END "\r\n"
#define Z_MAKECMD(CMD)        CMD_BEG #CMD CMD_END

#if 0
#define TEST                  "AT\r\n"  
#define ECHO_OFF              "ATE0" CMD_END            // resp: 0d 0a 4f 4b 0d 0a  --> ..OK..
#define SET_MODE_AP_STATION   Z_MAKECMD(CWMODE=3)
#define NOMUX                 Z_MAKECMD(CIPMUX=0)
#define TCPSEND               Z_MAKECMD(CIPSEND)   // resp is a > prompt
#define TCPCLOSE              Z_MAKECMD(CIPCLOSE)
#define OPENTCP               "AT+CIPSTART=\"TCP\",\""   // "iot.espressif.cn", 8000
#define CONNECT               "AT+CWJAP_CUR=\"" // AT+CWJAP_CUR="G4_6095","12345678" --> WIFI CONNECTED WIFI GOT IP OK
#define EXITPASSTHRU          "+++"
#endif


const char PASSTHRU[] PROGMEM =                       {Z_MAKECMD(CIPMODE=1)};
const char TCPSEND[] PROGMEM =                        {Z_MAKECMD(CIPSEND)};
const char TCPCLOSE[] PROGMEM =                       {Z_MAKECMD(CIPCLOSE)};
const char NOMUX[] PROGMEM =                          {Z_MAKECMD(CIPMUX=0)};
const char SET_MODE_AP_STATION[] PROGMEM =            {Z_MAKECMD(CWMODE=3)};
const char OPENTCP[] PROGMEM =                        {"AT+CIPSTART=\"TCP\",\""};
const char CONNECT[] PROGMEM =                        {"AT+CWJAP=\""};
const char ECHO_OFF[] PROGMEM =                       {"ATE0" CMD_END};
const char TEST[] PROGMEM =                           {"AT\r\n"};
const char EXITPASSTHRU[] PROGMEM =                   {"+++"};
const char UARTCONFIG[] PROGMEM =                     {"AT+UART=115200,8,1,0,0\r\n"};
const char RESET[] PROGMEM =                          {Z_MAKECMD(RST)};
const char GMR[] PROGMEM =                            {"AT+GMR\r\n"};



#define OK                    "OK"
#define ERR                   "ERROR"
#define WIFI_GOT_IP           "WIFI GOT IP"
#define ALREADY_CONNECT       "ALREADY CONNECT"
#define NO_CHANGE             "no change"
#define READY                 "ready"

#define ESP_READ_TIMEOUT  ((uint16_t)1000u)
#if 0
#define SSID  "G4_6095"
#define PWD   "12345678"
#define HOST  "api.openweathermap.org"
#define PORT  "80"
#endif 

//const PROGMEM char SSID[] = {"G4_6095"};
//const PROGMEM char PWD[]  = {"12345678"};
//const PROGMEM char SSID[] = {"ihome_belkin"};
//const PROGMEM char PWD[]  = {"12345678"};
const PROGMEM char HOST[] = {"api.openweathermap.org"};
const PROGMEM char TCP_PORT[] = {"80"};

//const PROGMEM char h1[] = { "GET http://api.openweathermap.org/data/2.5/forecast?units=metric&cnt=4&q=" };
const PROGMEM char h2[] = { "&APPID=" };
//const PROGMEM char Location[] = "Singapore";
// Open Weather Map public key:
const PROGMEM char owmAPIKey[] = "dbbbde4905b735cbf7c09835af71bdac";
const PROGMEM char h3[] = { " HTTP/1.1\r\n" "User-Agent: Fiddler\r\n" "Host: api.openweathermap.org\r\n\r\n" };

typedef  void (*WriteHook) (uint16_t len, uint8_t* buf);
typedef uint16_t (*ReadAvailableHook) (uint8_t* buf);
typedef uint16_t (*ReadHook) (uint16_t len, uint8_t* buf);


class Timeout {
private:
	unsigned long TS;

public:
	Timeout() { TS = millis(); }

	bool IsExpired(const unsigned long interval) {
		if (((millis() - TS) & ~0UL) > interval)
			return true;
		else
			return false;
	}

	void Reset(unsigned long ts) {
		TS = ts;
	}
};

class Scmp {
  private:
    const PROGMEM char* const*  src;
    uint8_t Idx;
  public:
    Scmp(const PROGMEM char* const* p) {
      src = p;
    }
    bool parse(char c) {
      char buf[16];
      strcpy_P(buf, (char*) pgm_read_word(src));
      if (c == buf[Idx++]) {
        if (Idx == strlen(buf)) return true;
      }
      else {
        Idx = 0;
      }
      return false;
    }
};

class ESP8266{
    
public:
  char* BUFFER;

private:
//char BUFFER[256];
// Hooks to write data - either over bridge or direct UART
  WriteHook Write;
  ReadAvailableHook ReadAvailable;
  ReadHook Read;
    
public:
  ESP8266(WriteHook w, ReadAvailableHook ra, ReadHook r){
    Write = w;
    ReadAvailable = ra;
    Read = r;
  }

  void send_http_request(const PROGMEM char * h1, const PROGMEM char * loc) {
    strcpy_P(BUFFER, h1);
    strcat_P(BUFFER, loc);
    strcat_P(BUFFER, h2);
    strcat_P(BUFFER, owmAPIKey);
    strcat_P(BUFFER, h3);
    espW(BUFFER);
  }

  // return true if string is found, false otherwise
  bool sstrstr(const char* needle, const char* haystack, uint16_t len){
    uint16_t n_len = strlen(needle);
    for(int i=0; i + n_len <= len; i++){
      if (strncmp(&haystack[i], needle, n_len) == 0){
        return true;
      }
    }
    return false;
  }

uint8_t get_version(char* version_str) { 
    bool IsResponseOK = false;
    uint16_t len = 0;   
    unsigned long start = millis(); 
    
    espW_P(GMR, strlen_P(GMR));      
    while ( ((millis() - start) & ~0UL) < 10000)
    {           
        len += ReadAvailable((uint8_t*)BUFFER + len);        
        if( sstrstr("OK" , &BUFFER[0], len))
        {               
            //Serial.println("AT+GMR response OK");
            IsResponseOK = true;
            break;
        }       
    }

    if(!IsResponseOK)
    {
        Serial.print("AT+GMR Response NOT OK : ");
        for(int i=0; i < len; i++)
        {
            Serial.print((char)BUFFER[i]); Serial.print(' ');
        }
        Serial.print('\n');
        len = 0; //resetting len to 0 when response is not ok
    }   
    else    
        memcpy(version_str,BUFFER,len);    
    return len;  
}

  bool at_reset(void){
    espW_P(RESET, strlen_P(RESET));
    if(rxString(OK, READY)  == 0xFF)return false;
    delay(1000);
    return true;
  }
  
  bool login_network(void) {

    espW_P(TEST, strlen_P(TEST));
    if(rxString(OK, READY) == 0xFF)return false;
    espW_P(ECHO_OFF, strlen_P(ECHO_OFF));
    if(rxString(OK, READY) == 0xFF)return false;
    espW_P(SET_MODE_AP_STATION, strlen_P(SET_MODE_AP_STATION));
    if(rxString(OK, NO_CHANGE) == 0xFF)return false;
    espConnect(SSID, PWD);
    if(rxString(OK, ALREADY_CONNECT,READY,  20000) == 0xFF)return false;
    espW_P(NOMUX, strlen_P(NOMUX));
    if(rxString(OK) != 1)Serial.println(F("NOMUX failed"));
    // At this point we already have a useful WIFI connection
    return true;
  }

  bool open_socket(void){
    tcpConnect(HOST, TCP_PORT);
    if(rxString("CONNECT", READY, 10000) != 1)Serial.println(F("Sock up"));
    
    espW_P(PASSTHRU, strlen_P(PASSTHRU));
    if(rxString(OK) != 1)return false;
    espW_P(TCPSEND, strlen_P(TCPSEND));
    if(rxString(">") != 1)return false;

    return true;
  }

  bool close_socket(void){
    espW_P(EXITPASSTHRU, strlen_P(EXITPASSTHRU));
    delay(2000); // must wait for 1s before sending next AT command according to ESP docs
    espW_P(TCPCLOSE, strlen_P(TCPCLOSE));
    if(rxString(OK) != 1)return false;
    Serial.print(F("CLOSE\n"));
    return true;
  }

  void espConnect(const PROGMEM char* ssid, const PROGMEM char* pwd) {
    char buff[64];
    strcpy_P(buff, CONNECT);
    strcat_P(buff, ssid);
    strcat(buff, "\",\"");
    strcat_P(buff, pwd);
    strcat(buff, "\"\r\n");
    Serial.println(F("Connecting to: "));
    espW(buff);
  }

  void tcpConnect(const PROGMEM char* host, const PROGMEM char* port) {
    char buff[64];
    strcpy_P(buff, OPENTCP);
    strcat_P(buff, host);
    strcat(buff, "\",");
    strcat_P(buff, port);
    strcat(buff, "\r\n");
    Serial.println(F("Connecting to: "));
    //Serial.print((char*)buff);
    espW(buff);
  }

  // FIXME: Add IOCTL option to flush buffer
  void esp_flush(void){
    uint16_t len = 1;
    while(len){
      //len = CleO.ReadUART(h_esp_comm, sizeof(BUFFER)-1, (uint8_t*)BUFFER); // clear buffer
      len = Read( sizeof(BUFFER)-1, (uint8_t*)BUFFER); // clear buffer
    }
    //CleO.ReadAvailableUART(h_esp_comm,(uint8_t*)BUFFER); // clear buffer
  }
  
  void espW(const char* cmd) {
    Serial.print((const char*)cmd);
    esp_flush();
  #if 0
    Serial.print(len); Serial.print(":");
    for (int i = 0; i < len; i++) {
      Serial.print((char)BUFFER[i]);
    }
    Serial.print("\n");
  #endif
    //CleO.WriteUART(h_esp_comm, strlen(cmd), (uint8_t*)cmd);
    Write( strlen(cmd), (uint8_t*)cmd);
  }

  void espW_P(const char PROGMEM * cmd, uint16_t len) {
    //Serial.print(cmd);
    esp_flush();
    uint8_t b[len];
    for(int i=0; i<len; i++){
      b[i] = pgm_read_byte(&cmd[i]);
      //Serial.print((char)b[i]);
    }
    //CleO.WriteUART(h_esp_comm, len, b);
    Write( len, b);
  }

  uint8_t rxString(const char* t1, uint16_t timeout = 1000){
    return(rxString(t1, NULL, NULL, timeout));
  }

  uint8_t rxString(const char* t1, const char* t2, uint16_t timeout = 1000){
    return(rxString(t1, t2, NULL, timeout));
  }

  // Returns the ID of matching target, 0xFF if timeout
  // remember to clear the buffer before sending out a command
  uint8_t rxString(const char* t1,const char* t2,const char* t3, uint16_t timeout){
    uint8_t ret = 0xFF;
    uint16_t len = 0;
    unsigned long start = millis();
    while ( ((millis() - start) & ~0UL) < timeout) {
      //  len += CleO.ReadAvailableUART(h_esp_comm,(uint8_t*)BUFFER + len);
      len += ReadAvailable((uint8_t*)BUFFER + len);
      if((t1 != NULL) && sstrstr(t1, &BUFFER[0], len)){
        //Serial.print(F("Found t1"));
        Serial.print(t1);
        ret = 1;
        break;
      }

      if((t2 != NULL) && sstrstr(t2, &BUFFER[0], len)){
        //Serial.print(F("Found t2"));
        Serial.print(t2);
        ret = 2;
        break;
      }
      if((t3 != NULL) && sstrstr(t3, &BUFFER[0], len)){
        //Serial.print(F("Found t3"));
        Serial.print(t3);
        ret = 3;
        break;
      }
    }

    if(ret == 0xFF){
      for(int i=0; i < len; i++){
          Serial.print((char)BUFFER[i]); Serial.print(' ');
      }
    }
    Serial.print('\n');
    return ret;
  }

    
};


#endif
