/***************************************************************************
                          LOGPacket.h  -  description
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

#ifndef _LOGPACKET_H
#define _LOGPACKET_H
#include "AGILEPacket.h"

namespace AGILETelem {

/// LOG fits files store one line each 0.1 s
class LOGPacket : public AGILETelem::AGILEPacket  {


public:
    /// It takes the configuration file .stream and the input/output file name
    /// \param packetConfig The packet configuration file (.stream)
    /// \param tmInputFileName The input file name of the packet
    /// \param tmOutputFileName The output file name of the packet
	LOGPacket(string packetConfig, string tmInputFileName, string tmOutputFileName);

	LOGPacket(string packetConfig);

    ~LOGPacket();

	/// Writing the packet
	void writePacket();
	
	/// Printing the created packet
	void printPacket_output();

	void setTime(double time);
	
	double getTime();
	
	void setPhase(byte phase);

	byte getPhase();	

	void setLivetime(float livetime);
	
	float getLivetime();
	
	void setAttitudeRaY(double value) { sdf_out->setFieldValue_5_2(0, value); };
	
	double getAttitudeRaY() { return sdf_in->getFieldValue_5_2(0); };

	void setAttitudeDecY(double value)  { sdf_out->setFieldValue_5_2(4, value); };
	
	double getAttitudeDecY() { return sdf_in->getFieldValue_5_2(4); };
	
	void setEarthRa(double value)  { sdf_out->setFieldValue_5_2(8, value); };
	
	double getEarthRa() { return sdf_in->getFieldValue_5_2(8); };

	void setEarthDec(double value)  { sdf_out->setFieldValue_5_2(12, value); };
	
	double getEarthDec() { return sdf_in->getFieldValue_5_2(12); };
};
}

#endif
