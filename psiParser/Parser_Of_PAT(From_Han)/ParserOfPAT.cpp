//============================================================================
// Name        : ParserOfPAT.cpp
// Author      : Han
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#define HAVE_BYTESWAP_H
#define HAVE_STDINT_H

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <dvbsi++/program_association_section.h>
#include <dvbsi++/descriptor.h>
#include <dvbsi++/short_section.h>
#include <dvbsi++/byte_stream.h>
#include <dvbsi++/application_name_descriptor.h>
#include <dvbsi++/packet_id.h>
#include <dvbsi++/table_id.h>
#include <dvbsi++/name_descriptor.h>

using namespace std;
using namespace boost;


#define HEADER_SINK_BYTES 0x47 // Header -> 0x47
#define PID_PAT 0x0000 // 0x0000 => PAT
#define TID_PAT 0x00 // Table ID 0x00 => PAT
#define TS_PACKET_BYTES 188

#define TRANCEPORT_MASK 0x80
#define PAYLOAD_MASK 0xc0
#define IMPORTANCE_VALUE_MASK 0x20
#define ADAPTION_FIELD_CONTROL_MASK 0x30
#define ADAPTION_FIELD_MASK 0xff

#define EXIST_PAYLOAD 0x10
#define EXIST_ADAPTION_PAYLOAD 0x30
#define EXIST_ADAPTION 0x20

int main() {

	uint8_t transPortErrorValue = 0, payLoadValue = 0, importanceValue = 0;
	uint8_t adaptionFieldControlValue = 0x00;

	ifstream TsFile("file/test.ts", ifstream::binary);

	char *buffer = new char[TS_PACKET_BYTES];
	char *payLoadOffSet;

	while(!TsFile.eof()){

		TsFile.read(buffer,TS_PACKET_BYTES);

		if( buffer[0] == HEADER_SINK_BYTES ) {

			transPortErrorValue = (buffer[1] & TRANCEPORT_MASK);
			payLoadValue = (buffer[1] & PAYLOAD_MASK);
			importanceValue =  (buffer[1] & IMPORTANCE_VALUE_MASK);

			cout.flags(ios::internal | ios::hex | ios::showbase);
			cout << "----CHECK 3BIT FOLLOWING THE HEADER----" <<endl;
			cout << "transPortError_Check : " <<(int)transPortErrorValue <<endl<<endl;
			cout << "payLoad_Check : "<< (int)payLoadValue <<endl<<endl;
			cout << "importanceValue_Check : "<< (int)importanceValue <<endl<<endl;

			if(PID_PAT == (DVB_PID(&buffer[1])) ){

				adaptionFieldControlValue = (buffer[3] & ADAPTION_FIELD_CONTROL_MASK);
				cout << "adaption_field_control_Check : "<< (int)adaptionFieldControlValue <<endl<<endl;
				cout << "adaption_field_length : "<< (int)(buffer[4])<<endl; // TODO : 포인터 표시를 배열로 바꾸는 것이 좋을 것 같음
				// TODO : if -> case

				switch (adaptionFieldControlValue) {

					case EXIST_PAYLOAD : payLoadOffSet = buffer + 5 ; break;
					case EXIST_ADAPTION :  ; break;
					case EXIST_ADAPTION_PAYLOAD : ; break;
					default : ;
				}
				break;
			}
		}
	}

	ProgramAssociationSection programAssociationSection((const uint8_t *) payLoadOffSet);
	ProgramAssociationList const *programAssociationList = programAssociationSection.getPrograms();
	NetworkAssociationList const *networkAssociationList = programAssociationSection.getNetworks();


	cout << "------PROGRAM_ACCOSIATION_SECTION------" <<endl; // TODO : endl 연속 되는 것 바꾸기
	cout << endl;
	cout << "table_id : ";
	cout  << (int)(programAssociationSection.getTableId()) << endl <<endl;

	cout << "section_syntax_indicator : ";
	cout  << (int)programAssociationSection.getSectionSyntaxIndicator() << endl<<endl;

	cout << "section_length : ";
	cout.width(4); // TODO : 설명이 부족
	cout << setfill('0') << programAssociationSection.getSectionLength() << endl<<endl;

	cout << "table_id_extension : ";
	cout.width(5); // TODO : 출력이 잘못 되었다.
	cout << setfill('0') << programAssociationSection.getTableIdExtension() << endl<<endl;

	cout << "current_next_indicator : ";
	cout  << (int)(programAssociationSection.getCurrentNextIndicator()) << endl<<endl;

	cout << "section_number : ";
	cout  << (int)(programAssociationSection.getSectionNumber()) << endl<<endl;

	cout << "last_section_number : ";
	cout  << (int)(programAssociationSection.getLastSectionNumber()) << endl<<endl;

	cout << "crc_32 : ";
	cout.width(10); // TODO : 왜 10으로 표시했는지 모른다. 궁금해지지 않게
	cout << setfill('0') << programAssociationSection.getCrc32() << endl<<endl;
	cout <<endl;
	cout << "----------------PROGRAMS---------------" <<endl;
	cout <<endl;

	/*
	for(NetworkAssociationList::const_iterator  networkAssociationIterator = networkAssociationList->begin();
			networkAssociationIterator != networkAssociationList->end();networkAssociationIterator++){

		cout<< "network_information_table_ID : " << (*networkAssociationIterator)->getNetworkPid() << endl;
	}
	 */
	/*
	int programCount = 1; // TODO : num -> Count, 시작을 0으로
	for(ProgramAssociationList::const_iterator programAssociationIterator = programAssociationList->begin();
			programAssociationIterator != programAssociationList->end(); programAssociationIterator++){

		cout.flags(ios::internal | ios::dec | ios::showbase);
		cout<<"PROGRAM "<< programCount << endl; programCount++;
		cout.flags(ios::internal | ios::hex | ios::showbase);
		cout<< "program_number : " << (*programAssociationIterator)->getProgramNumber() << endl;
		cout<< "program_map_PID : " << (*programAssociationIterator)->getProgramMapPid() << endl;
		cout<<endl;
	}
	*/

	BOOST_FOREACH( NetworkAssociation *networkAssociationElement, *networkAssociationList){
		cout<< format("network_information_table_pid : \t 0x%04X") % networkAssociationElement->getNetworkPid() << endl;

	}

	BOOST_FOREACH( ProgramAssociation *programAssociationElement, *programAssociationList) {
		cout << format("program_number : \t 0x%04X") % programAssociationElement->getProgramNumber() << endl;
		cout << format("program_map_PID : \t 0x%04X") % programAssociationElement->getProgramMapPid() << endl;
		cout << endl;
	}

	TsFile.close();
	delete[] buffer;
	return 0;
}
