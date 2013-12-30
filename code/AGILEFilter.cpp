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

AGILEFilter::AGILEFilter(string archivename) {
	this->archivename = archivename;
}

AGILEFilter::~AGILEFilter() {

}

uint32_t AGILEFilter::midpoint(uint32_t imin, uint32_t imax) {
	return imin + ((imax - imin) / 2);
}