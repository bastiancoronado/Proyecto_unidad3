/* 
    DS1307.h - Library for convert from dec to bin codification, 
    and initialize the DS1307 divice with other library, Wire.h.   
    Created by Sebastian Coroonado Berrio - 14/05/21.  
*/

#ifndef DS1307_h
#define DS1307_h

#include "Arduino.h"

class DS1307
{
  public:
    //constructor
    DS1307();
    
    //initial
    void setAddress();    
    void setAllDate(uint8_t sg, uint8_t mn, uint8_t hr, int ampm, uint8_t dWeek, uint8_t date, uint8_t mounth, uint8_t year); //Am/pm
    void setAllDate(uint8_t sg, uint8_t mn, uint8_t hr, uint8_t dWeek, uint8_t date, uint8_t mounth, uint8_t year); //HH
    void setTime(bool pmam, uint8_t hr, uint8_t mn, uint8_t sg);
    void setTime(uint8_t hr, uint8_t mn, uint8_t sg);
    void setDate(uint8_t dWeek, uint8_t date, uint8_t mounth, uint8_t year);
    bool onBus();
    uint8_t getDate(uint8_t* date);
    void getAllDate(uint8_t* date);
    uint8_t getAddress();    

  private:
    //escritura
    byte decBin_date(uint8_t decimal);
    byte decBin_12Hrs(uint8_t decimal, bool ampm); 
    byte decBin_24Hrs(uint8_t decimal);
        
    //lectura
    uint8_t binDec_date(byte bin);
    uint8_t binDec_12Hrs(byte bin);
    uint8_t binDec_24Hrs(byte bin);
    uint8_t binDec_Hrs(byte bin);
        
    //variables
    uint8_t _address;    
    
};

#endif
