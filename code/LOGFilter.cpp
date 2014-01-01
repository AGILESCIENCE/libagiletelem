/***************************************************************************
                          LOGFilter.cpp  -  description
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


#include "LOGFilter.h"


LOGFilter::LOGFilter(string archivename, uint32_t timeStep) : AGILEFilter(archivename) {
	this->timeStep = timeStep;
	/// The Packet containing the FADC value of each triggered telescope
	log = new AGILETelem::LOGPacket(basedir + "/share/agiletelem/agile.stream", archivename, "");
	packet = (AGILEPacket*) log;
	checkarchive(archivename);
	reset();
}

LOGFilter::~LOGFilter() {

}

bool LOGFilter::query(double tstart, double tstop, short phasecode) {
	//binary search index1
	uint32_t index1;
	bool ret = binary_search(tstart, index1, true);
	if(ret == false) {
		cerr << "LOGFilter::query index1 not found" << endl;
		return false;
	}
	//binary search index2
	uint32_t index2;
	ret = binary_search(tstop, index2, false);
	if(ret == false) {
		cerr << "LOGFilter::query index2 not found" << endl;
		return false;
	}
	for(uint32_t i=index1; i<=index2; i++) {
		byte* b_log = log->readPacket(packetdim * i);
		short phase = log->getPhase();
		
		bool add = checkPhasecode(phasecode, phase);
        
		if(add) {
			//add data to arrays
			ra_y.push_back(log->getAttitudeRaY());
			dec_y.push_back(log->getAttitudeDecY());
			earth_ra.push_back(log->getEarthRa());
			earth_dec.push_back(log->getEarthDec());
			time.push_back(log->getTime());
			livetime.push_back(log->getLivetime());
			this->phase.push_back((short)log->getPhase());
		}
		
	}
	return true;
}
    
void LOGFilter::reset() {
	ra_y.reserve(capacity);
	ra_y.clear();
	dec_y.reserve(capacity);
	dec_y.clear();
	earth_ra.reserve(capacity);
	earth_ra.clear();
	earth_dec.reserve(capacity);
	earth_dec.clear();
	time.reserve(capacity);
	time.clear();
	phase.reserve(capacity);
	phase.clear();
	livetime.reserve(capacity);
	livetime.clear();
}