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
#include "packet/File.h"

LOGFilter::LOGFilter(string filename, uint32_t timeStep) {
	this->timeStep = timeStep;
	/// The Packet containing the FADC value of each triggered telescope
	log = new AGILETelem::LOGPacket("./conf/agile.stream", filename, "");
	PacketLib::File f(log->isBigendian());
	f.open(filename.c_str(), "r");
	filedim = f.fsize();
	//get first Packet
	byte* b_log = log->readPacket(0L);
	tstart = log->getTime();
	packetdim = log->getInputPacketDimension(b_log);
	numberofpackets = filedim / packetdim;
	b_log = log->readPacket(filedim - packetdim);
	tend = log->getTime();
	cout << "filedim " << filedim << endl;
	cout << "packetdim " << packetdim << endl;
	cout << "numberofpackets " << numberofpackets << endl;
	cout << "time = " << setprecision(15) << tstart << ", " << tend << endl;
	reset();
}

LOGFilter::~LOGFilter() {

}



void LOGFilter::readTimeInterval(uint32_t index_end, double &timestart, double &timeend) {
	byte* b_log = log->readPacket(packetdim * index_end);
	timeend = log->getTime();
	b_log = log->readPacket(packetdim * (index_end-1));
	timestart =  log->getTime();
	return;
}

uint32_t LOGFilter::midpoint(uint32_t imin, uint32_t imax) {
	return imin + ((imax - imin) / 2);
}

bool LOGFilter::binary_search(double time, uint32_t &index, bool lowerbound, uint32_t iminstart, uint32_t imaxstart) {
uint32_t imin = 1;
if(iminstart != 0)
	imin = iminstart;
uint32_t imax = numberofpackets-1;
if(imaxstart != 0)
	imax = imaxstart;
int count = 0;
	while(imax >= imin) {
		count++;
		// calculate the midpoint for roughly equal partition
      	int imid = midpoint(imin, imax);
      	double timestart;
      	double timeend;
      	readTimeInterval(imid, timestart, timeend);
      	if(timestart < time && timeend >= time) {
      		cout << count << endl;
      		// key found at index imid
      		if(lowerbound)
      			index = imid;
      		else {//upperbound
      			if(time == timeend)
      				index = imid;
      			else
      				index = imid-1;
      		}
      		log->readPacket(packetdim * index);
      		cout << "I: [" << timestart << ", " << timeend << "]" << endl;
      		cout << "A: " << setprecision(15) << log->getTime() << endl;
        	return true;
        }
        else if (time > timeend)
        	// change min index to search upper subarray
        	imin = imid + 1;
      	else         
        	// change max index to search lower subarray
        	imax = imid - 1;
	}
	cout << count << endl;
	return false;
	
}

uint32_t LOGFilter::query(double tstart, double tstop, short phasecode) {
	//binary search index1
	uint32_t index1;
	bool ret = binary_search(tstart, index1, true);
	if(ret == false) {
		cerr << "LOGFilter::query index1 not found" << endl;
		return -1;
	}
	//binary search index2
	uint32_t index2;
	ret = binary_search(tstop, index2, false, index1);
	if(ret == false) {
		cerr << "LOGFilter::query index2 not found" << endl;
		return -1;
	}
	for(uint32_t i=index1; i<=index2; i++) {
		byte* b_log = log->readPacket(packetdim * i);
		short phase = log->getPhase();
		/*
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
        */
        bool add = false;
        int countcond = 0;
        int countverified = 0;
        if ((phasecode & 1) == 1) {
        	countcond++;
        	if(phase != 0)
        		countverified++;
        }
        if ((phasecode & 2) == 2){
        	countcond++;
        	if(phase != 1)
        		countverified++;
        }
        if ((phasecode & 4) == 4){
        	countcond++;
        	if(phase != 2)
        		countverified++;
        }
        if ((phasecode & 8) == 8){
        	countcond++;
        	if(phase != 3)
        		countverified++;
        }
        if ((phasecode & 16) == 16){
        	countcond++;
        	if(phase != 4)
        		countverified++;
        }
        if(countcond > 0 && countcond == countverified)
        	add = true;
		if(add) {
			//add data to arrays
			ra_y.push_back(log->getAttitudeRaY());
			dec_y.push_back(log->getAttitudeDecY());
			earth_ra.push_back(log->getEarthRa());
			earth_dec.push_back(log->getEarthDec());
			time.push_back(log->getTime());
			livetime.push_back(log->getLivetime());
			
		}
		
	}
	return time.size();
}
    
void LOGFilter::reset() {
	int capacity = 2e6;
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
	//phase.clear();
	livetime.reserve(capacity);
	livetime.clear();
}