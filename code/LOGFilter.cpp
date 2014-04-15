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
	resetprequery();
}

LOGFilter::~LOGFilter() {

}

void LOGFilter::addEvent(dword index) {
	short phase;
	double attra;
	double attdec;
	double earth_ra;
	double earth_dec;
	double earth_theta;
	double earth_phi;
	double time;
	float livetime;

	log->readPacket(packetdim * index);
	phase = log->getPhase();
	attra = log->getAttitudeRaY();
	attdec = log->getAttitudeDecY();
	earth_ra = log->getEarthRa();
	earth_dec = log->getEarthDec();
	earth_theta = log->getEarthTheta();
	earth_phi = log->getEarthPhi();
	time = log->getTime();
	livetime = log->getLivetime();
			
	this->pre_ra_y.push_back(attra);
	this->pre_dec_y.push_back(attdec);
	this->pre_earth_ra.push_back(earth_ra);
	this->pre_earth_dec.push_back(earth_dec);
	this->pre_earth_theta.push_back(earth_theta);
	this->pre_earth_phi.push_back(earth_phi);
	this->pre_time.push_back(time);
	this->pre_livetime.push_back(livetime);
	this->pre_phase.push_back((short)phase);
}

bool LOGFilter::prequery(double tstart, double tstop, short phasecode) {
	cout << "LOGFilter::prequery" << endl;
	resetprequery();
	isaprequery = true;
	prequery_ok = false;
	addEvent(0);
	bool ok = query(tstart, tstop, phasecode);
	addEvent(numberofpackets-1);
	isaprequery = false;
	prequery_ok = ok;
	preval_tstart = tstart;
	preval_tstop = tstop;
	preval_phasecode = phasecode;
	return prequery_ok;
}


bool LOGFilter::query(double tstart, double tstop, short phasecode) {

	reset();
	
	if(!isaprequery && prequery_ok) {
		//check if prequery is valid
		if(tstart < preval_tstart)
			prequery_ok = false;
		else
		if(tstop > preval_tstop)
			prequery_ok = false;
					
		if(prequery_ok == false)
			resetprequery();
	}
	
		
	//binary search index1
	uint32_t index1;
	bool ret = binary_search(tstart, index1, true);
	if(ret == false) {
		if(prequery_ok && tstart >= preval_tstart && tstart <= preval_tstop)
			return true;
		cout << "LOGFilter::query index1 not found in " << setprecision(15) << tstart << " " << tstop << endl;
		return false;
	}
	//binary search index2
	uint32_t index2;
	ret = binary_search(tstop, index2, false);
	if(ret == false) {
		if(prequery_ok && tstop >= preval_tstart && tstop <= preval_tstop)
			return true;
		cout << "LOGFilter::query index2 not found in " << setprecision(15) << tstart << " " << tstop << endl;
		return false;
	}
	for(uint32_t i=index1; i<=index2; i++) {
		short phase;
		double attra;
		double attdec;
		double earth_ra;
		double earth_dec;
		double earth_theta;
		double earth_phi;
		double time;
		float livetime;
		bool add;
		
		if(!prequery_ok) {
			log->readPacket(packetdim * i);
			phase = log->getPhase();
			attra = log->getAttitudeRaY();
			attdec = log->getAttitudeDecY();
			earth_ra = log->getEarthRa();
			earth_dec = log->getEarthDec();
			earth_theta = log->getEarthTheta();
			earth_phi = log->getEarthPhi();
			time = log->getTime();
			livetime = log->getLivetime();
		} else {
			phase = pre_phase[i];
			attra = pre_ra_y[i];
			attdec = pre_dec_y[i];
			earth_ra = pre_earth_ra[i];
			earth_dec = pre_earth_dec[i];
			earth_theta = pre_earth_theta[i];
			earth_phi = pre_earth_phi[i];
			time = pre_time[i];
			livetime = pre_livetime[i];
		}
		
		add = checkPhasecode(phasecode, phase);
        
		if(add) {
			if(isaprequery) {
				//add data to arrays
				this->pre_ra_y.push_back(attra);
				this->pre_dec_y.push_back(attdec);
				this->pre_earth_ra.push_back(earth_ra);
				this->pre_earth_dec.push_back(earth_dec);
				this->pre_earth_theta.push_back(earth_theta);
				this->pre_earth_phi.push_back(earth_phi);
				this->pre_time.push_back(time);
				this->pre_livetime.push_back(livetime);
				this->pre_phase.push_back((short)phase);
			} else {
				//add data to arrays
				this->ra_y.push_back(attra);
				this->dec_y.push_back(attdec);
				this->earth_ra.push_back(earth_ra);
				this->earth_dec.push_back(earth_dec);
				this->earth_theta.push_back(earth_theta);
				this->earth_phi.push_back(earth_phi);
				this->time.push_back(time);
				this->livetime.push_back(livetime);
				this->phase.push_back((short)phase);
			}
		}
		
	}
	return true;
}
    
void LOGFilter::reset() {
	AGILEFilter::reset();
	ra_y.reserve(capacity);
	ra_y.clear();
	dec_y.reserve(capacity);
	dec_y.clear();
	earth_ra.reserve(capacity);
	earth_ra.clear();
	earth_dec.reserve(capacity);
	earth_dec.clear();
	earth_theta.reserve(capacity);
	earth_theta.clear();
	earth_phi.reserve(capacity);
	earth_phi.clear();
	phase.reserve(capacity);
	phase.clear();
	livetime.reserve(capacity);
	livetime.clear();
}

void LOGFilter::resetprequery() {
	AGILEFilter::resetprequery();
	
	//cout << "****************LOGFilter::resetprequery() " << endl;
	pre_ra_y.reserve(capacity);
	pre_ra_y.clear();
	pre_dec_y.reserve(capacity);
	pre_dec_y.clear();
	pre_earth_ra.reserve(capacity);
	pre_earth_ra.clear();
	pre_earth_dec.reserve(capacity);
	pre_earth_dec.clear();
	pre_earth_theta.reserve(capacity);
	pre_earth_theta.clear();
	pre_earth_phi.reserve(capacity);
	pre_earth_phi.clear();
	pre_phase.reserve(capacity);
	pre_phase.clear();
	pre_livetime.reserve(capacity);
	pre_livetime.clear();
	
}