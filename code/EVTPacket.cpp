/***************************************************************************
                          EVTPacket.cpp  -  description
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


#include "EVTPacket.h"
#include "packet/OutputFile.h"
#include "packet/InputFile.h"
#include "packet/PacketDataField.h"
#include "packet/SDFRBlock.h"
#include "packet/OutputPacketStream.h"



AGILETelem::EVTPacket::EVTPacket(string packetConfig, string tmInputFileName, string tmOutputFileName) : AGILEPacket(packetConfig, tmInputFileName, tmOutputFileName, 1) {

}

AGILETelem::EVTPacket::EVTPacket(string packetConfig) : AGILEPacket(packetConfig, 1) {


}

AGILETelem::EVTPacket::~EVTPacket() {

}

/// Writing the packet
void AGILETelem::EVTPacket::writePacket() {
	//initialize some filds
	//ID
	hea_out->setFieldValue(2, 1);
	
	AGILEPacket::writePacket();
}

void AGILETelem::EVTPacket::setTime(double time) {
	outputPacket->dataField->dataFieldHeader->setFieldValue_5_2(0, time);	
}
	
double AGILETelem::EVTPacket::getTime() {
	return inputPacket->dataField->dataFieldHeader->getFieldValue_5_2(0);
}

void AGILETelem::EVTPacket::setRA(float ra) {
    sdf_out->setFieldValue_5_1(0, ra);
}

float AGILETelem::EVTPacket::getRA() {
    return sdf_in->getFieldValue_5_1(0);
}

void AGILETelem::EVTPacket::setDEC(float dec) {
    sdf_out->setFieldValue_5_1(2, dec);
}

float AGILETelem::EVTPacket::getDEC() {
    return sdf_in->getFieldValue_5_1(2);
}

void AGILETelem::EVTPacket::setEvstatus(byte evstatus) {
	hea_out->setFieldValue(3, (word)evstatus);
}

byte AGILETelem::EVTPacket::getEvstatus() {
	return (byte) hea_in->getFieldValue(3);
}
	
void AGILETelem::EVTPacket::setPhase(byte phase) {
	hea_out->setFieldValue(4, (word)phase);
}

byte AGILETelem::EVTPacket::getPhase() {
	return (byte) hea_in->getFieldValue(4);
}