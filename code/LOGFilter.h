/***************************************************************************
                          LOGFilter.h  -  description
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

#ifndef _LOGFILTER_H
#define _LOGFILTER_H
#include "LOGPacket.h"
#include "AGILEFilter.h"

using namespace PacketLib;
using namespace AGILETelem;

/// AGILE LOG filter
/// The query is the following (prefiltered are LOG_STATUS, MODE and ROW)
//TIME >= 162907200.000000 && TIME <= 165585600.000000 && LIVETIME > 0 && LOG_STATUS == 0 && MODE == 2 && PHASE .NE. 1 && ((#ROW == 1) || (#ROW == (#ROW/160) *160))

/*static string LogExprString(const Intervals& intvs, int phasecode, int timeStep)
{
    if (intvs.Count()<=0)
        return string("");
    stringstream str(ios_base::out);

    str << LogExprTimeString(intvs);

    str << " && LIVETIME > 0 && LOG_STATUS == 0 && MODE == 2";

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
    str << " && ((#ROW == 1) || (#ROW == (#ROW/" << timeStep << ") *" << timeStep << "))";
    return str.str();
}
*/

class LOGFilter : public AGILEFilter {


public:
    
    ///\param filename the filename of the archive
    ///\param timeStep the sample of lines withint the original FITS archive. Now is pre-filtered
	LOGFilter(string archivename, uint32_t timeStep);

    ~LOGFilter();
    
    ///Example:
    ///TIME >= 162907200.000000 && TIME <= 165585600.000000 && LIVETIME > 0 && LOG_STATUS == 0 && MODE == 2 && PHASE .NE. 1 && ((#ROW == 1) || (#ROW == (#ROW/160) *160))
    ///but pre-filtered are
    ///LIVETIME > 0 && LOG_STATUS == 0 && MODE == 2 && ((#ROW == 1) || (#ROW == (#ROW/timeStep) *timeStep))
    ///\param tstart,tstop should be a list of time intervals
    ///\return false if the index of the lower or upper bound is not found, found if the interval is found
    bool query(double tstart, double tstop, short phasecode);
    
    ///reset all the vectors of the result
    void reset();
    
    //results
    vector<double> ra_y;
    vector<double> dec_y;
    vector<double> earth_ra;
    vector<double> earth_dec;
    vector<double> time;
    vector<short> phase;
    vector<double> livetime; //double for compatibility with AG_expmapgenT5
    
    ///the sample of lines withint the original FITS archive.
    uint32_t timeStep;
    
    bool binary_search(double time, uint32_t& index, bool lowerbound, uint32_t iminstart = 0, uint32_t imaxstart = 0);
    void readTimeInterval(uint32_t index_end, double &timestart, double &timend);
	
    
protected:

	AGILETelem::LOGPacket* log;
	double tstart;
	double tend;
	dword packetdim;
	dword numberofpackets;
	dword filedim;
    
};


#endif