/**
 * @file NexHardware.cpp
 *
 * The implementation of base API for using Nextion. 
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
#include <NexHardware.h>

#define NEX_RET_CMD_FINISHED (0x01)
#define NEX_RET_EVENT_LAUNCHED (0x88)
#define NEX_RET_EVENT_UPGRADED (0x89)
#define NEX_RET_EVENT_TOUCH_HEAD (0x65)
#define NEX_RET_EVENT_POSITION_HEAD (0x67)
#define NEX_RET_EVENT_SLEEP_POSITION_HEAD (0x68)
#define NEX_RET_CURRENT_PAGE_ID_HEAD (0x66)
#define NEX_RET_STRING_HEAD (0x70)
#define NEX_RET_NUMBER_HEAD (0x71)
#define NEX_RET_INVALID_CMD (0x00)
#define NEX_RET_INVALID_COMPONENT_ID (0x02)
#define NEX_RET_INVALID_PAGE_ID (0x03)
#define NEX_RET_INVALID_PICTURE_ID (0x04)
#define NEX_RET_INVALID_FONT_ID (0x05)
#define NEX_RET_INVALID_BAUD (0x11)
#define NEX_RET_INVALID_VARIABLE (0x1A)
#define NEX_RET_INVALID_OPERATION (0x1B)

//////// Funciones acomodadas by ARM

////// Ir a pagina
void goToPage(String page)
{
  String compValue = "page " + page;
  sendCommand(compValue.c_str());
}

////////Set valores a componentes

//set font color
void setColorTxt(String component, long txtcolor)
{
  String compValue = component + ".pco=" + txtcolor;
  sendCommand(compValue.c_str());
}
//set back color
void setBackColor(String component, long backcolor)
{
  String compValue = component + ".bco=" + backcolor;
  sendCommand(compValue.c_str());
}

//set pic
void setImagePic(String component, int pic)
{
  String compValue = component + ".pic=" + pic;
  sendCommand(compValue.c_str());
}

//set pic pushed
void setPushedPic(String component, int pic)
{
  String compValue = component + ".pic2=" + pic;
  sendCommand(compValue.c_str());
}

//set pic del fondo
void setBackPic(String component, int pic)
{
  String compValue = component + ".picc=" + pic;
  sendCommand(compValue.c_str());
}

//poner valor entero
void setComponentValue(String component, uint32_t value)
{
  String compValue = component + ".val=" + value; //Set component value
  sendCommand(compValue.c_str());
} //set_component_value

// poner texto
void setComponentText(String component, String txt)
{
  String componentText = component + ".txt=\"" + txt + "\""; //Set Component text
  sendCommand(componentText.c_str());
} //end set_component_txt

// set valores a waveform
void setWaveVal(uint8_t cid, uint8_t ch, uint8_t val)
{
  String componentText = "add " + String(cid) + "," + String(ch) + "," + String(val); //Set Component text
  sendCommand(componentText.c_str());
}

////////////Get valores de componentes

//para obtener strings
String listen()
{ //returns generic

  char _bite;
  char _end = 0xff; //end of file x3
  String cmd;
  int countEnd = 0;

  while (nexSerial.available() > 0)
  {
    delay(10);
    if (nexSerial.available() > 0)
    {
      _bite = nexSerial.read();
      cmd += _bite;
      if (_bite == _end)
      {
        countEnd++;
      } //end if
      if (countEnd == 3)
      {
        break;
      } //end if
    }   //end if
  }     //end while

  String temp = ""; //detectar comando, sino devuelve ""
  switch (cmd[0])
  {
  case 'e':       //0x65   Same than default -.-
    countEnd = 0; //Revision for not include last space " "
    for (uint8_t i = 0; i < cmd.length(); i++)
    {
      if (cmd[i] == _end)
      {
        countEnd++;
      }                            //end if
      temp += String(cmd[i], HEX); //add hexadecimal value
      if (countEnd == 3)
      {
        return temp;
      }            //end if
      temp += " "; //For easy visualization
    }              //end for
    break;
  case 'f': //0x66
    //Serial.print(String(cmd[1], HEX));
    return String(cmd[1], DEC);
    break;
  case 'g': //0x67
    cmd = String(cmd[2], DEC) + "," + String(cmd[4], DEC) + "," + String(cmd[5], DEC);
    return cmd;
    break;
  case 'h': //0x68
    cmd = String(cmd[2], DEC) + "," + String(cmd[4], DEC) + "," + String(cmd[5], DEC);
    cmd = "68 " + cmd;
    return cmd;
    break;
  case 'p': //0x70 Aca se recibe el string
    cmd = cmd.substring(1, cmd.length() - 3);
    //cmd = "70 " + cmd; //ingnoro el 70
    return cmd;
    break;
  default:
    //	cmd += String(b, HEX);
    //if(ff == 3){break;}//end if
    //cmd += " ";//
    return cmd; //
    break;
  } //end switch
  return "";

} //end listen

// get texto
String getComponentText(String component)
{
  String tempStr = "get " + component + ".txt";
  sendCommand(tempStr.c_str());
  tempStr = "";
  //rutina para recibir bien
  unsigned long i = 0;
  while (tempStr == "")
  {
    tempStr = listen(); //sin timeout
    if (tempStr == "")
    {
      if (i > 150000)
        break;
      i++;
    }
  }

  return tempStr;
} //getComponentText

//Get value
uint32_t getComponentValue(String component)
{
  String getValue = "get " + component + ".val"; //Get componetn value
  uint32_t value = 0;
  sendCommand(getValue.c_str());
  while (false == recvRetNumber(&value)) //mientras sea failed
  {
  }
  return value;
} //get_component_value */

////////// Las que ya venian con la libreria original

/** Receive uint32_t data.
 * @param number - save uint32_t data. 
 * @param timeout - set timeout time. 
 * @retval true - success. 
 * @retval false - failed.
 */
bool recvRetNumber(uint32_t *number, uint32_t timeout)
{
  bool ret = false;
  uint8_t temp[8] = {0};

  if (!number)
  {
    goto __return;
  }

  nexSerial.setTimeout(timeout);
  if (sizeof(temp) != nexSerial.readBytes((char *)temp, sizeof(temp)))
  {
    goto __return;
  }

  if (temp[0] == NEX_RET_NUMBER_HEAD && temp[5] == 0xFF && temp[6] == 0xFF && temp[7] == 0xFF)
  {
    *number = ((uint32_t)temp[4] << 24) | ((uint32_t)temp[3] << 16) | (temp[2] << 8) | (temp[1]);
    ret = true;
  }

__return:

  if (ret)
  {
    dbSerialPrint("recvRetNumber :");
    dbSerialPrintln(*number);
  }
  else
  {
    dbSerialPrintln("recvRetNumber err");
  }

  return ret;
} // */

/*
 * Receive string data. 
 * 
 * @param buffer - save string data. 
 * @param len - string buffer length. 
 * @param timeout - set timeout time. 
 *
 * @return the length of string buffer.
 *
uint16_t recvRetString(char *buffer, uint16_t len, uint32_t timeout)
{
    uint16_t ret = 0;
    bool str_start_flag = false;
    uint8_t cnt_0xff = 0;
    String temp = String("");
    uint8_t c = 0;
    long start;

    if (!buffer || len == 0)
    {
        goto __return;
    }
    
    start = millis();
    while (millis() - start <= timeout)
    {
        while (nexSerial.available())
        {
            c = nexSerial.read();
            if (str_start_flag)
            {
                if (0xFF == c)
                {
                    cnt_0xff++;                    
                    if (cnt_0xff >= 3)
                    {
                        break;
                    }
                }
                else
                {
                    temp += (char)c;
                }
            }
            else if (NEX_RET_STRING_HEAD == c)
            {
                str_start_flag = true;
            }
        }
        
        if (cnt_0xff >= 3)
        {
            break;
        }
    }

    ret = temp.length();
    ret = ret > len ? len : ret;
    strncpy(buffer, temp.c_str(), ret);
    
__return:

    dbSerialPrint("recvRetString[");
    dbSerialPrint(temp.length());
    dbSerialPrint(",");
    dbSerialPrint(temp);
    dbSerialPrintln("]");

    return ret;
}// */

/* Send command to Nextion.
 * @param cmd - the string of command.
 */
void sendCommand(const char *cmd)
{
  while (nexSerial.available())
  {
    nexSerial.read();
  }

  nexSerial.print(cmd);
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
}

/** Command is executed successfully.
 * @param timeout - set timeout time.
 * @retval true - success.
 * @retval false - failed. 
 */
bool recvRetCommandFinished(uint32_t timeout)
{
  bool ret = false;
  uint8_t temp[4] = {0};

  nexSerial.setTimeout(timeout);
  if (sizeof(temp) != nexSerial.readBytes((char *)temp, sizeof(temp)))
  {
    ret = false;
  }

  if (temp[0] == NEX_RET_CMD_FINISHED && temp[1] == 0xFF && temp[2] == 0xFF && temp[3] == 0xFF)
  {
    ret = true;
  }

  if (ret)
  {
    dbSerialPrintln("recvRetCommandFinished ok");
  }
  else
  {
    dbSerialPrintln("recvRetCommandFinished err");
  }

  return ret;
} //*/

bool nexInit(void)
{
  bool ret1 = false;
  bool ret2 = false;

  dbSerialBegin(115200);
  nexSerial.begin(9600);

  //si se resetea, volvemos el nextion al inicio
  sendCommand("");
  sendCommand("bkcmd=1");
  ret1 = recvRetCommandFinished();
  sendCommand("page 0");
  ret2 = recvRetCommandFinished();

  return ret1 && ret2;
}

void nexLoop(void (*atendedor)(uint8_t, uint8_t))
{
  static uint8_t __buffer[10];
  Atendedor_f llamador = atendedor;
  uint16_t i;
  uint8_t c;

  while (nexSerial.available())
  {

    delay(10);
    c = nexSerial.read();

    if (NEX_RET_CURRENT_PAGE_ID_HEAD == c) // entra ante Sendme (by ARMHC)
    {
      if (nexSerial.available() >= 4)
      {
        __buffer[0] = c;
        for (i = 1; i < 5; i++)
        {
          __buffer[i] = nexSerial.read();
        }
        __buffer[5] = 0x99; // si es sendme, obj: 99 (en hexa!! )
        __buffer[6] = 0x01; //push: se aplica como si fuera un touch push de una pag

        if (0xFF == __buffer[2] && 0xFF == __buffer[3] && 0xFF == __buffer[4])
        {
          llamador(__buffer[1], __buffer[5]); // si es sendme, obj: 99 (en hexa!! )
        }
      }
    }

    if (NEX_RET_EVENT_TOUCH_HEAD == c) //entra por tochs events
    {
      if (nexSerial.available() >= 6)
      {
        __buffer[0] = c;
        for (i = 1; i < 7; i++)
        {
          __buffer[i] = nexSerial.read();
        }
        __buffer[i] = 0x00;

        if (0xFF == __buffer[4] && 0xFF == __buffer[5] && 0xFF == __buffer[6])
        {
          llamador(__buffer[1], __buffer[2]);
        }
      }
    }
  }
}
