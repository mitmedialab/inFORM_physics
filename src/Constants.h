/*
 *  Constants.h
 *  Relief
 *
 *  Created by Daniel Leithinger on 2/27/10.
 *  Copyright 2010 MIT Media Lab. All rights reserved.
 *
 */

#ifndef _CONSTANTS
#define _CONSTANTS	

#define SCREEN1_WIDTH 500
#define SCREEN1_HEIGHT 500
#define SCREEN2_WIDTH 500
#define SCREEN2_HEIGHT 500

#define	RELIEF_SIZE_X	  30
#define	RELIEF_SIZE_Y	  30

#define RELIEF_PROJECTOR_SIZE_X 900
#define RELIEF_PROJECTOR_SIZE_Y 900

#define PIN_PIXEL_SIZE 30

#define RELIEF_PROJECTOR_OFFSET_X 369
#define RELIEF_PROJECTOR_OFFSET_Y 285//311
#define RELIEF_PROJECTOR_SCALED_SIZE_X 607
#define RELIEF_PROJECTOR_SCALED_SIZE_Y 607

#define TOPVIEW_SCALE_X 33.0f
#define TOPVIEW_SCALE_Y 33.0f
#define TOPVIEW_SCALE_Z 33.0f

#define TERM_ID_HEIGHT (unsigned char)246
#define TERM_ID_GAIN_P (unsigned char)247
#define TERM_ID_GAIN_I (unsigned char)248
#define TERM_ID_MAX_I (unsigned char)249
#define TERM_ID_DEADZONE (unsigned char)250
#define TERM_ID_GRAVITYCOMP (unsigned char)251
#define TERM_ID_MAXSPEED (unsigned char)252

#define BASE_HEIGHT 70
#define MAX_HEIGHT 245

#define PROJECTOR_SCALEFACTOR 30

#define NUM_SERIAL_CONNECTIONS 5
#define NUM_ARDUINOS 150
#define NUM_PINS_ARDUINO 6


//#define SERIAL_PORT_0 "/dev/tty.usbserial-A101NU5I"
#define SERIAL_PORT_0 "/dev/tty.usbserial-A4011F1E"
#define SERIAL_PORT_0_FIRST_ID 1
#define SERIAL_PORT_1 "/dev/tty.usbserial-A101NU5H"
#define SERIAL_PORT_1_FIRST_ID 29
#define SERIAL_PORT_2 "/dev/tty.usbserial-A101NU5G"
#define SERIAL_PORT_2_FIRST_ID 61
#define SERIAL_PORT_3 "/dev/tty.usbserial-A101NU5F"
#define SERIAL_PORT_3_FIRST_ID 91
#define SERIAL_PORT_4 "/dev/tty.usbserial-A101NUAR"
#define SERIAL_PORT_4_FIRST_ID 121
#define SERIAL_BAUD_RATE 115200

//#define ARDUINO_GAIN_P 150
//#define ARDUINO_GAIN_I 35
//#define ARDUINO_MAX_I  60
//#define ARDUINO_DEADZONE 0

#define RELIEFSETTINGS "reliefSettings.xml"

#define PROJECTOR_RAW_RESOLUTION_X 1400
#define PROJECTOR_RAW_RESOLUTION_Y 1050
#define SEAN_SCREEN_RESOLUTION_X 1680
#define SEAN_SCREEN_RESOLUTION_Y 1050
#define DANIEL_SCREEN_RESOLUTION_X 1440
#define DANIEL_SCREEN_RESOLUTION_Y 1050
#define CINEMA_DISPLAY_OFFSET 880


#define CINEMA_DISPLAY 0

#define SEAN_SCREEN 1

#define USE_KINECT 1

#define DEBUG 1

#endif