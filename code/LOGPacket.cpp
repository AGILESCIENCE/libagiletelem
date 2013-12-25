/***************************************************************************
                          LOGPacket.cpp  -  description
                             -------------------
    copyright            : (C) 2013 Andrea Bulgarelli
    email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/


#include "LOGPacket.h"
#include "OutputFile.h"
#include "InputFile.h"
#include "PacketDataField.h"
#include "SDFRBlock.h"
#include "OutputPacketStream.h"



AGILETelem::LOGPacket::LOGPacket(string packetConfig, string tmInputFileName, string tmOutputFileName) : AGILEPacket(packetConfig, tmInputFileName, tmOutputFileName, 2) {

}

AGILETelem::LOGPacket::LOGPacket(string packetConfig) : AGILEPacket(packetConfig, 2) {


}

AGILETelem::LOGPacket::~LOGPacket() {

}

