/***************************************************************************
                          AGILEFilter.h  -  description
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

#ifndef _AGILEFILTER_H
#define _AGILEFILTER_H

#include <vector>
#include <iomanip>
#include "AGILEPacket.h"

using namespace PacketLib;
using namespace AGILETelem;

class AGILEFilter  {


public:
    
	AGILEFilter(string archivename);

    ~AGILEFilter();
    
    void checkarchive(string archivename);

	virtual void reset() = 0;
	
	bool binary_search(double time, uint32_t& index, bool lowerbound, uint32_t iminstart = 0, uint32_t imaxstart = 0);
  	
protected:

	uint32_t midpoint(uint32_t imin, uint32_t imax);
	
    void readTimeInterval(uint32_t index_end, double &timestart, double &timend);
    
    ///check if phasecode is contained into phase
	///		if ((phasecode & 1) == 1)
	///			str << " && PHASE .NE. 0";
	///		if ((phasecode & 2) == 2)
	///			str << " && PHASE .NE. 1";
	///		if ((phasecode & 4) == 4)
	///			str << " && PHASE .NE. 2";
	///		if ((phasecode & 8) == 8)
	///			str << " && PHASE .NE. 3";
	///		if ((phasecode & 16) == 16)
	///			str << " && PHASE .NE. 4";
    bool checkPhasecode(uint8_t phasecode, uint8_t phase);
    
	string archivename;
	string basedir;
	///initial capacity of vectors of data
	int capacity;
	
	double tstart;
	double tend;
	dword packetdim;
	dword numberofpackets;
	dword filedim;
	
	AGILETelem::AGILEPacket* packet;
};

#endif