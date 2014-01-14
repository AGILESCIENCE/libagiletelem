#include "AGILEPacket.h"
#include "packet/OutputFile.h"
#include "packet/InputFile.h"
#include "packet/PacketException.h"

void AGILETelem::AGILEPacket::printListOfString(char** r) {
	int i = 0;
	while (r[i] != 0)
		cout << r[i++] << endl;
}

void AGILETelem::AGILEPacket::printListOfString(string* r) {

	cout << *r << endl;
}

AGILETelem::AGILEPacket::AGILEPacket(string packetConfig, string tmInputFileName,
		string tmOutputFileName, int packetID) {

	cout << "packetConfig " << packetConfig << endl;

	in = 0;
	out = 0;
	ips = 0;
	ops = 0;
	outputPacket = 0;
	inputPacket = 0;
	this->packetStreamConfig = packetConfig;
	this->packetID = packetID;

	try {
		char** param = (char**) new char*[3];

		if (tmOutputFileName != "") {
			/// create output packet stream
			ops = new OutputPacketStream();
			ops->setFileNameConfig(packetConfig.c_str());
			ops->createStreamStructure();
			outputPacket = ops->getPacketType(packetID);
			
			hea_out = outputPacket->header;
			dfh_out = outputPacket->dataField->dataFieldHeader;
			sdf_out = (SDFRBlock*) outputPacket->dataField->sourceDataField;
			
			//cout << (const char*) outputPacket->getName() << endl;
			/// parameter for the output: file
			out = (Output*) new OutputFile(ops->isBigEndian());
			/// file name
			param[0] = (char*) tmOutputFileName.c_str();
			param[1] = "a";
			param[2] = 0;

			/// open output
			out->open(param);

			/// connect the output
			ops->setOutput(out);

		}
		if (tmInputFileName != "") {
			/// create input packet stream
			ips = new InputPacketStream();
			ips->setFileNameConfig(packetConfig.c_str());
			ips->createStreamStructure();
			inputPacket = ips->getPacketType(packetID);
			
			hea_in = inputPacket->header;
			dfh_in = inputPacket->dataField->dataFieldHeader;
			sdf_in = (SDFRBlock*) inputPacket->dataField->sourceDataField;

			in = (Input*) new PacketLib::InputFile(ips->isBigEndian());
			/// file name
			param[0] = (char*) tmInputFileName.c_str();
			param[1] = 0;

			/// open input
			in->open(param);

			/// set a particular input
			ips->setInput(in);
		}

	}
	catch (PacketException* e) {
		cout << "AGILETelem::AGILEPacket::AGILEPacket(string packetConfig, string tmInputFileName, string tmOutputFileName): " << e->geterror()
				<< endl;
	}

}

AGILETelem::AGILEPacket::AGILEPacket(string packetConfig, int packetID) {

	in = 0;
	out = 0;
	ips = 0;
	ops = 0;
	outputPacket = 0;
	inputPacket = 0;

	this->packetStreamConfig = packetConfig;
	this->packetID = packetID;
	
	try {
		char** param = (char**) new char*[2];


		/// create input packet stream
		ips = new InputPacketStream();
		ips->setFileNameConfig(packetConfig.c_str());
		ips->createStreamStructure();
		inputPacket = ips->getPacketType(packetID);
		//cout << "IP " << inputPacket << endl;
		//header = new AGILEPacketHeader(inputPacket, outputPacket);

	}
	catch (PacketException* e) {
		cout << "AGILETelem::AGILEPacket::AGILEPacket(string packetConfig): " << e->geterror()
				<< endl;
	}

}


AGILETelem::AGILEPacket::~AGILEPacket() {
	if (in)
		in->close();
	if (out)
		out->close();
}

void AGILETelem::AGILEPacket::printPacket_output() {
	cout << "HEADER ----------" << endl;
	char** r = outputPacket->header->printValue();
	printListOfString(r);
	cout << outputPacket->header->outputstream->printStreamInHexadecimal()
			<< endl;
	cout << "max dimension: " << outputPacket->header->getDimension() << endl;
	cout << "DATA FIELD HEADER ----------" << endl;
	r = outputPacket->dataField->dataFieldHeader->printValue();
	printListOfString(r);
	cout << "max dimension: "
			<< outputPacket->dataField->dataFieldHeader->getDimension() << endl;
	cout << "SOURCE DATA FIELD ----------" << endl;
	/// Get a pointer to the source data field
	SDFRBlock* sdf = (SDFRBlock*) outputPacket->dataField->sourceDataField;
	cout << "max dimension: " << sdf->getDimension() << endl;
	sdf->printValueStdout();
	cout << "DIM: " << outputPacket->getDimension() << endl;
	cout << "MAXDIM: " << outputPacket->getMaxDimension() << endl;

}

void AGILETelem::AGILEPacket::writePacket() {
	ops->writePacket(outputPacket);
}

ByteStreamPtr AGILETelem::AGILEPacket::readPacket(dword pos) {
	//set the position
	if(pos != -1) {
		InputFile* ipf = (InputFile*) in;
		ipf->setpos(pos);
	}
	//read the packet
	inputPacket = ips->readPacket();
	if (inputPacket == 0)
		return 0;
	if (inputPacket->getPacketID() != packetID)
		return 0;
	return inputPacket->getInputStream();
}

void AGILETelem::AGILEPacket::readPacketPy() {
	readPacket();
}

void AGILETelem::AGILEPacket::printPacket_input() {
	cout << "HEADER ----------" << endl;
	char** r = inputPacket->header->printValue();
	printListOfString(r);
	cout << inputPacket->header->getByteStream()->printStreamInHexadecimal() << endl;
	cout << "max dimension in byte of the header: "
			<< inputPacket->header->getDimension() << endl;
	cout << "packet length " << inputPacket->header->getPacketLength() << endl;
	cout << "DATA FIELD HEADER ----------" << endl;
	r = inputPacket->dataField->dataFieldHeader->printValue();
	printListOfString(r);
	cout << "max dimension: "
			<< inputPacket->dataField->dataFieldHeader->getDimension() << endl;
	cout << "SOURCE DATA FIELD ----------" << endl;
	/// Get a pointer to the source data field
	SDFRBlock* sdf = (SDFRBlock*) inputPacket->dataField->sourceDataField;
	sdf->printValueStdout();
	cout << "TOTAL DIM OF THE PACKET : " << inputPacket->getDimension() << endl;
	cout << "MAXDIM OF THE PACKET : " << inputPacket->getMaxDimension() << endl;

}

/*ByteStreamPtr AGILETelem::AGILEPacket::getInputPacketData() {
	return inputPacket->packet;
}*/

dword AGILETelem::AGILEPacket::getInputPacketDimension(ByteStreamPtr stream) {
	//cout << "C: " << inputPacket->getDimension() << endl;
    //inputPacket->verifyPacketValue(stream);
    //return inputPacket->getDimension();

	return ips->getPacketDimension(stream);
}

int AGILETelem::AGILEPacket::getInputPacketType(ByteStreamPtr stream) {

	return ips->detPacketType(stream);
}

bool AGILETelem::AGILEPacket::setStream(ByteStreamPtr stream) {
	//cout << inputPacket << endl;
	inputPacket->setPacketValue(stream->stream);
}
