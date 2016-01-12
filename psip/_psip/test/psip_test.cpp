//============================================================================
// Name        : psip_test.cpp
// Author      : erato
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <locale>
#include <string>
#include <iomanip>
#include <unistd.h>
using namespace std;


/**
 * mac osx의 경
 * 실행하기 위해서는 다음과 같이 DYLD_LIBRARY_PATH의 경로를 선언해주어야 한다.
 * export DYLD_LIBRARY_PATH=/Users/erato/workspace/psip/libdvbsi++-/libs:$DYLD_LIBRARY_PATH
 */

#include "program_association_section.h"
#include "PSIP_Descriptor.h"
#include "PSIP_ServiceLocationDescriptor.h"
#include "PSIP_SystemTimeSection.h"
#include "PSIP_MasterGuideSection.h"
#include "PSIP_TerrestrialVirtualChannelSection.h"
#include "PSIP_CableVirtualChannelSection.h"
#include "PSIP_EventInformationSection.h"
#include "PSIP_ExtendedTextSection.h"
#include "PSIP_RatingRegionSection.h"
#include "PSIP_ProgramMapSection.h"

static void GetSectionData(int PID, int TID, u_char* tsbuffer, int length);
static void GetPESData(int PID, u_char* tsbuffer, int length);

static int gs_section_option_parse_or_channel_map = 0;
#define MPEG2_VIDEO_PARSE_	(1)

void print_wide(const wchar_t* wstr)
{
    std::mbstate_t state = std::mbstate_t();
    int len = 1 + std::wcsrtombs(nullptr, &wstr, 0, &state);
    std::vector<char> mbstr(len);
    std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state);
    std::cout << "multibyte string: " << &mbstr[0] << '\n'
              << "Length, including '\\0': " << mbstr.size() << '\n';
}

std::string wide2string(const wchar_t* wstr)
{
	static bool locale = false;
	if ( !locale ) {
		std::setlocale(LC_ALL, "en_US.utf8");
		locale = true;
	}

    std::mbstate_t state = std::mbstate_t();
    int len = 1 + std::wcsrtombs(nullptr, &wstr, 0, &state);
    std::vector<char> mbstr(len);
    std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state);

    std::string str("");
    str.assign(&mbstr[0], mbstr.size());
    return str;
}

void OnSectionData(int pid, int tid, u_char* section, size_t length)
{
	if ( !gs_section_option_parse_or_channel_map ) {
		static int once = 0;

		if ( !once ) {

			printf("<----- OnSectionData(pid=0x%X, tid=0x%X, length=%d\n", pid, tid, length);

			if ( 0x1FFB == pid ) {
				if ( 0xCA == tid ) {
					PSIP_RatingRegionSection rrt(section);
					rrt.dump();
				}
				else if ( 0xCD == tid ) {
					PSIP_SystemTimeSection stt(section);
					stt.dump();
				}
				else if ( 0xC7 == tid ) {
					PSIP_MasterGuideSection mgt(section);
					mgt.dump();
	#if 0
					printf("====================================\n");
					const PSIP_MasterGuideInfoList* p = mgt.getTableInfo();
					PSIP_MasterGuideInfoList::const_iterator it;
					for ( it = p->begin(); it != p->end(); it++ ) {
						printf("master guide info\n");
						printf("	table_type = 0x%X\n", (*it)->getTableType());
						printf("	table_type_PID = 0x%X\n", (*it)->getTableTypePID());
						printf("	table_type_version_number = 0x%X\n", (*it)->getTableTypeVersionNumber());
						printf("	number_bytes = 0x%X\n", (*it)->getNumberBytes());

						const DescriptorList* dlist = (*it)->getDescriptors();
						DescriptorList::const_iterator it;
						for ( it = dlist->begin(); it != dlist->end(); it++ ) {
							switch ( (*it)->getTag() ) {
							case PSIP_Descriptor::PSIP_AC3_AUDIO_DESCRIPTOR:
								break;
							case PSIP_Descriptor::PSIP_CAPTION_SERVICE_DESCRIPTOR:
								break;
							case PSIP_Descriptor::PSIP_CONTENT_ADVISORY_DESCRIPTOR:
								break;
							case PSIP_Descriptor::PSIP_EXTENDED_CHANNEL_NAME_DESCRIPTOR:
								break;
							case PSIP_Descriptor::PSIP_SERVICE_LOCATION_DESCRIPTOR:
								break;
							case PSIP_Descriptor::PSIP_TIME_SHIFTED_SERVICE_DESCRIPTOR:
								break;
							case PSIP_Descriptor::PSIP_COMPONENT_NAME_DESCRIPTOR:
								break;
							}
						}
					}
	#endif
				}
				else if ( 0xC8 == tid ) {
					PSIP_TerrestrialVirtualChannelSection tvct(section);
	#if 0
					const PSIP_VirtualChannelInfoList* p = tvct.getChannelInfo();
					PSIP_VirtualChannelInfoList::const_iterator it = p->begin();
					(*it)->dump();
					once = true;
	#else
					tvct.dump();
	#endif
				}
				else if ( 0xC9 == tid ) {
					PSIP_CableVirtualChannelSection cvct(section);
					cvct.dump();
				}
				once = true;
			}
			else if ( PSIP_EventInformationSection::TID == tid ) {
				PSIP_EventInformationSection eit(section);
				eit.dump();
				once = true;
			}
			else if ( PSIP_ExtendedTextSection::TID == tid ) {
				PSIP_ExtendedTextSection ett(section);
				ett.dump();
				once = true;
			}
			else if ( PSIP_ProgramMapSection::TID == tid ) {
				PSIP_ProgramMapSection pmt(section);
				pmt.dump();
				once = true;
			}
			else if ( 0x00 == pid && 0x00 == tid ) {
				printf("-> [ ProgramAssociationSection ]\n");
				ProgramAssociationSection pat(section);
				const ProgramAssociationList* loop = pat.getPrograms();
				ProgramAssociationList::const_iterator it;
				for ( it = loop->begin(); it != loop->end(); it++ ) {
					printf("	program_number = %d, pid = %d\n", \
							(*it)->getProgramNumber(), (*it)->getProgramMapPid());

				}
				printf("<- [ ProgramAssociationSection ]\n");
				once = true;
			}
		}
	}
	else {
		static bool isDone = false;
		if ( isDone ) {
			return;
		}

		if ( 0x1FFB == pid && 0xC9 == tid ) {
			static uint8_t section_count = 0;
			static uint8_t last_section_number = 0;
			static PSIP_CableVirtualChannelSection* cvct_list[256] = {NULL, };

			PSIP_CableVirtualChannelSection* vct = NULL;
			vct = new PSIP_CableVirtualChannelSection(section);
			assert(NULL != vct);

			if ( 0x00 == last_section_number || \
					vct->getLastSectionNumber() != last_section_number ) {
				last_section_number = vct->getLastSectionNumber();
			}

			uint8_t section_number = vct->getSectionNumber();
			if ( cvct_list[section_number] ) {
				delete vct;
				vct = NULL;
				return;
			}

			cvct_list[section_number] = vct;
			++section_count;

			// received all section?
			if ( last_section_number < section_count ) {
				isDone = true;
			}

			if ( isDone ) {
				printf("=================== received all vct section! ======================\n");
				printf("	last_section_number = %d\n", last_section_number);
				printf("	total_section_count = %d\n", section_count);
				for ( uint8_t i = 0; i < section_count; i++ ) {
					if ( cvct_list[i] ) {

						const PSIP_VirtualChannelInfoList* _info = cvct_list[i]->getChannelInfo();
						PSIP_VirtualChannelInfoList::const_iterator it;
						for ( it = _info->begin(); it != _info->end(); it++ ) {
							if ( (*it)->isHidden() ) {
								continue;
							}

							printf("		[ ");
							std::wstring 	_wstr;
							for ( int k = 0; k < 7; k++ ) {
	//							printf(" 0x%X ", (*it)->getShortName()[k]);
								_wstr.push_back((*it)->getShortName()[k]);
							}

							std::string _str = wide2string(_wstr.c_str());
							printf("%s ]\n", _str.c_str());
							printf("			channel number(%d - %d), one_part_channel_number = %d\n", \
									(*it)->getMajorChannelNumber(), (*it)->getMinorChannelNumber(), (*it)->getOnePartChannelNumber());
							printf("			%dhz %s\n", (*it)->getCarrierFrequency(), \
														(*it)->getModulationMode() == 0x03 ? "256-QAM" : \
																(*it)->getModulationMode() == 0x04 ? "8-VSB" : \
																		(*it)->getModulationMode() == 0x05 ? "16-VSB" : \
																				(*it)->getModulationMode() == 0x01 ? "Analog" : "64-QAM");
							printf("			TSID : %d, program_number = %d\n", (*it)->getChannelTSID(), (*it)->getProgramNumber());
							printf("			hidden : %d, accessControlled = %d\n", (*it)->isHidden(), (*it)->isAccessControlled());
							printf("			source id : %d, service type = %s\n", (*it)->getSourceId(), \
														(*it)->getServiceType() == 0x01 ? "Analog-TV" : \
																(*it)->getServiceType() == 0x02 ? "ATSC-Digital-TV" : \
																		(*it)->getServiceType() == 0x03 ? "ATSC-Audio-Only" : \
																				(*it)->getServiceType() == 0x04 ? "ATSC-DataBroadcast" : "None");

							const DescriptorList* dlist = (*it)->getDescriptors();
							DescriptorList::const_iterator it_d;
							for ( it_d = dlist->begin(); it_d != dlist->end(); it_d++ ) {
								if ( PSIP_Descriptor::PSIP_SERVICE_LOCATION_DESCRIPTOR == (*it_d)->getTag() ) {
									PSIP_ServiceLocationDescriptor* p = static_cast<PSIP_ServiceLocationDescriptor*>(*it_d);
									printf("			[ stream info ]\n");
									printf("				PCR_PID = 0x%X\n", p->getPcrPid());
									const PSIP_ServiceLocationElementList* _es = p->getServiceLocationElementList();
									PSIP_ServiceLocationElementList::const_iterator it_es;
									for ( it_es = _es->begin(); it_es != _es->end(); it_es++ ) {
										printf("				stream type = 0x%X, PID = 0x%X, language = %s\n", \
												(*it_es)->getStreamType(), (*it_es)->getElementaryPID(), (*it_es)->getIso639LanguageCode());
									}
									break;
								}
							}
						}

						delete cvct_list[i];
						cvct_list[i] = NULL;
					}
				}
			}

	#if 0
			// unicode to utf8
			wchar_t wstr[] = {0x43, 0x4D, 0x42, 0x20, 0x56, 0x4F, 0x44, 0x00};
			std::wstring 	_wstr = &wstr[0];
			std::string		str(_wstr.begin(), _wstr.end());
			cout << "convert to string = " << str << endl;

			// utf8 to unicode
			_wstr.assign(str.begin(), str.end());
			std::wstring::iterator it;
			cout << "convert to string = ";
			for ( it = _wstr.begin(); it != _wstr.end(); it++ ) {
				printf("0x%X ", (*it));
			}
			cout << endl;
	#endif
		}
	}
}

void OnPESData(int pid, u_char* pes, size_t length)
{
	static int once = 0;

	if ( !once ) {
//		printf("<--- onpes data size = %d\n", length);

		uint32_t pes_packet_start_code = 0;
		int index = 0;
		pes_packet_start_code = (pes[index++] & 0xFF) << 16;
		pes_packet_start_code |= (pes[index++] & 0xFF) << 8;
		pes_packet_start_code |= pes[index++] & 0xFF;

		if ( 0x000001 == pes_packet_start_code ) {
#if MPEG2_VIDEO_PARSE_
			uint8_t	stream_id = 0;
			stream_id = pes[index++] & 0xFF;
			if ( 0xE0 && stream_id ) {
				uint32_t pes_packet_length = 0;
				uint32_t pes_header_data_length = 0;
				uint32_t data_length = 0;
				pes_packet_length = (pes[index++] & 0xFF) << 8;
				pes_packet_length |= pes[index++] & 0xFF;

				index += 2;

				pes_header_data_length = pes[index++] & 0xFF;
				if ( 0x00 != pes_packet_length ) {
					data_length = pes_packet_length - (2 +  pes_header_data_length);
				}
				else {
					data_length = length - (index +  pes_header_data_length);
				}

//				printf("pes_packet_length = %d\n", pes_packet_length);
//				printf("pes_header_data_length = %d\n", pes_header_data_length);
//				printf("data_length = %d\n", data_length);

				index += pes_header_data_length;

				// skip stuffing bytes
				for ( int i = 0; i < 32; i++ ) {
					if ( 0xFF == (pes[index] & 0xFF) ) {
						printf("<---- stuffing byte?\n");
						++index;
					}
					else {
						break;
					}
				}

				uint32_t _start_code = 0;

				while ( index < (int) data_length ) {
					_start_code = (pes[index++] & 0xFF) << 16;
					_start_code |= (pes[index++] & 0xFF) << 8;
					_start_code |= pes[index++] & 0xFF;

					if ( 0x000001 != _start_code ) {
						continue;
					}

					uint8_t _stream_type = pes[index++] & 0xFF;
					if ( 0xB3 == _stream_type  ) {
						printf("sequence header = 0x%X\n", _stream_type);
					}
					else if ( 0xB2 == _stream_type  ) {
						printf("user data = 0x%X\n", _stream_type);
					}
					else if ( 0x00 == _stream_type  ) {
						printf("picture = 0x%X\n", _stream_type);
					}
					else if ( 0xB8 == _stream_type  ) {
						printf("GOP = 0x%X\n", _stream_type);
					}
					else if ( 0xB7 == _stream_type  ) {
						printf("sequence end = 0x%X\n", _stream_type);
					}
				}

				once = true;
			}
#endif //MPEG2_VIDEO_PARSE_
		}
	}
}

void print_useage()
{
	cout << "Usage psip_test : " << endl;
	cout << "	-f : " << "input stream file" << endl;
	cout << "	-m : " <<"section or pes mode option :" << endl;
	cout << "		section(default:0), pes(1)" << endl;
	cout << "	-o : " << "section parse or channel map option :" << endl;
	cout << "		section parse(default:0), channel map(1)" << endl;
	cout << "	-p : " << "pid" << endl;
	cout << "	-t : " << "table id" << endl;
}

int main(int argc, char* argv[]) {

	cout << "!!! PSIP TEST !!!" << endl;

	char* in_file = NULL;
	int section_or_pes = 0;							// default : section
	int section_option_parse_or_channel_map = 0;	// default : parse
	int pid = -1, tid = -1;

	int opt = 0;
	while ((opt = getopt(argc, argv, "f:m:o:p:t:h:")) != -1) {
		switch ( opt ) {
		case 'f':
			in_file = optarg;
  			break;
		case 'm':
			sscanf(optarg, "%d", &section_or_pes);
			break;
		case 'o':
			sscanf(optarg, "%d", &section_option_parse_or_channel_map);
			break;
		case 'p':
			sscanf(optarg, "%d", &pid);
			break;
		case 't':
			sscanf(optarg, "%d", &tid);
			break;
		case '?':
			if (optopt == 'f') {
				cout << "input stream file" << endl;
			}
			else if (optopt == 'm') {
				cout << "section or pes mode option :" << endl;
				cout << "	section(default:0), pes(1)" << endl;
			}
			else if (optopt == 'o') {
				cout << "section parse or channel map option :" << endl;
				cout << "	section parse(default:0), channel map(1)" << endl;
			}
			else if (optopt == 'p') {
				cout << "pid" << endl;
			}
			else if (optopt == 't') {
				cout << "table id" << endl;
			}
			break;
		default:
			print_useage();
			return -1;
		}
	}

	if ( !in_file ||
			0 > pid || 0 > tid ) {
		print_useage();
		return -1;
	}

	cout << "psip_test option : " << endl;
	cout << "	file : " << in_file << endl;
	cout << "	section_or_pes : " << (!section_or_pes ? "section" : "pes") << endl;
	cout << "	section_option_parse_or_channel_map : " << (!section_option_parse_or_channel_map ? "section parse" : "channel map") << endl;
	cout << "	pid : " << pid << endl;
	cout << "	tid : " << tid << endl;

	gs_section_option_parse_or_channel_map = section_option_parse_or_channel_map;

	FILE* file = ::fopen(in_file, "rb");
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
								cout << "seek " << (int) std::distance(alignData.begin(), it) << "bytes" << endl;
								::fseek(file, std::distance(alignData.begin(), it), SEEK_CUR);
								break;
							}

							it++;
						}
					}
					continue;
				}

				if ( 0 == section_or_pes ) {
					GetSectionData(pid, tid, &readBuffer[0], readSize);
				}
				else {
					GetPESData(pid, &readBuffer[0], readSize);
				}
			}

			memset(&readBuffer[0], 0x00, READ_BUFF_SIZE);
		}
	}

	::fclose(file);
	return 0;
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

static void GetPESData(int PID, u_char* tsbuffer, int length)
{
	static int previouscc = -1;
	static int bufferindex = 0;
	static u_char pesdata[1310720] = {0,};

	if ( 0 > PID )
	{
		// retset
		previouscc = -1;
		bufferindex = 0;
		memset(&pesdata, 0x00, 1310720);
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
				if ( -1 < previouscc && 0 < bufferindex ) {
					OnPESData(pid, &pesdata[0], bufferindex);
				}

				// new pes data
				memset(&pesdata[0], 0x00, 1310720);
				bufferindex = 0;

				int packetLength = (188 - (index + adflength));
				memcpy(&pesdata[bufferindex], &packet[index + adflength], packetLength);
				bufferindex = packetLength;
				previouscc = continuity_counter;
			}
			else if ( -1 < previouscc )
			{
				if ( ((previouscc + 1) & 0x0F) == continuity_counter ) {
					memcpy(&pesdata[bufferindex], &packet[index + adflength], (188 - (index + adflength)));
					bufferindex += (188 - (index + adflength));
					previouscc = continuity_counter;
				}
				else {
					// clear
					memset(&pesdata[0], 0x00, 1310720);
					bufferindex = 0;
					previouscc = -1;
				}
			}
		}
	}
}
