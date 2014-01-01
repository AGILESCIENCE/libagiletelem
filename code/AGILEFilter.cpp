/***************************************************************************
                          AGILEFilter.cpp  -  description
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
 
#include "AGILEFilter.h""
#include "packet/File.h"

AGILEFilter::AGILEFilter(string archivename) {
	capacity = 2e6;
	this->archivename = archivename;
	const char* home = getenv("AGILE");
    if (!home)
    {
    	std::cerr << "AGILE environment variable is not defined." << std::endl;
        exit(0);
    }
    basedir = home;
}

AGILEFilter::~AGILEFilter() {

}

uint32_t AGILEFilter::midpoint(uint32_t imin, uint32_t imax) {
	return imin + ((imax - imin) / 2);
}

void AGILEFilter::checkarchive(string archivename) {
	PacketLib::File f(packet->isBigendian());
	f.open(archivename.c_str(), "r");
	filedim = f.fsize();
	//get first Packet
	byte* b_log = packet->readPacket(0L);
	tstart = packet->getTime();
	packetdim = packet->getInputPacketDimension(b_log);
	numberofpackets = filedim / packetdim;
	b_log = packet->readPacket(filedim - packetdim);
	tend = packet->getTime();
	cout << "filedim " << filedim << endl;
	cout << "packetdim " << packetdim << endl;
	cout << "numberofpackets " << numberofpackets << endl;
	cout << "time = " << setprecision(15) << tstart << ", " << tend << endl;
	f.close();
}

void AGILEFilter::readTimeInterval(uint32_t index_end, double &timestart, double &timeend) {
	byte* b_log = packet->readPacket(packetdim * index_end);
	timeend = packet->getTime();
	b_log = packet->readPacket(packetdim * (index_end-1));
	timestart =  packet->getTime();
	return;
}

bool AGILEFilter::binary_search(double time, uint32_t &index, bool lowerbound, uint32_t iminstart, uint32_t imaxstart) {
uint32_t imin = 1;
	if(iminstart != 0)
		imin = iminstart;
	uint32_t imax = numberofpackets-1;
	if(imaxstart != 0)
		imax = imaxstart;
	int count = 0;
	if(time < 0)
		return false;
	while(imax >= imin) {
		count++;
		// calculate the midpoint for roughly equal partition
      	int imid = midpoint(imin, imax);
      	double timestart;
      	double timeend;
      	readTimeInterval(imid, timestart, timeend);
      	if(timestart < time && timeend >= time) {
      		//cout << count << endl;
      		// key found at index imid
      		if(lowerbound)
      			index = imid;
      		else {//upperbound
      			if(time == timeend)
      				index = imid;
      			else
      				index = imid-1;
      		}
      		packet->readPacket(packetdim * index);
      		//string bound = lowerbound?"lowerbound":"upperbound";
      		//cout << "I"<< bound <<": [" << setprecision(15) <<  timestart << ", " << timeend << "] -> " << log->getTime() << " -> " << index << endl;
        	return true;
        }
        else if (time > timeend)
        	// change min index to search upper subarray
        	imin = imid + 1;
      	else         
        	// change max index to search lower subarray
        	imax = imid - 1;
	}
	//cout << count << endl;
	return false;
	
}

bool AGILEFilter::checkPhasecode(uint8_t phasecode, uint8_t phase) {
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
	return add;
}
