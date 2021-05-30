#include <Wire.h>
#include "DS1307.h"
#include "Arduino.h"


//constructor
DS1307::DS1307(){
  Wire.begin();
}

//initial
void DS1307::setAddress(){
  uint8_t address = 0;
  while(true){
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0)break;
    else address++; 
  }
  _address = address; 
 }

void DS1307::setAllDate(uint8_t sg, uint8_t mn, uint8_t hr, int ampm, uint8_t dWeek, uint8_t date, uint8_t mounth, uint8_t year){ //Am/pm
  Wire.beginTransmission(_address);
  Wire.write(0);
  Wire.write(decBin_date(sg));                   //segundos
  Wire.write(decBin_date(mn));                   //minutos
  Wire.write(decBin_12Hrs(hr, ampm == 1? true : false));            //Horas true->Pm, false->Am
  Wire.write(decBin_date(dWeek));                //dia de la semana
  Wire.write(decBin_date(date));                 //dia del mes
  Wire.write(decBin_date(mounth));               //mes
  Wire.write(decBin_date(year));                 //año
  Wire.endTransmission();
}

void DS1307::setAllDate(uint8_t sg, uint8_t mn, uint8_t hr, uint8_t dWeek, uint8_t date, uint8_t mounth, uint8_t year){ //HH
  Wire.beginTransmission(_address);
  Wire.write(0);
  Wire.write(decBin_date(sg));                   //segundos
  Wire.write(decBin_date(mn));                   //minutos
  Wire.write(decBin_24Hrs(hr));                  //Horas hh
  Wire.write(decBin_date(dWeek));                //dia de la semana
  Wire.write(decBin_date(date));                 //dia del mes
  Wire.write(decBin_date(mounth));               //mes
  Wire.write(decBin_date(year));                 //año
  Wire.endTransmission();
}

void DS1307::setTime(bool pmam, uint8_t hr, uint8_t mn, uint8_t sg){
  Wire.beginTransmission(_address);
  Wire.write(0);
  Wire.write(decBin_date(sg));                   //segundos
  Wire.write(decBin_date(mn));                   //minutos
  Wire.write(decBin_12Hrs(hr, pmam));            //Horas true->Pm, false->Am
  Wire.endTransmission();  
}

void DS1307::setTime(uint8_t hr, uint8_t mn, uint8_t sg){
  Wire.beginTransmission(_address);
  Wire.write(0);
  Wire.write(decBin_date(sg));                   //segundos
  Wire.write(decBin_date(mn));                   //minutos
  Wire.write(decBin_24Hrs(hr));                  //Horas true->Pm, false->Am
  Wire.endTransmission();  
}

void DS1307::setDate(uint8_t dWeek, uint8_t date, uint8_t mounth, uint8_t year){
  Wire.beginTransmission(_address);
  Wire.write(3);
  Wire.write(decBin_date(dWeek));                //dia de la semana
  Wire.write(decBin_date(date));                 //dia del mes
  Wire.write(decBin_date(mounth));               //mes
  Wire.write(decBin_date(year));                 //año
  Wire.endTransmission();  
}

bool DS1307::onBus(){
  Wire.beginTransmission(_address);  
  bool t  = Wire.endTransmission() == 0 ? true : false;   
  return t;  
}

uint8_t DS1307::getDate(uint8_t* date){
  byte t[7]; 
  Wire.beginTransmission(_address);
  Wire.write(0);
  Wire.endTransmission(false);     
  Wire.requestFrom(_address, sizeof(t));

  for (uint8_t i = 0; i < sizeof(t); i++){
    t[i] = Wire.read(); 
  }
  *date = binDec_date(t[0]);
  *(date+1) = binDec_date(t[1]);
  *(date+2) = binDec_Hrs(t[2]) == 0 ? binDec_24Hrs(t[2]) : binDec_12Hrs(t[2]);
  *(date+3) = binDec_date(t[3]);
  *(date+4) = binDec_date(t[4]);
  *(date+5) = binDec_date(t[5]);
  *(date+6) = binDec_date(t[6]);
  Wire.endTransmission();
  return binDec_Hrs(t[2]);
}

void DS1307::getAllDate(uint8_t* date){
  byte t[8]; 
  Wire.beginTransmission(_address);
  Wire.write(0);
  Wire.endTransmission(false);     
  Wire.requestFrom(_address, sizeof(t));

  for (uint8_t i = 0; i < sizeof(t); i++){
    t[i] = Wire.read(); 
  }
  *date = binDec_date(t[0]);
  *(date+1) = binDec_date(t[1]);
  *(date+2) = binDec_Hrs(t[2]) == 0 ? binDec_24Hrs(t[2]) : binDec_12Hrs(t[2]);
  *(date+3) = binDec_Hrs(t[2]); 
  *(date+4) = binDec_date(t[3]);
  *(date+5) = binDec_date(t[4]);
  *(date+6) = binDec_date(t[5]);
  *(date+7) = binDec_date(t[6]);
  
  Wire.endTransmission();

}

uint8_t DS1307::getAddress(){
  return _address;
}

//escritura
byte DS1307::decBin_date(uint8_t decimal){
  byte bin = (byte)( (decimal/10) << 4 ) + (byte)( decimal % 0x0a );
  return bin;  
}
byte DS1307::decBin_12Hrs(uint8_t decimal, bool ampm){
  byte ap = ampm ? 0x6 : 0x4; 
  byte bin = (byte)( ((decimal/10) + ap) << 4 ) + (byte)( decimal % 0x0a );
  return bin;   
}
byte DS1307::decBin_24Hrs(uint8_t decimal){
  byte bin = (byte)( (decimal/10) << 4 ) + (byte)( decimal % 0x0a );
  return bin;  
}

//lectura
uint8_t DS1307::binDec_date(byte bin){
  uint8_t dec = (uint8_t)( bin >> 4 )*10 + (uint8_t)( bin & 0xF );
  return dec;  
}

uint8_t DS1307::binDec_12Hrs(byte bin){
  byte bt = ((bin >> 4) & 0x02) == 0x02 ? 0x06 : 0x04;
  uint8_t dec = (uint8_t)(( bin >> 4 ) - bt)*10 + (uint8_t)( bin & 0xF );
  return dec;  
}

uint8_t DS1307::binDec_24Hrs(byte bin){
  uint8_t dec = (uint8_t)( bin >> 4 )*10 + (uint8_t)( bin & 0xF );
  return dec;  
}

uint8_t DS1307::binDec_Hrs(byte bin){
  uint8_t cn = 0;//"HH";
  if (((bin >> 4) & 0x04) == 0x04){
    cn = (((bin >> 4) & 0x02) == 0x02) ? 1 : 2;//"Pm" : "Am";
  }  
  return cn;  
}
