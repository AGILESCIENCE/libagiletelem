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

/// Writing the packet
void AGILETelem::LOGPacket::writePacket() {
	//initialize some filds
	//ID
	hea_out->setFieldValue(2, 2);
	
	AGILEPacket::writePacket();
}

/// Printing the created packet
void AGILETelem::LOGPacket::printPacket_output() {
	AGILEPacket::printPacket_output();
	cout << "ATTITUDE_RA_Y  " << sdf_out->getFieldValue_5_2(0) << endl;
	cout << "ATTITUDE_DEC_Y " << sdf_out->getFieldValue_5_2(4) << endl;
}

void AGILETelem::LOGPacket::setTime(double time) {
	dfh_out->setFieldValue_5_2(0, time);	
}
	
double AGILETelem::LOGPacket::getTime() {
	return dfh_in->getFieldValue_5_2(0);
}

void AGILETelem::LOGPacket::setLivetime(float livetime) {
    dfh_out->setFieldValue_5_1(4, livetime);
}

float AGILETelem::LOGPacket::getLivetime() {
    return dfh_in->getFieldValue_5_1(4);
}

void AGILETelem::LOGPacket::setPhase(byte phase) {
	hea_out->setFieldValue(4, (word)phase);
}

byte AGILETelem::LOGPacket::getPhase() {
	return (byte) hea_in->getFieldValue(4);
}