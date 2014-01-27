/***************************************************************************
                          RATPacket.cpp  -  description
                             -------------------
    copyright            : (C) 2014 Andrea Bulgarelli
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


#include "RATPacket.h"
#include "packet/OutputFile.h"
#include "packet/InputFile.h"
#include "packet/PacketDataField.h"
#include "packet/SDFRBlock.h"
#include "packet/OutputPacketStream.h"

#define IDRAT 3

AGILETelem::RATPacket::RATPacket(string packetConfig, string tmInputFileName, string tmOutputFileName) : AGILEPacket(packetConfig, tmInputFileName, tmOutputFileName, IDRAT) {

}

AGILETelem::RATPacket::RATPacket(string packetConfig) : AGILEPacket(packetConfig, IDRAT) {


}

AGILETelem::RATPacket::~RATPacket() {

}

/// Writing the packet
void AGILETelem::RATPacket::writePacket() {
	//initialize some filds
	//ID
	hea_out->setFieldValue(2, IDRAT);
	AGILEPacket::writePacket();
}

/// Printing the created packet
void AGILETelem::RATPacket::printPacket_output() {
	AGILEPacket::printPacket_output();
}

void AGILETelem::RATPacket::setTime(double time) {
	dfh_out->setFieldValue_5_2(0, time);	
}
	
double AGILETelem::RATPacket::getTime() {
	return dfh_in->getFieldValue_5_2(0);
}

