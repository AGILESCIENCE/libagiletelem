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
#include <cmath>
#include <assert.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double PI_BY_TWO = M_PI/2;
const double TWO_PI = M_PI*2;
const double FOUR_PI = M_PI*4;

const double RAD2DEG = 180.0/M_PI;
const double DEG2RAD = M_PI/180.0;


/// distance in Galactic coordinates. Longitude=l, latitude=b
inline double SphDistDeg(double long1, double lat1, double long2, double lat2)
{
    double l1 = long1*DEG2RAD;
    double l2 = long2*DEG2RAD;
    double b1 = lat1*DEG2RAD;
    double b2 = lat2*DEG2RAD;
    double val = sin(b1)*sin(b2) + cos(b1)*cos(b2) *cos(l1-l2);
    if(val > 1.0) return 0.0;
    if(val < -1.0) return 180.0;
    double dist = acos(val);
    return dist*RAD2DEG;
}

const double psi[6]    = { 0.57477043300, 4.9368292465,  0.00000000000, 0.00000000000, 0.11142137093, 4.71279419371};
const double stheta[6] = { 0.88998808748,-0.88998808748, 0.39777715593,-0.39777715593, 0.86766622025,-0.86766622025};
const double ctheta[6] = { 0.45598377618, 0.45598377618, 0.91748206207, 0.91748206207, 0.49714719172, 0.49714719172};
const double phi[6]    = { 4.9368292465,  0.57477043300, 0.0000000000,  0.00000000000, 4.71279419371, 0.11142137093};

void Euler(double ai, double bi, double * ao, double * bo, int select)
{
--select;
double a  = ai*DEG2RAD - phi[select];
double b = bi*DEG2RAD;
double sb = sin(b);
double cb = cos(b);
double cbsa = cb * sin(a);
b   = -stheta[select] * cbsa + ctheta[select] * sb;
*bo = b<1.0 ? asin(b)*RAD2DEG : 90.0;
a   = atan2( ctheta[select] * cbsa + stheta[select] * sb, cb * cos(a) );
*ao = fmod(a+psi[select]+FOUR_PI, TWO_PI) * RAD2DEG;
}

EVTFilter::EVTFilter(string archivename) : AGILEFilter(archivename) {
	// The Packet containing the FADC value of each triggered telescope
	evt = new AGILETelem::EVTPacket(basedir + "/share/agiletelem/agile.stream", archivename, "");
	packet = (AGILEPacket*) evt;
	checkarchive(archivename);
	postfilter1 = false;
	mdim = lc = bc = 0;
	reset();
	resetprequery();
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

void EVTFilter::addEvent(dword index) {
	short phase;
	uint8_t evstatus;
	uint16_t energy;
	uint8_t ph_earth;
	uint8_t theta;
	float ra;
	float dec;
	double time;
	evt->readPacket(index * packetdim);
	phase = evt->getPhase();
	evstatus = evt->getEvstatus();
	energy = evt->getEnergy();
	ph_earth = evt->getPh_Earth();
	theta = evt->getTheta();
	ra = evt->getRA();
	dec = evt->getDEC();
	time = evt->getTime();
	this->pre_ra.push_back((float) ra);
	this->pre_dec.push_back((float) dec);
	this->pre_time.push_back((double) time);
	this->pre_energy.push_back((uint16_t) energy);
	this->pre_ph_earth.push_back((uint8_t) ph_earth);
	this->pre_theta.push_back((uint8_t) theta);
	this->pre_evstatus.push_back((uint8_t) evstatus);
	this->pre_phase.push_back((uint8_t) phase);
}

bool EVTFilter::prequery(double tstart, double tstop, uint8_t phasecode, uint8_t filtercode, uint16_t emin, uint16_t emax, uint8_t albrad, uint8_t fovradmin, uint8_t fovradmax) {
	cout << "EVTFilter::prequery" << endl;
	resetprequery();
	isaprequery = true;
	prequery_ok = false;
	addEvent(0);
	bool ok = query(tstart, tstop, phasecode, filtercode, emin, emax, albrad, fovradmin, fovradmax);
	addEvent(numberofpackets-1);
	isaprequery = false;
	prequery_ok = ok;
	preval_tstart = tstart;
	preval_tstop = tstop;
	preval_emin = emin;
	preval_emax = emax;
	preval_fovradmin = fovradmin;
	preval_fovradmax = fovradmax;
	//cout << "PREQUERY DIM " << this->pre_time.size() << endl;
	//for(int i= 0; i<pre_time.size(); i++)
	//	cout << setprecision(20) << pre_time[i] << endl;
	return prequery_ok;
}

bool EVTFilter::query(double tstart, double tstop, uint8_t phasecode, uint8_t filtercode, uint16_t emin, uint16_t emax, uint8_t albrad, uint8_t fovradmin, uint8_t fovradmax) {

	reset();


	if(!isaprequery && prequery_ok) {
		//check if prequery is valid
		if(tstart < preval_tstart)
			prequery_ok = false;
		else
		if(tstop > preval_tstop)
			prequery_ok = false;
		else
		if(emin < preval_emin)
			prequery_ok = false;
		else
		if(emax < preval_emax)
			prequery_ok = false;
		else
		if(fovradmin < preval_fovradmin)
			prequery_ok = false;
		else
		if(fovradmax > preval_fovradmax)
			prequery_ok = false;

		if(prequery_ok == false)
			resetprequery();
	}


	//cout << (int) phasecode << " " << (int) filtercode << " " << (int) emin << " " << (int) emax << " " << (int) albrad << " " << (int) fovradmin << " " << (int) fovradmax << endl;
	//binary search index1
	uint32_t index1;
	bool ret = binary_search(tstart, index1, true);
	if(ret == false) {
		if(prequery_ok && tstart >= preval_tstart && tstart <= preval_tstop)
			return true;
		cout << "EVTFilter::query index1 not found" << endl;
		return false;
	}
	//binary search index2
	uint32_t index2;
	ret = binary_search(tstop, index2, false);
	if(ret == false) {
		if(prequery_ok && tstop >= preval_tstart && tstop <= preval_tstop)
			return true;
		cout << "EVTFilter::query index2 not found" << endl;
		return false;
	}
	//cout << "INDEX " << index1 << " " << index2 << endl;
	for(uint32_t i=index1; i<=index2; i++) {
		short phase;
		uint8_t evstatus;
		uint16_t energy;
		uint8_t ph_earth;
		uint8_t theta;
		float ra;
		float dec;
		double time;
		bool add;

		if(!prequery_ok) {

			evt->readPacket(packetdim * i);
			phase = evt->getPhase();
			//filter
			add = checkPhasecode(phasecode, phase);
			if(add == false)
				continue;
			evstatus = evt->getEvstatus();
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
			energy = evt->getEnergy();
			ph_earth = evt->getPh_Earth();
			theta = evt->getTheta();
			add = false;
			if(energy >= emin && energy <= emax && ph_earth >= albrad && theta < fovradmax && theta >= fovradmin)
				add = true;

			ra = evt->getRA();
			dec = evt->getDEC();

			if(add && postfilter1 == true) {
				add = checkPostfilter1(ra, dec);
			}

			time = evt->getTime();

		} else {

			phase = pre_phase[i];
			add = checkPhasecode(phasecode, phase);
			if(add == false)
				continue;
			evstatus = pre_evstatus[i];
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
			energy = pre_energy[i];
			ph_earth = pre_ph_earth[i];
			theta = pre_theta[i];
			add = false;
			if(energy >= emin && energy <= emax && ph_earth >= albrad && theta < fovradmax && theta >= fovradmin)
				add = true;

			ra = pre_ra[i];
			dec = pre_dec[i];

			if(add && postfilter1 == true) {
				add = checkPostfilter1(ra, dec);
			}

			time = pre_time[i];

		}

		if(add) {

			if(isaprequery) {
				//add data to arrays
				this->pre_ra.push_back((float) ra);
				this->pre_dec.push_back((float) dec);
				this->pre_time.push_back((double) time);
				this->pre_energy.push_back((uint16_t) energy);
				this->pre_ph_earth.push_back((uint8_t) ph_earth);
				this->pre_theta.push_back((uint8_t) theta);
				this->pre_evstatus.push_back((uint8_t) evstatus);
				this->pre_phase.push_back((uint8_t) phase);
			} else {
				//add data to arrays
				this->ra.push_back((float) ra);
				this->dec.push_back((float) dec);
				this->time.push_back((double) time);
				this->energy.push_back((uint16_t) energy);
				this->ph_earth.push_back((uint8_t) ph_earth);
				this->theta.push_back((uint8_t) theta);
				this->evstatus.push_back((uint8_t) evstatus);
			}

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
	AGILEFilter::reset();

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

void EVTFilter::resetprequery() {
	AGILEFilter::resetprequery();

	//cout << "****************EVTFilter::resetprequery() " << endl;
	pre_ra.reserve(capacity);
	pre_ra.clear();
	pre_dec.reserve(capacity);
	pre_dec.clear();
	pre_energy.reserve(capacity);
	pre_energy.clear();
	pre_ph_earth.reserve(capacity);
	pre_ph_earth.clear();
	pre_theta.reserve(capacity);
	pre_theta.clear();
	pre_evstatus.reserve(capacity);
	pre_evstatus.clear();
	pre_phase.reserve(capacity);
	pre_phase.clear();
}
