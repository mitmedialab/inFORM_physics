/*
 *  ReliefIOManager.cpp
 *  Relief
 *
 *  Created by Daniel Leithinger on 3/3/10.
 *  Copyright 2010 MIT Media Lab. All rights reserved.
 *
 */

#include "ReliefIOManager.h"

ReliefIOManager::ReliefIOManager() {
	//loadSettings(RELIEFSETTINGS);
	
    mSerialConnections[0] = new ReliefSerial(SERIAL_PORT_0, SERIAL_BAUD_RATE);
    mSerialConnections[1] = new ReliefSerial(SERIAL_PORT_1, SERIAL_BAUD_RATE);
    mSerialConnections[2] = new ReliefSerial(SERIAL_PORT_2, SERIAL_BAUD_RATE);
    mSerialConnections[3] = new ReliefSerial(SERIAL_PORT_3, SERIAL_BAUD_RATE);
    mSerialConnections[4] = new ReliefSerial(SERIAL_PORT_4, SERIAL_BAUD_RATE);
    
    loadSettings();
}

ReliefIOManager::~ReliefIOManager() {
    for (int i = 0; i < NUM_SERIAL_CONNECTIONS; i++) {
        delete mSerialConnections[i];
    }
}

void ReliefIOManager::sendPinHeightToRelief(unsigned char pPinHeightToRelief [RELIEF_SIZE_X][RELIEF_SIZE_Y]) {
    
    // make sure the values are clipped at their extremes
    for (int i = 0; i < RELIEF_SIZE_X; i ++) {
		for (int j = 0; j < RELIEF_SIZE_Y; j ++) {
			if (pPinHeightToRelief[i][j] <= 15) {
				pPinHeightToRelief[i][j] = (unsigned char) 15;
			}
			else if (pPinHeightToRelief[i][j] >= 240) {
				pPinHeightToRelief[i][j] = (unsigned char) 240;
			}
		}
    }
    
    
    for (unsigned char i = 0; i < NUM_ARDUINOS; i++) { // iterate through each board
        
        bool frameChanged = false;
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            
            // copy the height values from the current frame values to the arduino pins
            reliefBoardValues[i].currentFramePinHeight[j] = pPinHeightToRelief[reliefBoardValues[i].pinCoordinates[j][0]][reliefBoardValues[i].pinCoordinates[j][1]];
            
            // add the value to the moving average of the last 100 frames
            reliefBoardValues[i].rollingAverageHeight[j] = (0.01 * (float)reliefBoardValues[i].currentFramePinHeight[j]) + (1.0 - 0.01) * (float)reliefBoardValues[i].rollingAverageHeight[j];
            
            // invert the values if needed (this affects boards mounted upside down)
            if (reliefBoardValues[i].invertHeight) reliefBoardValues[i].currentFramePinHeight[j] = 255 - reliefBoardValues[i].currentFramePinHeight[j];
            
            // compare the difference between the frames, copy and set a flag if it's different
            if(abs(reliefBoardValues[i].currentFramePinHeight[j] - reliefBoardValues[i].lastFramePinHeight[j]) > 0) {
                reliefBoardValues[i].lastFramePinHeight[j] = reliefBoardValues[i].currentFramePinHeight[j];
                frameChanged = true;
            }
        }
        
        // if the frame changes, update the Arduino
        if (frameChanged) {
            // activate board
            /*if(reliefBoardValues[i].isBoardIdle()) {
                unsigned char value[NUM_PINS_ARDUINO];
                for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
                    value[j] = (reliefBoardValues[i].rollingAverageHeight[j] > 1) ? 200/2 : 0;
                    reliefBoardValues[i].pinIsIdle[i] = (reliefBoardValues[i].rollingAverageHeight[j] > 1) ? true : false;
                }
                
                
                //sendValuesToBoard(TERM_ID_MAXSPEED, i + 1, value, reliefBoardValues[i].serialConnection);
            }*/
            
            /*unsigned char messageContents[8];
            messageContents[0] = TERM_ID_HEIGHT;
            messageContents[1] = i+1;
            for (int j = 0; j < 6; j++) {
                messageContents[j + 2] = reliefBoardValues[i].currentFramePinHeight[j];
            }
            mSerialConnections[reliefBoardValues[i].serialConnection]->writeMessage(messageContents);*/
            
            sendValuesToBoard(TERM_ID_HEIGHT, i + 1, reliefBoardValues[i].currentFramePinHeight, reliefBoardValues[i].serialConnection);
            
        }
        
        //quick test
        /*unsigned char value[NUM_PINS_ARDUINO];
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            value[j] = (reliefBoardValues[i].rollingAverageHeight[j] > 1) ? 100 : 0;
            reliefBoardValues[i].pinIsIdle[i] = (reliefBoardValues[i].rollingAverageHeight[j] > 1) ? true : false;
        }
        sendValuesToBoard(TERM_ID_MAXSPEED, i + 1, value, reliefBoardValues[i].serialConnection);*/
        
        /*else if (!reliefBoardValues[i].isBoardIdle()) {
            
            // deactivate if the moving average is below a certain threshold
            unsigned char value[NUM_PINS_ARDUINO];
            for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
                value[j] = (reliefBoardValues[i].rollingAverageHeight[j] > 1) ? 200/2 : 0;
                reliefBoardValues[i].pinIsIdle[i] = (value[j] == 0) ? true : false;
            }
            //sendValuesToBoard(TERM_ID_MAXSPEED, i + 1, value, reliefBoardValues[i].serialConnection);
        }*/
    }
}



void ReliefIOManager::sendValuesToBoard(unsigned char termId, unsigned char boardId, unsigned char value[NUM_PINS_ARDUINO], int serialConnection) {
    unsigned char messageContents[8];
    messageContents[0] = termId;
    messageContents[1] = boardId;
    for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
        messageContents[j + 2] = value[j];
    }
    mSerialConnections[serialConnection]->writeMessage(messageContents);
}

void ReliefIOManager::sendValueToAllBoards(unsigned char termId, unsigned char value) {
    
    unsigned char messageContents[8];
    messageContents[0] = (unsigned char) termId;
    messageContents[1] = (unsigned char) 0;
    for (int i = 0; i < NUM_PINS_ARDUINO; i++) {
        messageContents[i + 2] = (unsigned char) value;
    }
    
    for (int i = 0; i < NUM_SERIAL_CONNECTIONS; i ++) {
        mSerialConnections[i]->writeMessage(messageContents);
    }	
}

void ReliefIOManager::loadSettings(string pFileName) {
	
	mXML.loadFile(pFileName);
	
	// load pin lookup table
	int numArduinos = mXML.getNumTags("BOX");
	if (numArduinos == NUM_ARDUINOS) {
		for (int i = 0; i < numArduinos; i ++) {
			mXML.pushTag("BOX", i);
			int ArduinoID = mXML.getValue("ID", 0);
			for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
				int x = mXML.getValue("PIN:X", 0, j);
				int y = mXML.getValue("PIN:Y", 0, j);
				//mPinLookupTable[ArduinoID][j][0] = x;
				//mPinLookupTable[ArduinoID][j][1] = y;
			}
			mXML.popTag();
		}
	}
}

void ReliefIOManager::loadSettings() {
    
    
    // set up coordinates for 
    for (int i = 0; i < 60; i++) {
        // every 3rd and 4th board is mounted upside down
        reliefBoardValues[i].invertHeight = (((i/2)%2) == 0) ? false : true;
        // determine which serial connection each board is on
        if (i < 28) {
            reliefBoardValues[i].serialConnection = 0;
        } else {
            reliefBoardValues[i].serialConnection = 1;
        }
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            int currentRow = (int)(i / 2);
            int currentColumn = j + (i%2 * 6);
            reliefBoardValues[i].lastFramePinHeight[j] = 0;
            reliefBoardValues[i].currentFramePinHeight[j] = 0;
            reliefBoardValues[i].rollingAverageHeight[j] = 0;
            reliefBoardValues[i].pinIsIdle[j] = true;
            reliefBoardValues[i].pinCoordinates[j][0] = currentRow;
            reliefBoardValues[i].pinCoordinates[j][1] = currentColumn;
        }
    }
    
    for (int i = 60; i < 150; i++) {
        // every 4th, 5th and 6th board is mounted upside down
        
        reliefBoardValues[i].invertHeight = ((((i-60)/3)%2) == 0) ? false : true;
        if (i < 90) {
            reliefBoardValues[i].serialConnection = 2;
        } else if (i < 120){
            reliefBoardValues[i].serialConnection = 3;
        } else {
            reliefBoardValues[i].serialConnection = 4;
        }
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            int currentRow = (int)((i - 60) / 3);
            int currentColumn = 12 + j + ((i-60)%3 * 6);
            reliefBoardValues[i].lastFramePinHeight[j] = 0;
            reliefBoardValues[i].currentFramePinHeight[j] = 0;
            reliefBoardValues[i].rollingAverageHeight[j] = 0;
            reliefBoardValues[i].pinIsIdle[j] = true;
            reliefBoardValues[i].pinCoordinates[j][0] = currentRow;
            reliefBoardValues[i].pinCoordinates[j][1] = currentColumn;
        }
    }
    
    
    // switch the coordinates for boards that have been set out of order for serial connection 0 and 1:
    int wrongCols[8]= {0,3,4,23,26,27,28,29};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            for (int k = 0; k < 2; k++) {
                unsigned char temp = reliefBoardValues[(wrongCols[i]*2)+1].pinCoordinates[j][k];
                reliefBoardValues[(wrongCols[i]*2)+1].pinCoordinates[j][k] = reliefBoardValues[(wrongCols[i]*2)].pinCoordinates[j][k];
                reliefBoardValues[(wrongCols[i]*2)].pinCoordinates[j][k] = temp;
            }
        }
    }
    // change the row order according to how the Arduinos have been mounted
    int rowOrder[30] = {1,0,2,3,5,4,7,6,9,8,11,10,13,12,14,15,16,17,19,18,21,20,23,22,25,24,27,26,28,29};
    for (int i = 0; i < 15; i++) {
        if (rowOrder[i*2] > rowOrder[(i*2) + 1]) {
            for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
                for (int k = 0; k < 2; k++) {
                    unsigned char temp = reliefBoardValues[(i*4)+ 2].pinCoordinates[j][k];
                    reliefBoardValues[(i*4) + 2].pinCoordinates[j][k] = reliefBoardValues[i*4].pinCoordinates[j][k];
                    reliefBoardValues[i*4].pinCoordinates[j][k] = temp;
                    
                    temp = reliefBoardValues[(i*4)+ 3].pinCoordinates[j][k];
                    reliefBoardValues[(i*4) + 3].pinCoordinates[j][k] = reliefBoardValues[(i*4)+ 1].pinCoordinates[j][k];
                    reliefBoardValues[(i*4)+ 1].pinCoordinates[j][k] = temp;
                }
            }
        }
    }
    
    // invert pin order if the pins have been mounted inverted for first 60 boards
    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            reliefBoardValues[i].pinCoordinates[j][1] = (!reliefBoardValues[i].invertHeight) ? 11 - reliefBoardValues[i].pinCoordinates[j][1] : reliefBoardValues[i].pinCoordinates[j][1];
        }
    }
    // do the same for the remaining
    for (int i = 60; i < 150; i++) {
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            reliefBoardValues[i].pinCoordinates[j][1] = (!reliefBoardValues[i].invertHeight) ? 12 + 29 - reliefBoardValues[i].pinCoordinates[j][1] : reliefBoardValues[i].pinCoordinates[j][1];
        }
    }
    
    // print out the mapping
    /*
    for (int i = 0; i < NUM_ARDUINOS; i++) {
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            printf("%d,%d(%d); ", reliefBoardValues[i].pinCoordinates[j][0], reliefBoardValues[i].pinCoordinates[j][1], reliefBoardValues[i].invertHeight);
        }
        printf("\n");
    }*/
}
