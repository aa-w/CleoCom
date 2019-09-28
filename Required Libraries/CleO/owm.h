
#ifndef OWM_H_
#define OWM_H_

#if SIMULATE // Raw data to test the parser
const PROGMEM char json_current[] = "{\"coord\":{\"lon\":103.85,\"lat\":1.29},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"02d\"}],\"base\":\"stations\",\"main\":{\"temp\":29.9,\"pressure\":1022.85,\"humidity\":93,\"temp_min\":29.9,\"temp_max\":29.9,\"sea_level\":1023.6,\"grnd_level\":1022.85},\"wind\":{\"speed\":2.02,\"deg\":224.505},\"clouds\":{\"all\":8},\"dt\":1477896732,\"sys\":{\"message\":0.0109,\"country\":\"SG\",\"sunrise\":1477867562,\"sunset\":1477911015},\"id\":1880252,\"name\":\"Singapore\",\"cod\":200}";

const PROGMEM char json_daily[] = "{\"city\":{\"id\":1880252,\"name\":\"Singapore\",\"coord\":{\"lon\":103.850067,\"lat\":1.28967},\"country\":\"SG\",\"population\":0},\"cod\":\"200\",\"message\":0.0197,\"cnt\":5,\"list\":[{\"dt\":1477886400,\"temp\":{\"day\":29.9,\"min\":28.28,\"max\":30.5,\"night\":28.28,\"eve\":30.5,\"morn\":29.9},\"pressure\":1022.85,\"humidity\":93,\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"02d\"}],\"speed\":2.02,\"deg\":225,\"clouds\":8},{\"dt\":1477972800,\"temp\":{\"day\":25.97,\"min\":25.12,\"max\":27.69,\"night\":27.25,\"eve\":27.69,\"morn\":25.22},\"pressure\":1024.05,\"humidity\":100,\"weather\":[{\"id\":501,\"main\":\"Rain\",\"description\":\"moderate rain\",\"icon\":\"10d\"}],\"speed\":1.53,\"deg\":347,\"clouds\":88,\"rain\":9.56},{\"dt\":1478059200,\"temp\":{\"day\":28.43,\"min\":26.77,\"max\":29.8,\"night\":28.45,\"eve\":29.8,\"morn\":26.77},\"pressure\":1023.58,\"humidity\":100,\"weather\":[{\"id\":802,\"main\":\"Clouds\",\"description\":\"scattered clouds\",\"icon\":\"03d\"}],\"speed\":1.06,\"deg\":246,\"clouds\":48},{\"dt\":1478145600,\"temp\":{\"day\":29.42,\"min\":27.04,\"max\":29.42,\"night\":27.04,\"eve\":27.23,\"morn\":27.75},\"pressure\":1021.43,\"humidity\":0,\"weather\":[{\"id\":501,\"main\":\"Rain\",\"description\":\"moderate rain\",\"icon\":\"10d\"}],\"speed\":1.91,\"deg\":67,\"clouds\":18,\"rain\":7.15},{\"dt\":1478232000,\"temp\":{\"day\":29,\"min\":26.6,\"max\":29,\"night\":26.6,\"eve\":27.31,\"morn\":27.59},\"pressure\":1020.97,\"humidity\":0,\"weather\":[{\"id\":501,\"main\":\"Rain\",\"description\":\"moderate rain\",\"icon\":\"10d\"}],\"speed\":1.4,\"deg\":160,\"clouds\":31,\"rain\":7.4}]}";

const PROGMEM char json_hourly[] = "{\"city\":{\"id\":1880252,\"name\":\"Singapore\",\"coord\":{\"lon\":103.850067,\"lat\":1.28967},\"country\":\"SG\",\"population\":0,\"sys\":{\"population\":0}},\"cod\":\"200\",\"message\":0.0018,\"cnt\":5,\"list\":[{\"dt\":1477904400,\"main\":{\"temp\":30.5,\"temp_min\":30.5,\"temp_max\":30.5,\"pressure\":1021.7,\"sea_level\":1022.51,\"grnd_level\":1021.7,\"humidity\":85,\"temp_kf\":0},\"weather\":[{\"id\":801,\"main\":\"Clouds\",\"description\":\"few clouds\",\"icon\":\"02d\"}],\"clouds\":{\"all\":20},\"wind\":{\"speed\":0.96,\"deg\":188.503},\"rain\":{},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2016-10-31 09:00:00\"},{\"dt\":1477915200,\"main\":{\"temp\":29.43,\"temp_min\":29.43,\"temp_max\":29.43,\"pressure\":1023.18,\"sea_level\":1023.98,\"grnd_level\":1023.18,\"humidity\":88,\"temp_kf\":0},\"weather\":[{\"id\":801,\"main\":\"Clouds\",\"description\":\"few clouds\",\"icon\":\"02n\"}],\"clouds\":{\"all\":12},\"wind\":{\"speed\":2.11,\"deg\":91.5032},\"rain\":{},\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2016-10-31 12:00:00\"},{\"dt\":1477926000,\"main\":{\"temp\":28.28,\"temp_min\":28.28,\"temp_max\":28.28,\"pressure\":1024.19,\"sea_level\":1025,\"grnd_level\":1024.19,\"humidity\":96,\"temp_kf\":0},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01n\"}],\"clouds\":{\"all\":0},\"wind\":{\"speed\":2.92,\"deg\":76.0004},\"rain\":{},\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2016-10-31 15:00:00\"},{\"dt\":1477936800,\"main\":{\"temp\":27,\"temp_min\":27,\"temp_max\":27,\"pressure\":1023.11,\"sea_level\":1023.95,\"grnd_level\":1023.11,\"humidity\":100,\"temp_kf\":0},\"weather\":[{\"id\":500,\"main\":\"Rain\",\"description\":\"light rain\",\"icon\":\"10n\"}],\"clouds\":{\"all\":12},\"wind\":{\"speed\":0.87,\"deg\":3.5051},\"rain\":{\"3h\":0.195},\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2016-10-31 18:00:00\"},{\"dt\":1477947600,\"main\":{\"temp\":25.22,\"temp_min\":25.22,\"temp_max\":25.22,\"pressure\":1022.99,\"sea_level\":1023.69,\"grnd_level\":1022.99,\"humidity\":100,\"temp_kf\":0},\"weather\":[{\"id\":501,\"main\":\"Rain\",\"description\":\"moderate rain\",\"icon\":\"10n\"}],\"clouds\":{\"all\":80},\"wind\":{\"speed\":4.66,\"deg\":237.502},\"rain\":{\"3h\":7.5},\"sys\":{\"pod\":\"n\"},\"dt_txt\":\"2016-10-31 21:00:00\"}]}";
#endif

// The order of elements is not important, however you need to analyze the JSON to find a way to identify the end of a string
// either by the ID of the last element or some other means
#define JSON_HOURLY_FORECAST      \
  X(dt, stol)       \
  X(temp, stoi)       \
  X(humidity, stoi)     \
  X(description, scp)   \
  X(icon, scp)        \
  X(speed, stof)      \
  X(rain, rtof)

#define JSON_DAILY_FORECAST       \
  X(dt, stol)       \
  X(day, stoi)        \
  X(min, stoi)        \
  X(max, stoi)        \
  X(humidity, stoi)     \
  X(icon, scp)        \
  X(rain, rtof)       \
  X(speed, stof)

#define DAY_OF_WEEK             \
  X(Sun)            \
  X(Mon)            \
  X(Tue)            \
  X(Wed)            \
  X(Thu)            \
  X(Fri)            \
  X(Sat)

// rain > broken > scattered > few
// http://openweathermap.org/weather-conditions
#define ICON_CODE                            \
              X( 0,"01", "Sun")              \
              X( 1,"02", "Cloud-Sun")        \
              X( 2,"03", "Cloud-Sun")        \
              X( 3,"04", "Cloud")            \
              X( 4,"09", "Cloud-Rain-Alt")   \
              X( 5,"10", "Cloud-Rain")       \
              X( 6,"11", "Cloud-Lightning")  \
              X( 7,"13", "Cloud-Snow")       \
              X( 8,"50", "Cloud-Fog")            

#define ICON_CODE2                            \
              X( 0,"01", ICON_SUN_PNG)              \
              X( 1,"02", ICON_CLOUD_SUN_PNG)        \
              X( 2,"03", ICON_CLOUD_SUN_PNG)        \
              X( 3,"04", ICON_CLOUD_PNG)            \
              X( 4,"09", ICON_CLOUD_RAIN_ALT_PNG)   \
              X( 5,"10", ICON_CLOUD_RAIN_PNG)       \
              X( 6,"11", ICON_CLOUD_LIGHTNING_PNG)  \
              X( 7,"13", ICON_CLOUD_SNOW_PNG)       \
              X( 8,"50", ICON_CLOUD_FOG_PNG)                       

#define FILE_EXT  ".png"

typedef enum {
#define X(num, id, fname) icon_##num,
  ICON_CODE
#undef X
  MaxDayIcon
}IconDay;

// 1. Generate individual Icon file names
#define X(num, id, fname) const char ICNd_##num[] PROGMEM = {fname,};
  ICON_CODE
#undef X

// 2. Put them in a table of string pointers, reads are via this table
#define X(num, id, fname) ICNd_##num,
const char* const ICNd_table[] PROGMEM = { 
  ICON_CODE 
  };
#undef X

#define X(num, id, icon_id) icon_id,
const char ICNd_table2[] PROGMEM = { 
  ICON_CODE2 
  };
#undef X

// NIGHT
#define ICON_CODE_NIGHT                             \
              X( 0,"01", "Moon")                    \
              X( 1,"02", "Cloud-Moon")              \
              X( 2,"03", "Cloud-Moon")              \
              X( 3,"04", "Cloud")                   \
              X( 4,"09", "Cloud-Rain-Moon-Alt")     \
              X( 5,"10", "Cloud-Rain-Moon")         \
              X( 6,"11", "Cloud-Lightning")         \
              X( 7,"13", "Cloud-Snow")              \
              X( 8,"50", "Cloud-Fog")     

#define ICON_CODE_NIGHT2                            \
              X( 0,"01", ICON_MOON_PNG)              \
              X( 1,"02", ICON_CLOUD_MOON_PNG)        \
              X( 2,"03", ICON_CLOUD_MOON_PNG)        \
              X( 3,"04", ICON_CLOUD_PNG)            \
              X( 4,"09", ICON_CLOUD_RAIN_MOON_ALT_PNG)   \
              X( 5,"10", ICON_CLOUD_RAIN_MOON_PNG)       \
              X( 6,"11", ICON_CLOUD_LIGHTNING_PNG)  \
              X( 7,"13", ICON_CLOUD_SNOW_PNG)       \
              X( 8,"50", ICON_CLOUD_FOG_PNG)       
// 1. Generate individual Icon file names
#define X(num, id, fname) const char ICNn_##num[] PROGMEM = {fname,};
  ICON_CODE_NIGHT
#undef X

// 2. Put them in a table of string pointers, reads are via this table
#define X(num, id, fname) ICNn_##num,
const char* const ICNn_table[] PROGMEM = { 
  ICON_CODE_NIGHT 
  };
#undef X

#define X(num, id, icon_id) icon_id,
const char ICNn_table2[] PROGMEM = { 
  ICON_CODE_NIGHT2 
  };
#undef X

typedef struct {
  long dt;
  int day;
  int max;
  int min;
  float speed;
  float rain;
  int humidity;
  char icon[4];
} DailyForecast;

// Note that the names here match the strings in the forecast list, but the order is not important
typedef struct {
  long dt;
  int temp;
  int humidity;
  float speed;
  float rain;
  char description[MAX_ELE_CHAR_LEN + 1];
  char icon[4];
} WeatherInfo;

// We have a total of 3 APIs to poll
typedef enum {
  APICurrentWeather,
  APIHourlyForecast,
  APIDailyForecast,
  MaxAPITypes
} APITypes;


void scp(void* out, char* in) {
  strcpy((char*)out, in);
}

void stol(long int* out, char* in) {
  *out = atol(in);
}

void stoi(int* out, char* in) {
  float f = atof(in);
  *out = (int)(f + 0.5f);
}

void stof(float* out, char* in) {
  *out = (float)atof(in);
}

void rtof(float* out, char* in) {
  char* pch = strchr(in, ':');
  if ((pch - in) < strlen(in)) {
    in = pch + 1; // Gobble chars until :
  }

  *out = (float)atof(in);
}

bool ss2(const char* scr, char* tgt) {
  for (int i = 0; i < strlen(tgt); i++) {
    if (scr[i] != tgt[i]) return false;
  }
  return true;
}

// Convert a weather icon code like "01d" to sd file name
void code2file(const char* code, char* file) {
  if (code[0] == '\0') {
    strcpy_P(file, (char*)pgm_read_word(&(ICNd_table[0])));
    return;
  }
  if (code[2] == 'd') {
#define X(num, id, fame) if(ss2(code, id) == true){ strcpy_P(file,(char*)pgm_read_word(&(ICNd_table[num]))); return; }
    ICON_CODE
#undef X
  }
  else {
#define X(num, id, fame) if(ss2(code, id) == true){ strcpy_P(file,(char*)pgm_read_word(&(ICNn_table[num]))); return; }
    ICON_CODE_NIGHT
#undef X
  }
}

uint8_t code2icon(const char* code) {
  if (code[0] == '\0') {
    return pgm_read_byte(&(ICNd_table2[0]));
  }
  if (code[2] == 'd') {
#define X(num, id, fame) if(ss2(code, id) == true){ return pgm_read_byte(&(ICNd_table2[num])); }
    ICON_CODE2
#undef X
  }
  else {
#define X(num, id, fame) if(ss2(code, id) == true){ return pgm_read_byte(&(ICNn_table2[num])); }
    ICON_CODE_NIGHT2
#undef X
  }
}

#endif
