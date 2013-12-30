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
  
protected:

	
	uint32_t midpoint(uint32_t imin, uint32_t imax);

	string archivename;
	  
};

#endif