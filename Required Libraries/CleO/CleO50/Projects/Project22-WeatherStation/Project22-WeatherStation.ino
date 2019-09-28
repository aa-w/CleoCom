/* (C) Copyright Bridgetek Pte Ltd.
* ============================================================================
*
* This source code ("the Software") is provided by Bridgetek Pte Ltd
*  ("Bridgetek") subject to the licence terms set out
* http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
* You must read the Licence Terms before downloading or using the Software.
* By installing or using the Software you agree to the Licence Terms. If you
* do not agree to the Licence Terms then do not download or use the Software.
*/


/**
  Sketch:              
    Project22-WeatherStation.ino
  Purpose:              
    This project creates an Internet based weather station using an ESP8266 Wifi module
    and CleO. The current and forecased weather data is polled from https://openweathermap.org/
    The Icons in this project are created by Adam Whitcroft (http://adamwhitcroft.com/climacons/)
    and are distributed with permission.
  External component:   
    CleOWifi module
    REFERENCES:
    1. ESP8266 Developer Resources - http://bbs.espressif.com/index.php
    2. Open Weather Map API - http://openweathermap.org/api
  Hardware Connection:
    Connect CleOWifi module to CleO50 CN5 connector
*/

#include <SPI.h>
#include <CleO.h>

// Set your router SSID and PWD here!
const PROGMEM char SSID[] = { "ihome_belkin" };
const PROGMEM char PWD[] = { "12345678" };
#include <esp8266.h>
#include <Weather.h>
#define SIMULATE       0 // SET to use dummy JSON data without WiFi
#define MAX_ELE_CHAR_LEN  32u  // Max characters in any json element we will parse (anything more than this will be truncated)
#include <owm.h>

#define UTC_OFFSET    (8UL*60*60)
#define POLL_INTERVAL (180*1000UL)
#define HTTP_TIMEOUT  (30*1000UL)
// Current Weather
const  char h1_current[] PROGMEM = { "GET http://api.openweathermap.org/data/2.5/weather?units=metric&q=" };
// Hourly forecast, limit count to 4
const  char h1_hourly[] PROGMEM = { "GET http://api.openweathermap.org/data/2.5/forecast?units=metric&cnt=5&q=" };
// Daily forecast, limit count to 5
const  char h1_daily[] PROGMEM = { "GET http://api.openweathermap.org/data/2.5/forecast/daily?units=metric&cnt=6&q=" };

// -----------------------  UI Elements for weather ------------------------------
/* UI Elements */

#define W   (int16_t)(480)
#define H   (int16_t)(800)
#define FONT_A FONT_SANS_3
#define FONT_B FONT_SANS_3
#define FONT_TEMP   FONT_SANS_7
#define FONT_TEMP   FONT_SANS_8
#define FONT_TODAY  FONT_SANS_4
#define FONT_DESCRIPTION  FONT_SANS_3

/* Location String [Justification = MM] */
#define LOCX  (W/2)
#define LOCY  (20)
/* Todays Date [Justification = MM] */
#define TDTX  (W/2)
#define TDTY  (50)
//#define TDTY  (70)
/* Current Temperature [Justification = MM] */
#define TCX   (W/2 - 20)
#define TCY   (H/4 - 10)
/* Description [Justification = MM] */
#define DSCX   (W/2)

#define DSCY   (TCY + 75) 
/* Big rectangle [40% of screen, J=BL] */
#define RCTX   (0)
#define RCTY   (H)
#define RCTW   (W)
#define RCTH   ((40 * H)/100)
/* Forecast [Justification = MM, J=TL] */
#define RCT2X   (0)
#define RCT2Y   (H - RCTH)
#define RCT2W   (W/2)

#define RCT2H   (60) 
/* Today [Justification = MM] */
#define TDYX   (RCT2X + (RCT2W / 2))
#define TDYY   (RCT2Y + (RCT2H / 2))
/* Forecast [Justification = MM] */
#define NXTX   (RCT2X + ((3 * RCT2W) / 2))
#define NXTY   (TDYY)
/* WiFi ICON */
#define WIFIX   (W - 30)
#define WIFIY   (20)
/* Refresh ICON */
#define RFSHX   (20)
#define RFSHY   (20)

#define BTMTXT  (50) 
/* Things at the bottom, there are 4 elements */
#define LSTSPN  (W/4)
/* Icons */
#define LSTX   (LSTSPN/2)

#define LSTY   (H - 140) 
/* String 1 */
#define STRB1X   (LSTX)
#define STRB1Y   (TDYY + RCT2H)
/* Min */

#define STRB2X   (LSTX - 30) 
#define STRB2Y   (H - BTMTXT)
/* Thermometer */
#define IMGTX   (LSTX)
#define IMGTY   (H - BTMTXT)
/* Max */

#define STRB3X   (LSTX + 30) 
#define STRB3Y   (H - BTMTXT)
/* Humidity */
#define IMGHX   (W/6)
#define IMGHY   (DSCY + 50)
#define STRHX   (IMGHX)
#define STRHY   (IMGHY + 30)
/* Wind Speed */
#define IMGWX   (W/6 + W/3)
#define IMGWY   (DSCY + 50)
#define STRWX   (IMGWX)
#define STRWY   (IMGWY + 30)
/* Rain */
#define IMGRX   (W/6 + (2*W)/3)
#define IMGRY   (DSCY + 50)
#define STRRX   (IMGRX)
#define STRRY   (IMGRY + 30)

uint8_t CurrentAPI;

typedef enum {
	Good,
	Bad
} WiFiConnectivity;

uint8_t WiFiStatus;

WeatherInfo MyWeather[5];
DailyForecast MyDailyForecast[5];

const char Location[] PROGMEM = { "Singapore" };
uint8_t flag4Day = false;
bool flagDaily;
bool IsCelsius;
uint8_t MySelection[2]; // current selection, default = 0
const char Forecast[] PROGMEM = { "4 Days" };
const char Today[] PROGMEM = { "Today" };
const char ICON_Humidity[] PROGMEM = { "@Icons\\m48.ftico" };
const char ICON_Weather[] PROGMEM = { "@Icons\\weather.ftico" };
const char ConnectingTo[] PROGMEM = { "WiFi Connecting to:" };
const uint32_t BgColors[] PROGMEM = { LEMONCHIFFON4, DARKGREEN, 0xFD3C3C, 0xFFB74C, 0x138d90 };


#define Z_F2C(x)      (int)(((((x) - 32) * 5.0) / 9) + 0.5)
#define Z_C2F(x)      (int)(((((x) * 9) / 5.0) + 32) + 0.5)
#define T(x)          ((IsCelsius) ? (x) : Z_C2F(x))

#define ICON_WIFI_GOOD    ICON_Humidity
#define ICON_WIFI_BAD   ICON_Humidity
#define ICON_REFSH      ICON_Humidity
#define TAG_4DAY    20
#define TAG_CELSIUS     30
#define TAG_REFRESH     40

#define Z_GetDecimal(x)     ((int)(((x) - (int)(x)) * 100))

// Indexes for the JSON elements
typedef enum {
#define X(a, b) hourly_##a,
	JSON_HOURLY_FORECAST
#undef X
	MaxForecastIdx
} ForecastIdx;

typedef enum {
#define X(a, b) daily_##a,
	JSON_DAILY_FORECAST
#undef X
	MaxDailyForecastIdx
} DailyForecastIdx;

/*
To keep the JSON element strings in flash, we need a two step approach.
See: https://www.arduino.cc/en/Reference/PROGMEM
*/
// 1. Generate individual forecast strings
#define X(name, b) const char FCST_##name[] PROGMEM = {"\"" #name "\":"};
JSON_HOURLY_FORECAST
#undef X

#define X(name, b) const char FCST_daily_##name[] PROGMEM = {"\"" #name "\":"};
JSON_DAILY_FORECAST
#undef X

#define X(name) const char day_##name[] PROGMEM = { #name };
DAY_OF_WEEK
#undef X

// 2. Put them in a table of string pointers, reads are via this table
#define X(name, b) FCST_##name,
const char* const FCST_table[] PROGMEM = {
	JSON_HOURLY_FORECAST
};
#undef X

#define X(name, b) FCST_daily_##name,
const char* const FCST_daily_table[] PROGMEM = {
	JSON_DAILY_FORECAST
};
#undef X

#define X(name) day_##name,
const char* const DayOfWeek_table[] PROGMEM = {
	DAY_OF_WEEK
};
#undef X

#define NO_OF_ITEMS			5u

#define X(a, b)   Scmp cmp_hr_##a = Scmp(&(FCST_table[hourly_##a]));
JSON_HOURLY_FORECAST
#undef X

#define X(a, b)   Scmp cmp_dy_##a = Scmp(&FCST_daily_table[daily_##a]);
JSON_DAILY_FORECAST
#undef X

uint8_t WIdx = 0; // Index to the information parsed
uint8_t ParserState = 0; // 0 - look for key, 1 - read value
uint8_t max_len = 0; // The maximum length for a parameter in WeatherInfo
uint8_t DestIdx = 0; // Current index of result buffer
uint8_t ElementCnt; // Number of units we want to collect
char result[32];
char BUFFER[256];
uint8_t h_esp_comm, h_esp_enable, h_esp_reset;
Timeout poll;
bool rebuild_ui;
uint32_t rectColors[4] = { DARKGREEN , 0xFD3C3C, 0xFFB74C, 0x138d90 };
int16_t im_t,im_h,im_w,im_r,im_refresh; /* Handles for images */
/* callbacks from ESP8266 class */
void Write(uint16_t len, uint8_t* buf);
uint16_t Read(uint16_t len, uint8_t* buf);
uint16_t ReadAvailable(uint8_t* buf);

ESP8266 ESP(Write, ReadAvailable, Read);

void setup() {
	Serial.begin(230400);
	ESP.BUFFER = BUFFER;

	CleO.begin();
	CleO.DisplayRotate(2, 0);
	init_ui();
	
	h_esp_enable = CleO.OpenGPIO(CLICK2_CS, Bridge::Output, Bridge::pad_pull_none);
	h_esp_reset = CleO.OpenGPIO(CLICK2_RST, Bridge::Output, Bridge::pad_pull_none);
	CleO.WriteGPIO(h_esp_enable, ((uint32_t)0x01 << CLICK2_CS));
	CleO.WriteGPIO(h_esp_reset, (uint32_t)0);
    delay(200);
	CleO.WriteGPIO(h_esp_reset, ((uint32_t)0x01 << CLICK2_RST));
	delay(500);
	

	Serial.println(F("==== UART Open ===="));
	
	h_esp_comm = (uint8_t)CleO.OpenUART(Bridge::CleO50_click_2, Bridge::b115200, ESP_READ_TIMEOUT);
	
	Serial.print(F("UART Handle:"));
	Serial.print(h_esp_comm, HEX);
	WiFiStatus = Bad;
	CurrentAPI = APICurrentWeather;
	IsCelsius = true;

	// Load fixed icons that we always need
	char spf_buffer[32];
	strcpy_P(spf_buffer, ICON_Weather);
	im_t = CleO.LoadIcon(spf_buffer, ICON_THERMOMETER_50_PNG);
	strcpy_P(spf_buffer, ICON_Humidity);
	im_h = CleO.LoadIcon(spf_buffer, ICON_LOCATION_ON);
	strcpy_P(spf_buffer, ICON_Weather);
	im_w = CleO.LoadIcon(spf_buffer, ICON_WIND_PNG);
	strcpy_P(spf_buffer, ICON_Weather);
	im_r = CleO.LoadIcon(spf_buffer, ICON_UMBRELLA_PNG);
	strcpy_P(spf_buffer, ICON_REFSH);
	im_refresh = CleO.LoadIcon(spf_buffer, ICON_REFRESH);
}

void loop() {
	switch (WiFiStatus) {
	case Bad:
#if SIMULATE
		WiFiStatus = Good;
		poll.Reset(millis() - (POLL_INTERVAL + 10));
#else
		if (!ESP.login_network()) {
			Serial.println(F("Unable to connect to WiFi and get IP"));
		}
		else {
			Serial.println(F("WiFi nw OK"));
			WiFiStatus = Good;
			poll.Reset(millis() - (POLL_INTERVAL + 10));
		}
#endif
		break;

	case Good:
#if SIMULATE
		if (poll.IsExpired(POLL_INTERVAL)) {
			sync_sim();
			rebuild_ui = true;
			poll.Reset(millis());
		}
#else
		if (poll.IsExpired(POLL_INTERVAL)) {
			build_screen(true);
			clear_cache();
			for (CurrentAPI = 0; (CurrentAPI < MaxAPITypes) && (WiFiStatus == Good); CurrentAPI++) {
				init_parser();
				sync_weather();
				rebuild_ui = true;
			}
			poll.Reset(millis());
		}
#endif
		break;
	}

	if (rebuild_ui) build_screen(false);

	control();

}

void clear_cache(void) {
	memset(MyWeather, 0, sizeof(MyWeather));
	memset(MyDailyForecast, 0, sizeof(MyDailyForecast));
	MySelection[0] = 0;
	MySelection[1] = 0;
}

void init_ui(void) {
	char spf_buffer[32];
	CleO.Start();
	CleO.SetBackgroundGradient(0, 0, W, H, DARK_BLUE, DARK_RED);
	CleO.RectangleCorner(1);
	strcpy_P(spf_buffer, ConnectingTo);
	CleO.StringExt(FONT_MEDIUM, W / 2, H / 2 - 50, WHITE, MM, 0, 0, spf_buffer);
	strcpy_P(spf_buffer, SSID);
	CleO.StringExt(FONT_MEDIUM, W / 2, H / 2, GREEN, MM, 0, 0, spf_buffer);
	CleO.Show();
}


void build_screen(bool refresh) {
	char spf_buffer[32];
	int16_t year, month, day, h, m, s, wday, im_wifi, icon_id;
	int16_t img[4];
	char file[16];

	CleO.Start();

	CleO.SetBackgroundGradient(0, 0, W, H, pgm_read_dword(&BgColors[MySelection[flag4Day]]), pgm_read_dword(&BgColors[MySelection[flag4Day]]));

	CleO.RectangleCorner(1);
	CleO.RectangleExt(RCTX, RCTY, RCTW, RCTH, getColor(23, 66, 83, 1.0f), BL, 0, 0); //0xffffffUL
	CleO.EnableTag(true);
	CleO.Tag(TAG_4DAY);
	if (flag4Day)
		CleO.RectangleExt(RCT2X, RCT2Y, RCT2W, RCT2H, getColor(23, 66, 83, 0.5f), TL, 0, 0);
	else
		CleO.RectangleExt(RCT2X + RCT2W, RCT2Y, RCT2W, RCT2H, getColor(23, 66, 83, 0.5f), TL, 0, 0);
	CleO.EnableTag(false);
	// Location
	strcpy_P(spf_buffer, Location);
	CleO.StringExt(FONT_MEDIUM, LOCX, LOCY, WHITE, MM, 0, 0, spf_buffer);
	// Time
	if (flag4Day) {
		CleO.EpochToDate((uint32_t)(MyDailyForecast[MySelection[flag4Day]].dt + UTC_OFFSET), year, month, day, h, m, s, wday);
		sprintf_P(spf_buffer, PSTR("%S  %02d:%02d:%04d"), (char*)pgm_read_word(&DayOfWeek_table[wday]), day, month + 1, year);
	}
	else {
		CleO.EpochToDate((uint32_t)(MyWeather[MySelection[flag4Day]].dt + UTC_OFFSET), year, month, day, h, m, s, wday);
		sprintf_P(spf_buffer, PSTR("%02d:%02d:%02d  %02d:%02d:%04d"), h, m, s, day, month + 1, year);
	}

	CleO.StringExt(FONT_A, TDTX, TDTY, WHITE, MM, 0, 0, spf_buffer);
	Serial.println(spf_buffer);

	// Current Temp
	if (flag4Day) {
		CleO.EnableTag(true);
		CleO.Tag(TAG_CELSIUS);
		print_temp(MyDailyForecast[MySelection[flag4Day]].day, FONT_TEMP, TCX, TCY, 1);
		CleO.EnableTag(false);
		sprintf_P(spf_buffer, PSTR("%d%%"), MyDailyForecast[MySelection[flag4Day]].humidity);
		CleO.StringExt(FONT_A, STRHX, STRHY, WHITE, MM, 0, 0, spf_buffer);
		sprintf_P(spf_buffer, PSTR("%d.%2d mm"), (int)MyDailyForecast[MySelection[flag4Day]].rain, Z_GetDecimal(MyDailyForecast[MySelection[flag4Day]].rain));
		CleO.StringExt(FONT_A, STRRX, STRRY, WHITE, MM, 0, 0, spf_buffer);
		sprintf_P(spf_buffer, PSTR("%d.%2d m/s"), (int)MyDailyForecast[MySelection[flag4Day]].speed, Z_GetDecimal(MyDailyForecast[MySelection[flag4Day]].speed));
		CleO.StringExt(FONT_A, STRWX, STRWY, WHITE, MM, 0, 0, spf_buffer);
	}
	else {
		CleO.EnableTag(true);
		CleO.Tag(TAG_CELSIUS);
		print_temp(MyWeather[MySelection[flag4Day]].temp, FONT_TEMP, TCX, TCY, 1);
		CleO.EnableTag(false);
		sprintf_P(spf_buffer, PSTR("%d%%"), MyWeather[MySelection[flag4Day]].humidity);
		CleO.StringExt(FONT_A, STRHX, STRHY, WHITE, MM, 0, 0, spf_buffer);
		sprintf_P(spf_buffer, PSTR("%d.%2d mm"), (int)MyWeather[MySelection[flag4Day]].rain, Z_GetDecimal(MyWeather[MySelection[flag4Day]].rain));
		CleO.StringExt(FONT_A, STRRX, STRRY, WHITE, MM, 0, 0, spf_buffer);
		sprintf_P(spf_buffer, PSTR("%d.%2d m/s"), (int)MyWeather[MySelection[flag4Day]].speed, Z_GetDecimal(MyWeather[MySelection[flag4Day]].speed));
		Serial.print(spf_buffer);
		CleO.StringExt(FONT_A, STRWX, STRWY, WHITE, MM, 0, 0, spf_buffer);
	}

	for (int i = 0, j = 0; j < 4; i++) {
		if (i == MySelection[flag4Day]) continue;

		CleO.EnableTag(true);
		CleO.Tag((j + 1));
		CleO.RectangleExt(j * LSTSPN, RCT2Y + RCT2H, LSTSPN, H - (RCT2Y + RCT2H), pgm_read_dword(&BgColors[i]), TL, 0, 0);
		CleO.EnableTag(false);

		if (flag4Day) {

			CleO.EpochToDate((uint32_t)((MyDailyForecast[i].dt) + UTC_OFFSET), year, month, day, h, m, s, wday);
			strcpy_P(spf_buffer, (char*)pgm_read_word(&(DayOfWeek_table[wday])));
			CleO.StringExt(FONT_A, STRB1X + j * LSTSPN, STRB1Y, WHITE, MM, 0, 0, spf_buffer);
			icon_id = code2icon(MyDailyForecast[i].icon);
			strcpy_P(spf_buffer, ICON_Weather);
			Serial.println(spf_buffer);
			img[j] = CleO.LoadIcon(spf_buffer, icon_id);
			CleO.BitmapExt(img[j], LSTX + j * LSTSPN, LSTY, BLACK, MM, 0, 0);
			print_temp(MyDailyForecast[i].min, FONT_B, STRB2X + j * LSTSPN, STRB2Y, 0);
			print_temp(MyDailyForecast[i].max, FONT_B, STRB3X + j * LSTSPN, STRB3Y, 0);
		}
		else {
			CleO.EpochToDate((uint32_t)((MyWeather[i].dt) + UTC_OFFSET), year, month, day, h, m, s, wday);
			sprintf_P(spf_buffer, PSTR("%02d:%02d"), h, m);
			CleO.StringExt(FONT_A, STRB1X + j * LSTSPN, STRB1Y, WHITE, MM, 0, 0, spf_buffer);
			icon_id = code2icon(MyWeather[i].icon);
			strcpy_P(spf_buffer, ICON_Weather);
			Serial.println(spf_buffer);
			img[j] = CleO.LoadIcon(spf_buffer, icon_id);
			CleO.BitmapExt(img[j], LSTX + j * LSTSPN, LSTY, BLACK, MM, 0, 0);
			print_temp(MyWeather[i].temp, FONT_A, IMGTX + j * LSTSPN, IMGTY, 0);
		}
		j++;
	}

	CleO.BitmapExt(im_w, IMGWX, IMGWY, BLACK, MM, 0, 0);
	CleO.BitmapExt(im_r, IMGRX, IMGRY, BLACK, MM, 0, 0);
	if (flag4Day) {
		print_temp((MyDailyForecast[MySelection[flag4Day]].min), FONT_SANS_3, DSCX - 35, DSCY, 0);
		print_temp((MyDailyForecast[MySelection[flag4Day]].max), FONT_SANS_3, DSCX + 27, DSCY, 0);
		CleO.BitmapExt(im_t, DSCX, DSCY, BLACK, MM, 0, 0);
	}
	else {
		CleO.StringExt(FONT_DESCRIPTION, DSCX, DSCY, WHITE, MM, 0, 0, MyWeather[MySelection[flag4Day]].description);
	}
	strcpy_P(spf_buffer, Today);
	CleO.StringExt(FONT_TODAY, TDYX, TDYY, WHITE, MM, 0, 0, spf_buffer);
	strcpy_P(spf_buffer, Forecast);
	CleO.StringExt(FONT_TODAY, NXTX, NXTY, WHITE, MM, 0, 0, spf_buffer);

	CleO.BitmapTransform(0.8f, 0);
	for (int i = 0; i < 4 && flag4Day; i++) {
		CleO.BitmapExt(im_t, IMGTX + i * LSTSPN, IMGTY, BLACK, MM, 0, 0);
	}

	if (WiFiStatus == Good) {
		strcpy_P(spf_buffer, ICON_WIFI_GOOD);
		im_wifi = CleO.LoadIcon(spf_buffer, ICON_WIFI);
		CleO.BitmapExt(im_wifi, WIFIX, WIFIY, GREEN, MM, 0, 0);
	}
	else {
		strcpy_P(spf_buffer, ICON_WIFI_BAD);
		im_wifi = CleO.LoadIcon(spf_buffer, ICON_SIGNAL_WIFI_OFF);
		CleO.BitmapExt(im_wifi, WIFIX, WIFIY, RED, MM, 0, 0);
	}
	strcpy_P(spf_buffer, ICON_REFSH);

	CleO.EnableTag(true);
	CleO.Tag(TAG_REFRESH);
	if (refresh)
		CleO.BitmapExt(im_refresh, RFSHX, RFSHY, BLACK, MM, 0, 0);
	else
		CleO.BitmapExt(im_refresh, RFSHX, RFSHY, GREEN, MM, 0, 0);
	CleO.EnableTag(false);

	CleO.BitmapTransform(0.8f, 180);
	CleO.BitmapExt(im_h, IMGHX, IMGHY, BLACK, MM, 0, 0);

	CleO.Show();
	CleO.Free(im_wifi);
	for (int i = 0; i < 4; i++) {
		CleO.Free(img[i]);
	}
	rebuild_ui = false;
}

bool sync_weather(void) {
	bool success = false;
	init_parser();

	for (uint8_t retry = 0; retry < 4; retry++) {
		if (ESP.open_socket()) {
			success = true;
			break;
		}
		success = false;
		Serial.println(F("Error opening sock"));
	}
	if (!success) {
		WiFiStatus = Bad;
		return false;
	}

	if (CurrentAPI == APIDailyForecast)
		ESP.send_http_request(h1_daily, Location);
	else if (CurrentAPI == APIHourlyForecast)
		ESP.send_http_request(h1_hourly, Location);
	else if (CurrentAPI == APICurrentWeather)
		ESP.send_http_request(h1_current, Location);

	Timeout timeout;
	bool parsed = false;
	while (!timeout.IsExpired(HTTP_TIMEOUT) && !parsed)
	{
		uint16_t len = ReadAvailable((uint8_t*)BUFFER);
		if (len > 250) Serial.println(len);
		parsed = parse_http_response(len);
	}

	if (!parsed) {
		Serial.println(F("Error: Parser Timeout!"));
		WiFiStatus = Bad;
	}
	Serial.println(F("Done Parsing!"));

	for (uint8_t retry = 0; retry < 2; retry++) {
		if (ESP.close_socket()) {
			success = true;
			break;
		}
		success = false;
		Serial.println(F("Error closing sock"));
	}
	if (!success) {
		WiFiStatus = Bad;
		return false;
	}

	return true;
}

/* Print the temperature with the degree symbol by drawing a circle */
void print_temp(int t, int f_size, int x, int y, int opt)
{
	t = T(t); // convert to Cel/Far
	int nd = 0, w = 0, h = 0, nc = 0, dx = 0, dy = 0, r1 = 0, r2 = 0;
	if (t < 0) nc = 1;
	if (t < 10) nd = 1;
	else if (t < 100) nd = 2;

	if (f_size == FONT_SANS_8) // w = 52, h = 102
	{
		w = 52; h = 108;
		dx = (nd * w / 2 + nc * 41 / 2) + 12;
		dy = 20;
		r1 = 10 / 2;
		r2 = 20 / 2;
	}
	else if (f_size == FONT_SANS_7) // w = 40, h = 83
	{
		w = 40; h = 83;
		dx = (nd  * w / 2 + nc * 32 / 2) + 12;
		dy = 10;
		r1 = 9 / 2;
		r2 = 18 / 2;
	}
	else if (f_size == FONT_TINY) //26
	{
		w = 8; h = 8;
		dx = (nd * w / 2 + nc * 7 / 2) + 4;
		dy = 3;
		r1 = 3 / 2;
		r2 = 4 / 2;
	}
	else if (f_size == FONT_SANS_3) //29
	{
		w = 14; h = 28;
		dx = (nd * w / 2 + nc * 11 / 2) + 5;
		dy = 3;
		r1 = 4 / 2;
		r2 = 8 / 2;
	}
	else if (f_size == FONT_SANS_1) //27
	{
		w = 10; h = 20;
		dx = (nd * w / 2 + nc * 7 / 2) + 3;
		dy = 4;
		r1 = 1;
		r2 = 2;
	}
	char spf_buffer[10];
	sprintf_P(spf_buffer, PSTR("%d"), t);
	CleO.StringExt(f_size, x, y, WHITE, MM, 0, 0, spf_buffer);
	CleO.CircleExt(x + dx, y - dy, r1, r2, WHITE, BM, 0, 0);
	if (opt == 1)
	{
		if (IsCelsius)
		{
			CleO.StringExt(--f_size, x + dx + 30, y, WHITE, MM, 0, 0, "C");
		}
		else
		{
			CleO.StringExt(--f_size, x + dx + 30, y, WHITE, MM, 0, 0, "F");
		}
	}
}


uint32_t getColor(uint8_t r, uint8_t g, uint8_t b, float alpha) {
	uint32_t ret = 0;
	uint8_t x = Z_ALPHA(r, BG_R, alpha);
	uint8_t y = Z_ALPHA(g, BG_G, alpha);
	uint8_t z = Z_ALPHA(b, BG_B, alpha);
	ret = (((uint32_t)x << 16) | ((uint32_t)y << 8) | z);
	return ret;
}

bool IsTag(int16_t t)
{
	if (t >= 1 && t <= 4) return true;
	else if (t == TAG_4DAY || t == TAG_CELSIUS || t == TAG_REFRESH) return true;
	else return false;
}

void control()
{
	int16_t x, y, dur, tag;
	CleO.TouchCoordinates(x, y, dur, tag);
	static int16_t prevdur, prevtag;
	/* pen down  */
	if ((prevdur == 0) && (dur > 0) && IsTag(tag) && (prevtag != tag))
	{
		CleO.SetSound(0x57, 0, 100, 0);
		CleO.SoundPlay(1);
	}
    else
    {
        CleO.SoundPlay(0);
    }
    
	if (!dur && prevdur && (tag == TAG_4DAY)) {
		flag4Day ^= 1;
		if (WiFiStatus == Good) rebuild_ui = true;
	}
	else if (!dur && prevdur && (tag == 1 || tag == 2 || tag == 3 || tag == 4)) {
		if (tag <= MySelection[flag4Day]) tag--;

		MySelection[flag4Day] = tag;
		rebuild_ui = true;
	}
	else if (!dur && prevdur && (tag == TAG_CELSIUS)) { // using pen-up
		rebuild_ui = true;
		IsCelsius ^= 1;
	}
	else if (!dur && prevdur && (tag == TAG_REFRESH)) {
		poll.Reset(millis() - POLL_INTERVAL);
	}
	prevdur = dur;
	prevtag = tag;
}


void init_parser(void) {
	WIdx = 0; // Index to the information parsed
	ParserState = 0; // 0 - look for key, 1 - read value
	max_len = 0; // The maximum length for a parameter in WeatherInfo
	DestIdx = 0; // Current index of result buffer
	ElementCnt = 0;
}

void init_parse_state1(void) {
	ParserState = 1;
	memset(result, '\0', sizeof(result));
	DestIdx = 0;
	max_len = MAX_ELE_CHAR_LEN;
}

// len = number of valid bytes in the RxBuffer that can be searched
// returns true if a value was parsed, false otherwise
bool parse_http_response(uint16_t len) {
	uint16_t i = 0;
	char cstring[64]; // A ram copy of the current element we are looking for

	for (; i < len; i++) {
		switch (ParserState) {
		case 0:
			// we want to parse all the items in parallel
			if (CurrentAPI == APIDailyForecast) {
#define X(a, b)																	\
  do{																			\
	if(cmp_dy_##a.parse(BUFFER[i]) == true){									\
	  strcpy_P(cstring, (char*)pgm_read_word(&(FCST_daily_table[daily_##a])));  \
	  Serial.print(cstring);													\
	  WIdx = daily_##a;															\
	  init_parse_state1();														\
	  break;																	\
	}																			\
  } while (0);
				JSON_DAILY_FORECAST
#undef X
			}
			else {
#define X(a, b)																	\
  do{																			\
	if(cmp_hr_##a.parse(BUFFER[i]) == true){									\
	  strcpy_P(cstring, (char*)pgm_read_word(&(FCST_table[hourly_##a])));       \
	  Serial.print(cstring);													\
	  WIdx = hourly_##a;														\
	  init_parse_state1();														\
	  break;																	\
	}																			\
  } while (0);
				JSON_HOURLY_FORECAST
#undef X
			}
			break;

		case 1:
			if ((BUFFER[i] == ',') || (BUFFER[i] == '}') || (DestIdx >= max_len)) {
				// done capturing value!
				//Serial.print(cstring);
				Serial.println((char*)result);
				ParserState = 0;

				// Store the parsed value, move to next parameter
				if (CurrentAPI == APIDailyForecast) {
					if (WIdx == daily_dt) {
						Serial.println(F("+++++++++++++"));
						if (++ElementCnt > NO_OF_ITEMS) {
							return true; // Done parsing!
						}
					}
					switch (WIdx) {
#define X(a, conv)    case daily_##a: conv(&MyDailyForecast[ElementCnt - 1].a, result); break;
						JSON_DAILY_FORECAST
#undef X
					}
					break;
				}
				else if (CurrentAPI == APIHourlyForecast) {
					if (WIdx == hourly_dt) {
						Serial.println(F("=============="));
						if (++ElementCnt > 4) {
							return true; // Done parsing!
						}
					}
					switch (WIdx) {
#define X(a, conv)    case hourly_##a: conv(&MyWeather[ElementCnt].a, result); break;
						JSON_HOURLY_FORECAST
#undef X
					}
					break;
				}
				else {
					switch (WIdx) {
#define X(a, conv)    case hourly_##a: conv(&MyWeather[0].a, result); break;
						JSON_HOURLY_FORECAST
#undef X
					}

					if ((WIdx) == hourly_dt) {
						Serial.println(F("%%%%%%%%%%%%%%"));
						return true; // Done parsing!
					}
					break;
				}
			}
			else {
				if (BUFFER[i] != '\"') // ignore " character inside string values
					result[DestIdx++] = BUFFER[i];
			}
			break;
		}
	}
	return false;
}

void Write(uint16_t len, uint8_t* buf) {
	CleO.WriteUART(h_esp_comm, len, buf);
}
uint16_t Read(uint16_t len, uint8_t* buf) {
	return (CleO.ReadUART(h_esp_comm, len, buf));
}
uint16_t ReadAvailable(uint8_t* buf) {
	return CleO.ReadAvailableUART(h_esp_comm, buf);
}

#if SIMULATE

void sync_sim() {
	uint16_t ptr;
	bool parsed = false;
	init_parser();
	memset(MyWeather, 0, sizeof(MyWeather));
	memset(MyDailyForecast, 0, sizeof(MyDailyForecast));

	for (CurrentAPI = 0; CurrentAPI < MaxAPITypes;) {
		WIdx = 0; parsed = false; ptr = 0;
		init_parser();

		while (!parsed) {
			uint16_t len = 10;
			if (CurrentAPI == APICurrentWeather) memcpy_P(BUFFER, json_current + ptr, 10);
			else if (CurrentAPI == APIDailyForecast) memcpy_P(BUFFER, json_daily + ptr, 10);
			else if (CurrentAPI == APIHourlyForecast) memcpy_P(BUFFER, json_hourly + ptr, 10);
			parsed = parse_http_response(len);
			ptr += len;
		}
		CurrentAPI = CurrentAPI + 1;
	}
}

#endif
