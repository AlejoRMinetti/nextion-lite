/**
 * @file Nextion.h
 *
Lite version by ARM
 */
#ifndef __NEXTION_H__
#define __NEXTION_H__

#include <NexHardware.h>
#include "Arduino.h"

/* * Define nexSerial for communicate with Nextion touch panel.
 * Serial solo para emular, deshabilitando debugSerial (Cambiar en aplicacion real)
 */
#define nexSerial Serial

//#define DEBUG_SERIAL_ENABLE

/* Define dbSerial for the output of debug messages.  */
#define dbSerial Serial3 // SerialUSB usa el Native port del DUE

#ifdef DEBUG_SERIAL_ENABLE
#define dbSerialPrint(a)    dbSerial.print(a)
#define dbSerialPrintln(a)  dbSerial.println(a)
#define dbSerialBegin(a)    dbSerial.begin(a)
#else
#define dbSerialPrint(a)    do{}while(0)
#define dbSerialPrintln(a)  do{}while(0)
#define dbSerialBegin(a)    do{}while(0)
#endif


#endif /* #ifndef __NEXTION_H__ */
