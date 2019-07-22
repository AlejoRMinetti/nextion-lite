/**
 * @file NexHardware.h
 *
 * The definition of base API for using Nextion. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2015/8/11
 * @copyright 
 * Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#ifndef __NEXHARDWARE_H__
#define __NEXHARDWARE_H__
#include <Arduino.h>
#include <Nextion.h>

//// para atender eventos
typedef void (*Atendedor_f)(uint8_t, uint8_t);

//////// Funciones acomodadas by ARM

////////Set valores a componentes
// Ir a pagina
void goToPage(String page);
//set back color
void setBackColor(String component, long backcolor);
//set font color
void setColorTxt(String component, long txtcolor);
//set pic
void setImagePic(String component, int pic);
//set pic del fondo
void setBackPic(String component, int pic);
//set pic pushed
void setPushedPic(String component, int pic);
//poner valor entero
void setComponentValue(String component, uint32_t value);
// poner texto
void setComponentText(String component, String txt);
// set valores a waveform
void setWaveVal(uint8_t cid, uint8_t ch, uint8_t number);

////////////Get valores de componentes

//para obtener strings
String listen();
// get texto
String getComponentText(String component);
//Get value
uint32_t getComponentValue(String component);

/** Init Nextion.
 * @return true if success, false for failure. 
 */
bool nexInit(void);

/**
 * Listen touch event and calling callbacks attached before.
 * version modificada para LiteARM Arduino Nextion
 */
void nexLoop(void (*atendedor)(uint8_t, uint8_t));

bool recvRetNumber(uint32_t *number, uint32_t timeout = 100);
//uint16_t recvRetString(char *buffer, uint16_t len, uint32_t timeout = 100);
void sendCommand(const char* cmd);
bool recvRetCommandFinished(uint32_t timeout = 100);

#endif /* #ifndef __NEXHARDWARE_H__ */
