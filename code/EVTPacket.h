/***************************************************************************
                          EVTPacket.h  -  description
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

#ifndef _EVTPACKET_H
#define _EVTPACKET_H
#include "AGILEPacket.h"

namespace AGILETelem {

/// Creating and writing a packet for each AGILE grid event,

class EVTPacket : public AGILETelem::AGILEPacket {


public:
    /// It takes the configuration file .stream and the input/output file name
    /// \param packetConfig The packet configuration file (.stream)
    /// \param tmInputFileName The input file name of the packet
    /// \param tmOutputFileName The output file name of the packet
	EVTPacket(string packetConfig, string tmInputFileName, string tmOutputFileName);

	EVTPacket(string packetConfig);

    ~EVTPacket();
    
    /// Writing the packet
	void writePacket();

	void setTime(double time);
	
	double getTime();

	void setRA(float ra);
	
	float getRA();

	void setDEC(float dec);
	
	float getDEC();
	
	void setEvstatus(byte evstatus);

	byte getEvstatus();
	
	void setPhase(byte phase);

	byte getPhase();	
	
	void setEnergy(word energy) { dfh_out->setFieldValue(4, energy); };

	word getEnergy() { return dfh_in->getFieldValue(4);};
	
	void setPh_Earth(byte ph_earth) { dfh_out->setFieldValue(5, ph_earth); };

	byte getPh_Earth()  { return (byte) dfh_in->getFieldValue(5);};
	
	void setTheta(byte theta) { dfh_out->setFieldValue(6, theta); };

	byte getTheta()  { return (byte) dfh_in->getFieldValue(6);};
	
};
}

#endif
