/***************************************************************************
                          EVTFilter.h  -  description
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

#ifndef _EVTFILTER_H
#define _EVTFILTER_H
#include "AGILEFilter.h"
#include "EVTPacket.h"

using namespace PacketLib;
using namespace AGILETelem;

/// AGILE LOG filter
/// The query on data for AG_ctsmapgen is he following
/*if (intvs.Count()==1)
        str << "TIME >= " << fixed << intvs[0].Start() << " && TIME <= " << intvs[0].Stop();
    else {
        str << "( ";
        const char* sep = "";
        for (int i=0; i<intvs.Count(); ++i) {
            str << sep << " ( TIME >= " << fixed << intvs[i].Start() << " && TIME <= " << intvs[i].Stop() << " )";
            sep = " ||";
        }
        str << " )";
    }

    str << " && ENERGY >= " << emin;
    str << " && ENERGY <= " << emax;
    str << " && PH_EARTH >= " << albrad;
    str << " && THETA < " << fovradmax;
    str << " && THETA >= " << fovradmin;

    if ((phasecode & 1) == 1)
        str << " && PHASE .NE. 0";
    if ((phasecode & 2) == 2)
        str << " && PHASE .NE. 1";
    if ((phasecode & 4) == 4)
        str << " && PHASE .NE. 2";
    if ((phasecode & 8) == 8)
        str << " && PHASE .NE. 3";
    if ((phasecode & 16) == 16)
        str << " && PHASE .NE. 4";

    if ((filtercode & 1) == 1)
        str << " && EVSTATUS .NE. 'L'";
    if ((filtercode & 2) == 2)
        str << " && EVSTATUS .NE. 'G'";
    if ((filtercode & 4) == 4)
        str << " && EVSTATUS .NE. 'S'";
*/        
class EVTFilter : public AGILEFilter {


public:
    
	EVTFilter(string archivename);

    ~EVTFilter();
    
    ///\param tstart,tstop should be a list of time intervals
    ///\return false if the index of the lower or upper bound is not found, found if the interval is found
    bool query(double tstart, double tstop);
    

    
    ///reset all the vectors of the result
    void reset();
    
    //results
    vector<float> ra;
    vector<float> dec;
    
protected:

	bool binary_search(double time, uint32_t& index, bool lowerbound, uint32_t iminstart = 0, uint32_t imaxstart = 0);
    void readTimeInterval(uint32_t index_end, double &timestart, double &timend);
    
    
};


#endif