/***************************************************************************
                          CTATriggeredTelescope.h  -  description
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

/// Creating and writing a packet for each CTA triggered telescope,
/// containg the FADC value at each sample of the telescope pixel
/// \brief CTA triggered telescope packet class
class LOGPacket : public AGILETelem::AGILEPacket  {


public:
    /// It takes the configuration file .stream and the input/output file name
    /// \param packetConfig The packet configuration file (.stream)
    /// \param tmInputFileName The input file name of the packet
    /// \param tmOutputFileName The output file name of the packet
	LOGPacket(string packetConfig, string tmInputFileName, string tmOutputFileName);

	LOGPacket(string packetConfig);

    ~LOGPacket();




};
}

#endif
