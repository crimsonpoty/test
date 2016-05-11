/**
	*	@file		Main.cpp
	*	@date		2015. 02. 06
	*	@author		BenjaminTeam, k150030
	*	@brief		The Program of Paring TS-file
	*	@remarks
	*/

#include <fstream>
#include <iostream>
#include "Common.h"
#include <dvbsi++/program_association_section.h>

#include "IntrawayMessage.h"

#define TS_PACKET_SIZE 188

using namespace std;


static void GetSectionData(int PID, int TID, u_char* tsbuffer, int length);

static uint32_t issueId[] = {
	2155393433, 2153962154, 2157073514, 2153953502, 2156966052, 2153961525, 2156079615, 2156090059, 2156089755, 1861464413, 1860201202, 2156100606, 2155400188, 1861486662, 1861473005
};

int main(int argc, char *argv[])
{

	if(2 != argc)
	{
		printf("ERROR: Input file in arguments\n");
		return -1;
	}

	FILE* file = ::fopen(argv[1], "rb");
	if ( file ) {
		static const int READ_BUFF_SIZE = 188 * 7;

		int readSize = 0;
		u_char readBuffer[READ_BUFF_SIZE] = {0,};

		while ( !::feof(file) ) {
			readSize = ::fread(&readBuffer[0], sizeof(u_char), READ_BUFF_SIZE, file);
			if ( 0 < readSize ) {
				// 0. check sync(0x47) byte
				if ( 0x47 != readBuffer[0] ) {
					// assign data
					std::vector<u_char> alignData(&readBuffer[0], &readBuffer[0] + readSize);
					std::vector<u_char>::iterator it;

					it = alignData.begin();
					while ( it != alignData.end() && (376 < std::distance(it, alignData.end())) ) {
						it = std::find(it, alignData.end(), 0x47);
						if ( alignData.end() != it ) {
							if ( 0x47 == (*(it + 188)) && 0x47 == (*(it + 376)) ) {
								// mpeg2 ts!
								std::cout << "seek " << (int) std::distance(alignData.begin(), it) << "bytes" << endl;
								::fseek(file, std::distance(alignData.begin(), it), SEEK_CUR);
								break;
							}

							it++;
						}
					}
					continue;
				}

				GetSectionData(0x3E7, 0xF0, &readBuffer[0], readSize);
			}

			memset(&readBuffer[0], 0x00, READ_BUFF_SIZE);
		}
	}

	::fclose(file);

	return 0;
}


void OnSectionData(int pid, int tid, u_char* section, size_t length)
{
	static int once = 0;

	if ( !once ) {

		// printf("<----- OnSectionData(pid=0x%X, tid=0x%X, length=%zu\n", pid, tid, length);

		if (0x3E7 == pid && 0xF0 == tid) {
			CIntrawayMessageSection iwm(section);
			const IntrawayMessageList* iwmList = iwm.getIntrawayMessages();

			for(IntrawayMessageConstIterator it = iwmList->begin(); it != iwmList->end(); ++it) {
				// printf("sectionLength(%d), addressType(%d), addressLength(%d), stbId(%X), msgId(%X), msgVersion(%d), isCompressed(%d), msgDescLength(%d)\n",  iwm.getSectionLength(), (*it)->getAddressType(), (*it)->getAddressLength(), (*it)->getStbId(), (*it)->getMsgId(), (*it)->getMsgVersion(), (*it)->getIsCompressed(), (*it)->getMsgDescLength());
				// const uint8_t *pText = (*it)->getText();
				// printf("{");
				// for(int i = 0; i < iwm.getSectionLength(); i++) {
					// printf("0x%02X, ", section[i]);
				// }
				// printf("}, \n");
				// if(iwm.getSectionLength() > 1000) {
					// printf("%d\n", iwm.getSectionLength());
				// }
				bool isMatch = false;
				for(int i = 0; i < (int)(sizeof(issueId) / sizeof(uint32_t)); i++) {
					if(issueId[i] == (*it)->getStbId()) {
						isMatch = true;
						break;
					}
				}
				if(isMatch) {
					printf("AddressType: %d, StbId: %d\n", (*it)->getAddressType(), (*it)->getStbId());
					for(int i = 0; i < (int)length; i++) {
						printf("%02X", section[i]);
					}
					printf("\n\n");
				}
			}
			once = false;
		}
	}
}


static void GetSectionData(int PID, int TID, u_char* tsbuffer, int length)
{
	static int previouscc = -1;
	static int bufferindex = 0;
	static u_char sectiondata[4096] = {0,};

	if ( 0 > PID )
	{
		// retset
		previouscc = -1;
		bufferindex = 0;
		memset(&sectiondata, 0x00, 4096);
		return;
	}

	int index = 0;
	int loop = length / 188;
	int transport_error_indicator, payload_unit_start_indicator, pid, adaptation_field_control, continuity_counter;

	for ( int i = 0; i < loop; i++ )
	{
		index = 0;
		u_char* packet = &tsbuffer[i * 188];

		if ( 0x47 != (packet[index++] & 0xFF) )
		{
			previouscc = -1;
			bufferindex = 0;
			continue;
		}

		transport_error_indicator = (packet[index] & 0x80) >> 7;
		if ( transport_error_indicator )
		{
			previouscc = -1;
			bufferindex = 0;
			continue;
		}

		payload_unit_start_indicator = (packet[index] & 0x40) >> 6;
		pid = ((packet[index++] & 0x1F) << 8);
		pid |= (packet[index] & 0xFF);

		if ( pid != PID )
		{
			continue;
		}

		adaptation_field_control = (packet[++index] & 0x30) >> 4;
		continuity_counter = packet[index++] & 0x0F;

#if 0
		cout << "<================================================" << endl;
		printf("transport_error_indicator = %d\n", transport_error_indicator);
		printf("payload_unit_start_indicator = %d\n", payload_unit_start_indicator);
		printf("pid = %d\n", pid);
		printf("adaptation_field_control = %d\n", adaptation_field_control);
		printf("previouscc = %d, continuity_counter = %d\n", previouscc, continuity_counter);
		printf("bufferindex = %d\n", bufferindex);
#endif

		int adflength = 0;

		if ( 0x02 == adaptation_field_control || 0x03 == adaptation_field_control )	// adaptation_field only, no payload or adaptation_field followed by payload
		{
			adflength = packet[index + payload_unit_start_indicator] & 0xFF;
			adflength += 1;	// add 1byte(adaptation_field_length)
		}

		if ( 0x01 == (adaptation_field_control & 0x01) )
		{
			if ( payload_unit_start_indicator )
			{
				int pointfield = packet[index++] & 0xFF;

				if ( -1 < previouscc && 0 < bufferindex )
				{
					// previous data
					memcpy(&sectiondata[bufferindex], &packet[index + adflength], pointfield);
					bufferindex += pointfield;

					// send section data.
					if ( 0 <= TID && TID == (sectiondata[0] & 0xFF) ) {
						OnSectionData(PID, TID, &sectiondata[0], bufferindex);
					}
					else if ( 0 > TID ){
						OnSectionData(PID, (sectiondata[0] & 0xFF), &sectiondata[0], bufferindex);
					}

					memset(&sectiondata[0], 0x00, 4096);
					bufferindex = 0;
				}

				// new section data
				// 183 = 188 - 4(header) - 1(point field)
				memcpy(&sectiondata[bufferindex], &packet[index + pointfield], (183 - pointfield));
				bufferindex = (183 - pointfield);
				previouscc = continuity_counter;
			}
			else if ( -1 < previouscc )
			{
				if ( ((previouscc + 1) & 0x0F) == continuity_counter )
				{
					memcpy(&sectiondata[bufferindex], &packet[index + adflength], 188 - (index + adflength));
					bufferindex += (188 - (index + adflength));
					previouscc = continuity_counter;

					// check stuffing bytes
					if ( 0xFF == (sectiondata[bufferindex - 1] & 0xFF) ) {
						// section length
						int section_length = ((sectiondata[1] & 0x0F) << 8);
						section_length |= (sectiondata[2] & 0xFF);

						// send section data.
						if ( 0 <= TID && TID == (sectiondata[0] & 0xFF) ) {
							OnSectionData(PID, TID, &sectiondata[0],  (section_length + 3));
						}
						else if ( 0 > TID ){
							OnSectionData(PID, (sectiondata[0] & 0xFF), &sectiondata[0], (section_length + 3));
						}

						memset(&sectiondata[0], 0x00, 4096);
						bufferindex = 0;
					}
				}
				else
				{
					// clear
					memset(&sectiondata[0], 0x00, 4096);
					bufferindex = 0;
					previouscc = -1;
				}
			}
		}
	}
}
