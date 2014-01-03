/***************************************************************************
                          EVTFilter.cpp  -  description
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


#include "EVTFilter.h"

#include "MathUtils.h"

EVTFilter::EVTFilter(string archivename) : AGILEFilter(archivename) {
	// The Packet containing the FADC value of each triggered telescope
	evt = new AGILETelem::EVTPacket(basedir + "/share/agiletelem/agile.stream", archivename, "");
	packet = (AGILEPacket*) evt;
	checkarchive(archivename);
	postfilter1 = false;
	mdim = lc = bc = 0;
	reset();
}

EVTFilter::~EVTFilter() {

}

bool EVTFilter::checkEvstatus(uint8_t filtercode, uint8_t evstatus) {
		/*
		if ((filtercode & 1) == 1)
        	str << " && EVSTATUS .NE. 'L'";
    	if ((filtercode & 2) == 2)
        	str << " && EVSTATUS .NE. 'G'";
    	if ((filtercode & 4) == 4)
        	str << " && EVSTATUS .NE. 'S'";
        */
        /*
        if(evt.compare("G") == 0) status2[i] = 0;
		if(evt.compare("L") == 0) status2[i] = 1;
		if(evt.compare("S") == 0) status2[i] = 2;
		*/
	bool add = false;
	int countcond = 0;
	int countverified = 0;
	//L
	if ((filtercode & 1) == 1) {
		countcond++;
		if(evstatus != 1)
			countverified++;
	}
	//G
	if ((filtercode & 2) == 2){
		countcond++;
		if(evstatus != 0)
			countverified++;
	}
	//S
	if ((filtercode & 4) == 4){
		countcond++;
		if(evstatus != 2)
			countverified++;
	}
	
	if(countcond > 0 && countcond == countverified)
		add = true;
	return add;
}


bool EVTFilter::query(double tstart, double tstop, uint8_t phasecode, uint8_t filtercode, uint16_t emin, uint16_t emax, uint8_t albrad, uint8_t fovradmin, uint8_t fovradmax) {

	//cout << (int) phasecode << " " << (int) filtercode << " " << (int) emin << " " << (int) emax << " " << (int) albrad << " " << (int) fovradmin << " " << (int) fovradmax << endl;
	//binary search index1
	uint32_t index1;
	bool ret = binary_search(tstart, index1, true);
	if(ret == false) {
		cerr << "EVTFilter::query index1 not found" << endl;
		return false;
	}
	//binary search index2
	uint32_t index2;
	ret = binary_search(tstop, index2, false);
	if(ret == false) {
		cerr << "EVTFilter::query index2 not found" << endl;
		return false;
	}
	for(uint32_t i=index1; i<=index2; i++) {
		byte* b_log = evt->readPacket(packetdim * i);
		short phase = evt->getPhase();
		//filter
		bool add = checkPhasecode(phasecode, phase);
		if(add == false)
			continue;
		uint8_t evstatus = evt->getEvstatus();
		add = checkEvstatus(filtercode, evstatus);
        if(add == false)
			continue;
		/*
		str << " && ENERGY >= " << emin;
		str << " && ENERGY <= " << emax;
		str << " && PH_EARTH >= " << albrad;
		str << " && THETA < " << fovradmax;
		str << " && THETA >= " << fovradmin;
        */
        uint16_t energy = evt->getEnergy();
        uint8_t ph_earth = evt->getPh_Earth();
        uint8_t theta = evt->getTheta();
        add = false;
        if(energy >= emin && energy <= emax && ph_earth >= albrad && theta < fovradmax && theta >= fovradmin)
        	add = true;
        	
        float ra = evt->getRA();
        float dec = evt->getDEC();
        if(add && postfilter1 == true) {
        	add = checkPostfilter1(ra, dec);
        }
        
		if(add) {
			//add data to arrays
			this->ra.push_back((float) ra);
			this->dec.push_back((float) dec);
			this->time.push_back(evt->getTime());
			this->energy.push_back((uint16_t) energy);
			this->ph_earth.push_back((uint8_t) ph_earth);
			this->theta.push_back((uint8_t) theta);
			this->evstatus.push_back((uint8_t) evstatus);
		}
		
	}
	return true;
}
    
void EVTFilter::setPostfilter1(double mdim, double lc, double bc) {
	postfilter1 = true;
	this->mdim = mdim;
	this->lc = lc;
	this->bc = bc;
	//cout << "mdim " << this->mdim << " " << this->lc << " " << this->bc << endl;
}

void EVTFilter::unsetPostfilter1() {
	postfilter1 = false;
}

bool EVTFilter::checkPostfilter1(float ra, float dec) {
	double l = 0, b = 0;
	Euler((double)ra, (double)dec, &l, &b, 1);
	if (SphDistDeg(l, b, lc, bc)<=mdim/2.0)
		return true;
	else
		return false;
}

void EVTFilter::reset() {
	time.reserve(capacity);
	time.clear();
	ra.reserve(capacity);
	ra.clear();
	dec.reserve(capacity);
	dec.clear();
	energy.reserve(capacity);
	energy.clear();
	ph_earth.reserve(capacity);
	ph_earth.clear();
	theta.reserve(capacity);
	theta.clear();
	evstatus.reserve(capacity);
	evstatus.clear();
}