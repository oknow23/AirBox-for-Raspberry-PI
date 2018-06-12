/*
 * Simple example 
 * Author:  MiLi
 * Email:   oknow23@gmail.com
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "sht31-d.h"
#include "KQM2801AI.h"

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

void printtempandhumidity(int file);
void printstatus(int file);
void printserialnum(int file);
void printBitStatus(uint16_t stat);
char cmd[128];

void printusage(char *selfname)
{
  printf("%s (options)\n", selfname);
  printf("\ts - print status\n");
  printf("\tr - soft reset\n");
  printf("\tc - clear status\n");
  printf("\te - enable heater\n");
  printf("\td - disable heater\n");
  printf("\tn - print serial#\n");
  printf("\tp - print temp & humid\n");
  printf("\t-h this\n\n");
  printf("Example to print temperaturehum & humidity, serial number & status\n  %s p n s\n\n", selfname);
}

int comfort_level(float T/* Tempture */,float RH/* Humidity */,char *level)
{
  float THI;  //Temperature Humidity Index
  float Td;   //Dew point temperature

  Td = pow(RH,1/8)*(112+0.9*T)+0.1*T-112;

  THI = T-0.55*(1-( exp((17.269*Td)/(Td+237.3)) / exp((17.269*T)/(T+237.3)) ))*(T-14);
  printf("THI:%f\n",THI);
  if( THI <= 10 ){
    sprintf(level,"Cold+"); //very cold
  }
  else if( THI <= 15 ){
    sprintf(level,"Cold "); //0
  }
  else if( THI <= 19 ){
    sprintf(level,"Cool "); //Slightly cold
  }
  else if( THI <= 26 ){
    sprintf(level,"Comfo"); //Comfortable
  }
  else if( THI <= 30 ){
    sprintf(level,"Heat "); //0
  }
  else if( THI >= 31){
    sprintf(level,"Heat+"); //Heatstroke
    
    /* linebot msg */
    sprintf(cmd,"python2 line_bot.py '注意!: 溫度:%.0f°C 請開冷氣!' ",T);
    system(cmd);
  }

  return 0;
}

int humidity_level(float humi,char *level)
{
  if( humi <= 40 ){
    sprintf(level,"Dry"); //Dry
  }
  else if( humi <= 70 ){
    sprintf(level,"ok "); //ok
  }
  else if( humi >= 71 ){
    sprintf(level,"Wet"); //wet

    /* linebot msg */
    sprintf(cmd,"python2 line_bot.py '注意!: 濕度:%.0f% 請開除濕機!' ",humi);
    system(cmd);
  }

  return 0;
}

int display(int file)
{
  float tempC;
  float tempF;
  float humid;
  char buffer1[64];
  char buffer2[64];
 
 /*  KQM2801 density */
  float density;
  char air_level[64] = "";
  char cofort_level[64] = "";
  char humi_level[64] = "";

  int rtn = getDensity(&density,air_level);
  
  if ( rtn != KQM2801_OK && rtn != KQM2801_CRC_CHECK_FAILED) {
    printf("ERROR:- Get air failed\n!");
  }
  else{
    if ( rtn == KQM2801_CRC_CHECK_FAILED) {
      printf("WARNING:- Get status CRC check failed, don't trust results\n");
    }

    printf("Density = %.1lf PPM level:%s\n",density,air_level);
  }

  /* initial time */
  char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"}; 
  time_t timep; 
  struct tm *p; 
  time(&timep); 

  rtn = gettempandhumidity(file, &tempC, &humid);
  
  if ( rtn != SHT31_OK && rtn != SHT31_CRC_CHECK_FAILED) {
    printf("ERROR:- Get temp/humidity failed\n!");
    return rtn;
  }
    
  if ( rtn == SHT31_CRC_CHECK_FAILED) {
    printf("WARNING:- Get status CRC check failed, don't trust results\n");
  }
    
  if ( rtn == SHT31_OK || rtn == SHT31_CRC_CHECK_FAILED) {
    /* show temperture/humidity */
    snprintf(buffer1,64,"./lcdi2c -b 1 -x 0 -y 0 \"%.0f%cC  %.0f%% %.1lfPPM\" ",tempC,0xDF,humid,density);
    system(buffer1);

    /* show time date */
    // p=gmtime(&timep);
    // snprintf(buffer2,64,"./lcdi2c -b 1 -x 0 -y 1 \"%02d:%02d:%02d\" ",p->tm_hour, p->tm_min, p->tm_sec);

    /* show Air quarity */
    comfort_level(tempC,humid,cofort_level);
    humidity_level(humid,humi_level);
    snprintf(buffer2,64,"./lcdi2c -b 1 -x 0 -y 1 \"%s %s %s\" ",cofort_level,humi_level,air_level);
    system(buffer2);
  }

  return rtn;
}

int main(int argc, char *argv[])
{
  int file;
  int i;
  
  file = sht31_open(SHT31_INTERFACE_ADDR, SHT31_DEFAULT_ADDR);

  if (argc <= 1) {
    //initial lcd
    system("./lcdi2c -i");

    //initial need delay time
    sleep(3);

    printusage(argv[0]);
    while(1){
      display(file);
      sleep(1);
    }
    // exit (EXIT_SUCCESS);
  }
  
  
  for (i = 1; i < argc; i++)
  {
    if (strcmp (argv[i], "-h") == 0)
    {
      printusage(argv[0]);
      sht31_close(file);
      exit (EXIT_SUCCESS);
    } else if (strcmp (argv[i], "s") == 0) {
      printstatus(file);
    } else if (strcmp (argv[i], "r") == 0) {
      printf("Soft reset :- ");
      (softreset(file)==SHT31_OK)?printf("OK\n"):printf("Failed\n");
    } else if (strcmp (argv[i], "c") == 0) {
      printf("Status cleared :- ");
      (clearstatus(file)==SHT31_OK)?printf("OK\n"):printf("Failed\n");
    } else if (strcmp (argv[i], "e") == 0) {
      printf("Heater enabled :- ");
      (enableheater(file)==SHT31_OK)?printf("OK\n"):printf("Failed\n");
    } else if (strcmp (argv[i], "d") == 0) {
      printf("Heater disabled :- ");
      (disableheater(file)==SHT31_OK)?printf("OK\n"):printf("Failed\n");
    } else if (strcmp (argv[i], "p") == 0) {
      printtempandhumidity(file);
    } else if (strcmp (argv[i], "n") == 0) {
      printserialnum(file);
    } else {
      printf("ERROR :- '%s' unknown option\n",argv[i]);
    }
      
    delay(30);
  }
  
  sht31_close(file);
  
  return 0;
}

void printtempandhumidity(int file)
{
  float tempC;
  float tempF;
  float humid;
  
  int rtn = gettempandhumidity(file, &tempC, &humid);
  
  if ( rtn != SHT31_OK && rtn != SHT31_CRC_CHECK_FAILED) {
    printf("ERROR:- Get temp/humidity failed\n!");
    return;
  }
    
  if ( rtn == SHT31_CRC_CHECK_FAILED) {
    printf("WARNING:- Get status CRC check failed, don't trust results\n");
  }
    
  if ( rtn == SHT31_OK || rtn == SHT31_CRC_CHECK_FAILED) {
    tempF =  tempC * 9 / 5 + 32;
    printf("Temperature %.2fc - %.2ff\n",tempC,tempF);
    printf("Humidity %.2f%%\n",humid);
  }
}

void printBitStatus(uint16_t stat)
{
  printf("Status :-\n");
  printf("    Checksum status %d\n", CHECK_BIT(stat,0));
  printf("    Last command status %d\n", CHECK_BIT(stat,1));
  printf("    Reset detected status %d\n", CHECK_BIT(stat,4));
  printf("    'T' tracking alert %d\n", CHECK_BIT(stat,10));
  printf("    'RH' tracking alert %d\n", CHECK_BIT(stat,11));
  printf("    Heater status %d\n", CHECK_BIT(stat,13));
  printf("    Alert pending status %d\n", CHECK_BIT(stat,15));
}

void printstatus(int file)
{
  uint16_t stat;

  int rtn = getstatus(file, &stat);
  
  if ( rtn != SHT31_OK && rtn != SHT31_CRC_CHECK_FAILED) {
     printf("ERROR:- Get status failed!\n");
     return;
  }
  
  if ( rtn == SHT31_CRC_CHECK_FAILED) {
    printf("WARNING:- Get status CRC check failed, don't trust results\n");
  }
    
  if ( rtn == SHT31_OK || rtn == SHT31_CRC_CHECK_FAILED) {
    printBitStatus(stat);
  }
}


void printserialnum(int file)
{
  uint32_t serialNo;

  int rtn = getserialnum(file, &serialNo);
  
  if ( rtn == SHT31_CRC_CHECK_FAILED) {
    printf("WARNING:- Get getserial# CRC check failed, don't trust results\n");
  }
    
  if ( rtn == SHT31_OK || rtn == SHT31_CRC_CHECK_FAILED) {
    printf("Serial# = 0x%x\n",serialNo);
  }
  
}


