/**
	*	@file		Main.cpp
	*	@date		2015. 02. 06
	*	@author		BenjaminTeam, k150030
	*	@brief		The Program of Paring TS-file
	*	@remarks
	*/

#include <fstream>
#include "Common.h"
#include "Program_Association_Section.h"
#include "Program_Map_Section.h"
#include "Network_Information_Section.h"
#include "VirtualChannelTable.h"


#define TS_PACKET_SIZE 188

using namespace std;

enum EPID // ISO_13818-1
{
	ePID_PAT	= 0x0000,
	ePID_CAT	= 0x0001,
	ePID_TSDT	= 0x0002,
	//ePID_NIT	= 0x0010,
	ePID_NIT	= 0x0006,
	ePID_SDT	= 0x0011,
	ePID_EIT	= 0x0012,
	ePID_TOT	= 0x0014,
	ePID_BIT	= 0x0024,
	ePID_VCT	= 0x1FFB,
	ePID_NULL	= 0x1FFF
};
#define MAX_PID ePID_BIT

enum EADAPTATION_FIELD_CONTROL
{
	// eADAPTATION_FIELD_CONTROL_RESERVED,
	eADAPTATION_FIELD_CONTROL_PAYLOAD_ONLY = 1,
	eADAPTATION_FIELD_CONTROL_ADAP_FIELD_ONLY,
	eADAPTATION_FIELD_CONTROL_ADAP_FIELD_FOLLOWED_PAYLOAD
};


class CTS_Adaptation_Field
{
	public:
	CTS_Adaptation_Field() {};
	~CTS_Adaptation_Field() {
	};

	public:
	char* Parse_Adaptation_Field(const char *buffer) {
		char *ptr_buffer = (char *)buffer;
		m_Adaptation_Field_Length = ptr_buffer[0];
		m_Discontinuity_Indicator = FLAG0(ptr_buffer[1]);
		m_Random_Access_Indicator = FLAG1(ptr_buffer[1]);
		m_Elementary_Stream_Priority_Indicator = FLAG2(ptr_buffer[1]);
		m_PCR_Flag = FLAG3(ptr_buffer[1]);
		m_OPCR_Flag = FLAG4(ptr_buffer[1]);
		m_Splicing_Point_Flag = FLAG5(ptr_buffer[1]);
		m_Transport_Private_Data_Flag = FLAG6(ptr_buffer[1]);
		m_Adaptation_Field_Extension_Flag = FLAG7(ptr_buffer[1]);
		ptr_buffer += 2;

		if(1 == m_PCR_Flag) {
			m_Program_Clock_Reference_Base = (bit_swap_32(&ptr_buffer[0]) << 1) | ((ptr_buffer[4] & 0x80) >> 7);
			m_Program_Clock_Reference_Extension = bit_swap_16(&ptr_buffer[4]) & 0x1FF;
			ptr_buffer += 6;
		}
		if(1 == m_OPCR_Flag) {
			m_Original_Program_Clock_Reference_Base = (bit_swap_32(&ptr_buffer[0]) << 1) | ((ptr_buffer[4] & 0x80) >> 7);
			m_Original_Program_Clock_Reference_Extension = bit_swap_16(&ptr_buffer[4]) & 0x1FF;
			ptr_buffer += 6;
		}
		if(1 == m_Splicing_Point_Flag) {
			m_Splice_Countdown = ptr_buffer[0];
			ptr_buffer++;
		}
		if(1 == m_Transport_Private_Data_Flag) {
			m_Transport_Private_Data_Length = ptr_buffer[0];
			ptr_buffer++;
			// m_Private_Data = new char[m_Transport_Private_Data_Length];
			// for(int i = 0; i < m_Transport_Private_Data_Length; i++) {
				// m_Private_Data[i] = *(ptr_buffer);
				// ptr_buffer++;
			// }
		}
		if(1 == m_Adaptation_Field_Extension_Flag) {
			m_Adaptation_Field_Extension_Length = ptr_buffer[0];
			m_Ltw_Flag = FLAG0(ptr_buffer[1]);
			m_Piecewise_Rate_Flag = FLAG1(ptr_buffer[1]);
			m_Seamless_Splice_Flag = FLAG2(ptr_buffer[1]);
			ptr_buffer +=2;
			if(1 == m_Ltw_Flag) {
				m_Ltw_Valid_Flag = FLAG0(ptr_buffer[0]);
				m_Ltw_Offset = bit_swap_16(&ptr_buffer[0]) & 0x7FFF;
				ptr_buffer += 2;
			}
			if(1 == m_Piecewise_Rate_Flag) {
				m_Piecewise_Rate = bit_swap_32(&ptr_buffer[0]) & 0x3FFFFF;
				ptr_buffer += 3;
			}
			if(1 == m_Seamless_Splice_Flag) {
				Splice_Type = (ptr_buffer[0] & 0xF0) >> 4;
				//TODO: later..
				ptr_buffer += 5;
			}
			ptr_buffer++;	//reserved 되어있는 값으로 인해 포인터 증가
		}
		return ptr_buffer;
	}
	void Print_Adaptation_Field();	//TODO: later..
	uint get_Adaptation_Field_Length() {
		return m_Adaptation_Field_Length;
	}

	public:
	uchar m_Adaptation_Field_Length;
	bool m_Discontinuity_Indicator;
	bool m_Random_Access_Indicator;
	bool m_Elementary_Stream_Priority_Indicator	;
	bool m_PCR_Flag;
	bool m_OPCR_Flag;
	bool m_Splicing_Point_Flag;
	bool m_Transport_Private_Data_Flag;
	bool m_Adaptation_Field_Extension_Flag;
	uint64 m_Program_Clock_Reference_Base;
	ushort m_Program_Clock_Reference_Extension;
	uint64 m_Original_Program_Clock_Reference_Base;
	ushort m_Original_Program_Clock_Reference_Extension;
	uchar m_Splice_Countdown;
	uchar m_Transport_Private_Data_Length;
	// char *m_Private_Data; //private data 길이 만큼 동적할당(realloc)이 필요할듯
	uchar m_Adaptation_Field_Extension_Length;
	bool m_Ltw_Flag;
	bool m_Piecewise_Rate_Flag;
	bool m_Seamless_Splice_Flag;
	bool m_Ltw_Valid_Flag;
	ushort m_Ltw_Offset;
	uint32 m_Piecewise_Rate;
	uchar Splice_Type;
	bool m_DTS_Next_AU[33];
	bool m_Marker_Bit[3];
};

class CTS_Packet : public CTS_Adaptation_Field
{
	public:	// TODO: later..
	CTS_Packet() {};
	~CTS_Packet() {};

	public:
	char* Parse_TS_Header(const char *buffer) {
		m_Transport_Error_Indicator		= FLAG0(buffer[0]);
		m_Payload_Unit_Start_Indicator	= FLAG1(buffer[0]);
		m_Transport_Priority			= FLAG2(buffer[0]);
		m_ProgramID						= DVB_PID(&buffer[0]);
		m_Transport_Scrambling_Control	= (buffer[2] & 0xC0) >> 6;
		m_Adaptation_Field_Control		= (buffer[2] & 0x30) >> 4;
		m_Continuity_Counter			= (buffer[2] & 0x0F);
		if(2 == m_Adaptation_Field_Control || 3 == m_Adaptation_Field_Control) {
			return Parse_Adaptation_Field(&buffer[3]);
		}
		else if(1 == m_Adaptation_Field_Control) {
			return (char*)&buffer[3];
		}
		else {
			return (char*)&buffer[3];
		}
	}
	void Print_TS_Header() {
		PARSER_PRINT(("Transport_Error_Indicator: \t%d\n", m_Transport_Error_Indicator))
		PARSER_PRINT(("Payload_Unit_Start_Indicator: \t%d\n", m_Payload_Unit_Start_Indicator))
		PARSER_PRINT(("Transport_Priority: \t\t%d\n", m_Transport_Priority))
		PARSER_PRINT(("ProgramID: \t\t\t0x%X (%d)\n", m_ProgramID, m_ProgramID))
		PARSER_PRINT(("Transport_Scrambling_Control: \t%d\n", m_Transport_Scrambling_Control))
		PARSER_PRINT(("Adaptation_Field_Control: \t%d\n", m_Adaptation_Field_Control))
		PARSER_PRINT(("Continuity_Counter: \t\t%d\n\n", m_Continuity_Counter))
	}
	int get_Payload_Length() {
		int returnValue;

		if(2 == m_Adaptation_Field_Control || 3 == m_Adaptation_Field_Control) {
			returnValue = 188 - 4 - get_Adaptation_Field_Length();
		}
		else {
			returnValue = 188 - 4;
		}

		if(m_Payload_Unit_Start_Indicator) {
			returnValue--;
		}

		return returnValue;
	}

	public:
	static const uchar m_Sync_Byte = 0x47;
	bool m_Transport_Error_Indicator;
	bool m_Payload_Unit_Start_Indicator;
	bool m_Transport_Priority;
	ushort m_ProgramID;
	uchar m_Transport_Scrambling_Control: 2;
	uchar m_Adaptation_Field_Control	: 2;
	uchar m_Continuity_Counter			: 4;
};

int get_Section_Length(const char *buffer) {
	return bit_swap_16(&buffer[1]) & 0xFFF;
}


int main(int argc, char *argv[])
{

	if(2 != argc)
	{
		printf("ERROR: Input file in arguments\n");
		return -1;
	}

	ifstream Ts_Stream_in(argv[1], ios::in | ios::ate | ios::binary);
	uint32 fileSize = Ts_Stream_in.tellg();
	Ts_Stream_in.seekg(0, ios::beg);

	PARSER_PRINT(("\nFile Name : %s\n", argv[1]))
	PARSER_PRINT(("File Size : %.4gMB (%d)\n\n", ((float)fileSize / 1024 / 1024), fileSize))


	char *buffer = new char[TS_PACKET_SIZE];
	char *ptr_buffer;
	// char *ptr_Section[MAX_PID + 1];
	CTS_Packet TS_Packet;
	CProgram_Association_Section Program_Association_Section;
	list<CProgram>::iterator Program_Iterator;
	list<CProgram_Map_Section> PMT_List;
	list<CProgram_Map_Section>::iterator Iter_PMT;
	CNetwork_Infomation_Section Network_Infomation_Section;
	CLong_Section Long_Section;

	static uint32 loop_count;
	static uint32 PAT_Count;
	static uint32 CAT_Count;
	static uint32 PMT_Count;
	static uint32 TSDT_Count;
	static uint32 NIT_Count;
	static uint32 SDT_Count;
	static uint32 EIT_Count;
	static uint32 TOT_Count;
	static uint32 BIT_Count;
	static uint32 VCT_Count;
	// PARSER_PRINT(("Current Loop Count: \t\t%d\n\n", loop_count))

	while(!Ts_Stream_in.eof())
	{
		Ts_Stream_in.read(buffer, TS_PACKET_SIZE);

		if(TS_Packet.m_Sync_Byte == buffer[0])
		{
			loop_count++;
			ptr_buffer = TS_Packet.Parse_TS_Header(&buffer[1]);

			if(1 == TS_Packet.m_Payload_Unit_Start_Indicator) {
				const int Pointer_Field = ptr_buffer[0];
				ptr_buffer = ptr_buffer + Pointer_Field + 1; // 포인터 필드가 있을때, Parsing pointer 조정
			}

			switch(TS_Packet.m_ProgramID){
				case ePID_PAT:
				{
					if(false == Program_Association_Section.checkParse()) {
						Program_Association_Section.Parse(&ptr_buffer[0]);
						Program_Association_Section.Print();	//PAT 출력
					}
					PAT_Count++;
				}
				break;

				case ePID_CAT:
				{
					// TODO: later..
					CAT_Count++;
				}
				break;

				case ePID_TSDT:
				{
					// TODO: later..
					TSDT_Count++;
				}
				break;

				case ePID_NIT:
				{
					if(false == Network_Infomation_Section.checkParse()) {
						if(0x40 == ptr_buffer[0] || 0x41 == ptr_buffer[0]) {
							Long_Section.set_Total_Section_Length(get_Section_Length(&ptr_buffer[0]));
							Long_Section.m_Remained_Length = Long_Section.get_Total_Section_Length();
							Long_Section.Resize(Long_Section.get_Total_Section_Length());
						}

						//Long_Section의 경우
						if((Long_Section.m_Remained_Length > 0) &&
							(TS_Packet.get_Payload_Length() < Long_Section.get_Total_Section_Length())) {

							if(Long_Section.m_Remained_Length > TS_Packet.get_Payload_Length()) {	//Long_Section조립
								Long_Section.Assemble(&ptr_buffer[0], TS_Packet.get_Payload_Length());
								Long_Section.m_Remained_Length -= TS_Packet.get_Payload_Length();
							}
							else {	//Long_Section의 마지막 조각 조립 및 파싱
								Long_Section.Assemble(&ptr_buffer[0], Long_Section.m_Remained_Length);
								Long_Section.m_Remained_Length -= Long_Section.m_Remained_Length;
								Network_Infomation_Section.Parse(Long_Section.get_Buffer());
								Network_Infomation_Section.Print();	//NIT 출력
								NIT_Count++;
							}
						}
						else {	//Long_Section이 아닐때, 바로 파싱
							Network_Infomation_Section.Parse(&ptr_buffer[0]);
							Network_Infomation_Section.Print();	//NIT 출력
							NIT_Count++;
						}
					}
					NIT_Count++;
				}
				break;

				case ePID_SDT:
				{
					// TODO: later..
					SDT_Count++;
				}
				break;

				case ePID_EIT:
				{
					// TODO: later..
					EIT_Count++;
				}
				break;

				case ePID_TOT:
				{
					// TODO: later..
					TOT_Count++;
				}
				break;

				case ePID_BIT:
				{
					// TODO: later..
					BIT_Count++;
				}
				break;

				case ePID_VCT:
					{
						if(!VCT_Count) {
							App::CVirtualChannelTable VirtualChannelTable((uchar*)ptr_buffer, Program_Association_Section.get_Number_of_Programs());
							printf("\n--------------------- VCT ---------------------\n\n");
							VirtualChannelTable.PrintContent();
							printf("-----------------------------------------------\n\n");
						}
						VCT_Count++;
					}
					break;

				//Parse PMT
				default:
				{
					if(PMT_List.empty()) {
						Program_Association_Section.iter_Program_List = Program_Association_Section.m_Program_List.begin();
						if(!Program_Association_Section.iter_Program_List->get_Program_Number()) {	// PID가 network_ID이면, 반복자를 1 증가 시킨다.
							Program_Association_Section.iter_Program_List++;
						}
					}
					if(Program_Association_Section.iter_Program_List->get_Program_Map_PID() == TS_Packet.m_ProgramID) {
						PMT_List.push_back(CProgram_Map_Section(&ptr_buffer[0]));
						Program_Association_Section.iter_Program_List++;
						PMT_Count++;
					}
				}
				break;
			}
			// if(1 == NIT_Count) break;	// 무한 루프 방지...
		}
	}

	// PMT 출력
	for(Iter_PMT = PMT_List.begin(); Iter_PMT != PMT_List.end(); Iter_PMT++) { Iter_PMT->Print(); }

	PARSER_PRINT(("Total Loop Count: %d\n", loop_count))
	PARSER_PRINT(("PAT Count: %d\n", PAT_Count))
	PARSER_PRINT(("CAT Count: %d\n", CAT_Count))
	PARSER_PRINT(("PMT Count: %d\n", PMT_Count))
	PARSER_PRINT(("TSDT Count: %d\n", TSDT_Count))
	PARSER_PRINT(("NIT Count: %d\n", NIT_Count))
	PARSER_PRINT(("SDT Count: %d\n", SDT_Count))
	PARSER_PRINT(("EIT Count: %d\n", EIT_Count))
	PARSER_PRINT(("TOT Count: %d\n", TOT_Count))
	PARSER_PRINT(("BIT Count: %d\n", BIT_Count))
	PARSER_PRINT(("VCT Count: %d\n", VCT_Count))

	delete[] buffer;
	Ts_Stream_in.close();

	return 0;
}
