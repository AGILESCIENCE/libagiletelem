/***************************************************************************
                          RATPacket.h  -  description
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

#ifndef _RATPacket_H
#define _RATPacket_H
#include "AGILEPacket.h"

namespace AGILETelem {

/// LOG fits files store one line each 0.1 s
class RATPacket : public AGILETelem::AGILEPacket  {


public:
    /// It takes the configuration file .stream and the input/output file name
    /// \param packetConfig The packet configuration file (.stream)
    /// \param tmInputFileName The input file name of the packet
    /// \param tmOutputFileName The output file name of the packet
	RATPacket(string packetConfig, string tmInputFileName, string tmOutputFileName);

	RATPacket(string packetConfig);

    ~RATPacket();

	/// Writing the packet
	void writePacket();
	
	/// Printing the created packet
	void printPacket_output();

	virtual void setTime(double time);
	
	virtual double getTime();
	
	///File.open("a").each_line do | line |
	///l=line.split(" ")[1].split("RAT_")[1]
	///puts "void set"+l.to_s+"(word value) {sdf_out->setFieldValue("+i.to_s+", value);};"
	///puts "word get"+l.to_s+"() { return sdf_in->getFieldValue("+i.to_s+"); };"
	///i = i + 1
	///end
	void setM6271_0_AC_TOP1_RATEM(word value) {sdf_out->setFieldValue(0, value);};
	word getM6271_0_AC_TOP1_RATEM() { return sdf_in->getFieldValue(0); };
	void setM6272_0_AC_TOP2_RATEM(word value) {sdf_out->setFieldValue(1, value);};
	word getM6272_0_AC_TOP2_RATEM() { return sdf_in->getFieldValue(1); };
	void setM6273_0_AC_TOP3_RATEM(word value) {sdf_out->setFieldValue(2, value);};
	word getM6273_0_AC_TOP3_RATEM() { return sdf_in->getFieldValue(2); };
	void setM6274_0_AC_LAT1_1_RATEM(word value) {sdf_out->setFieldValue(3, value);};
	word getM6274_0_AC_LAT1_1_RATEM() { return sdf_in->getFieldValue(3); };
	void setM6275_0_AC_LAT1_2_RATEM(word value) {sdf_out->setFieldValue(4, value);};
	word getM6275_0_AC_LAT1_2_RATEM() { return sdf_in->getFieldValue(4); };
	void setM6276_0_AC_LAT1_3_RATEM(word value) {sdf_out->setFieldValue(5, value);};
	word getM6276_0_AC_LAT1_3_RATEM() { return sdf_in->getFieldValue(5); };
	void setM6277_0_AC_LAT2_1_RATEM(word value) {sdf_out->setFieldValue(6, value);};
	word getM6277_0_AC_LAT2_1_RATEM() { return sdf_in->getFieldValue(6); };
	void setM6278_0_AC_LAT2_2_RATEM(word value) {sdf_out->setFieldValue(7, value);};
	word getM6278_0_AC_LAT2_2_RATEM() { return sdf_in->getFieldValue(7); };
	void setM6279_0_AC_LAT2_3_RATEM(word value) {sdf_out->setFieldValue(8, value);};
	word getM6279_0_AC_LAT2_3_RATEM() { return sdf_in->getFieldValue(8); };
	void setM6280_0_AC_LAT3_1_RATEM(word value) {sdf_out->setFieldValue(9, value);};
	word getM6280_0_AC_LAT3_1_RATEM() { return sdf_in->getFieldValue(9); };
	void setM6281_0_AC_LAT3_2_RATEM(word value) {sdf_out->setFieldValue(10, value);};
	word getM6281_0_AC_LAT3_2_RATEM() { return sdf_in->getFieldValue(10); };
	void setM6282_0_AC_LAT3_3_RATEM(word value) {sdf_out->setFieldValue(11, value);};
	word getM6282_0_AC_LAT3_3_RATEM() { return sdf_in->getFieldValue(11); };
	void setM6283_0_AC_LAT4_1_RATEM(word value) {sdf_out->setFieldValue(12, value);};
	word getM6283_0_AC_LAT4_1_RATEM() { return sdf_in->getFieldValue(12); };
	void setM6284_0_AC_LAT4_2_RATEM(word value) {sdf_out->setFieldValue(13, value);};
	word getM6284_0_AC_LAT4_2_RATEM() { return sdf_in->getFieldValue(13); };
	void setM6285_0_AC_LAT4_3_RATEM(word value) {sdf_out->setFieldValue(14, value);};
	word getM6285_0_AC_LAT4_3_RATEM() { return sdf_in->getFieldValue(14); };
	void setM6305_0_PDHU_ratemeter_of_T1_YES_signal(word value) {sdf_out->setFieldValue(15, value);};
	word getM6305_0_PDHU_ratemeter_of_T1_YES_signal() { return sdf_in->getFieldValue(15); };
	void setM6320_0_PDHU_GRID_events_sent_to_ground(word value) {sdf_out->setFieldValue(16, value);};
	word getM6320_0_PDHU_GRID_events_sent_to_ground() { return sdf_in->getFieldValue(16); };
	void setM6573_0_MCAL_Fixed_Rt_BoardX(word value) {sdf_out->setFieldValue(17, value);};
	word getM6573_0_MCAL_Fixed_Rt_BoardX() { return sdf_in->getFieldValue(17); };
	void setM6575_0_MCAL_Fixed_Rt_BoardZ(word value) {sdf_out->setFieldValue(18, value);};
	word getM6575_0_MCAL_Fixed_Rt_BoardZ() { return sdf_in->getFieldValue(18); };

};
}

#endif
