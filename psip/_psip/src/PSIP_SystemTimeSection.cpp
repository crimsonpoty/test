/*
 * PSIP_SystemTimeSection.cpp
 *
 *  Created on: 2015. 9. 23.
 *      Author: erato
 */

#include <time.h>
#include <typeinfo>
#include "PSIP_Descriptor.h"
#include "PSIP_SystemTimeSection.h"

// reference : http://www.leapsecond.com/tools/gpsdate.c
/*
 * Return Modified Julian Day given calendar year,
 * month (1-12), and day (1-31).
 * - Valid for Gregorian dates from 17-Nov-1858.
 * - Adapted from sci.astro FAQ.
 */

long
DateToMjd (long Year, long Month, long Day)
{
    return
        367 * Year
        - 7 * (Year + (Month + 9) / 12) / 4
        - 3 * ((Year + (Month - 9) / 7) / 100 + 1) / 4
        + 275 * Month / 9
        + Day
        + 1721028
        - 2400000;
}

/*
 * Convert Modified Julian Day to calendar date.
 * - Assumes Gregorian calendar.
 * - Adapted from Fliegel/van Flandern ACM 11/#10 p 657 Oct 1968.
 */

void
MjdToDate (long Mjd, long *Year, long *Month, long *Day)
{
    long J, C, Y, M;

    J = Mjd + 2400001 + 68569;
    C = 4 * J / 146097;
    J = J - (146097 * C + 3) / 4;
    Y = 4000 * (J + 1) / 1461001;
    J = J - 1461 * Y / 4 + 31;
    M = 80 * J / 2447;
    *Day = J - 2447 * M / 80;
    J = M / 11;
    *Month = M + 2 - (12 * J);
    *Year = 100 * (C - 49) + Y + J;
}

/*
 * Convert GPS Week and Seconds to Modified Julian Day.
 * - Ignores UTC leap seconds.
 */

long
GpsToMjd (long GpsCycle, long GpsWeek, long GpsSeconds)
{
    long GpsDays;

    GpsDays = ((GpsCycle * 1024) + GpsWeek) * 7 + (GpsSeconds / 86400);
    return DateToMjd(1980, 1, 6) + GpsDays;
}

static const int _UTC_OFFSET = 315964800;

PSIP_SystemTimeSection::PSIP_SystemTimeSection(const uint8_t * const buffer)
 : LongCrcSection(buffer)
 , system_time(0)
 , protocol_version(0)
 , GPS_UTC_offset(0)
 , daylight_savings(0)
{
	if ( 16 < sectionLength ) {
		int index = 8;
		protocol_version = buffer[index++] & 0xFF;
		system_time = (buffer[index++] & 0xFF) << 24;
		system_time |= (buffer[index++] & 0xFF) << 16;
		system_time |= (buffer[index++] & 0xFF) << 8;
		system_time |= buffer[index++] & 0xFF;
		GPS_UTC_offset = buffer[index++] & 0xFF;
		daylight_savings = (buffer[index++] & 0xFF) << 8;
		daylight_savings |= buffer[index++] & 0xFF;

		mDaylight_saving.DS_status = (daylight_savings & 0x8000) >> 15;
		mDaylight_saving.DS_day_of_month = (daylight_savings & 0x1F00) >> 8;
		mDaylight_saving.DS_hour = daylight_savings & 0x00FF;

		uint16_t bytesLeft = (sectionLength - 5 - 8);	// 5 : table_id_extention ~ last_sectoin_number, 8 : protocol_version ~ daylight_savings
		bytesLeft -= 4;	// crc32

		uint16_t loopLength = 0;
		while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
			descriptorPsip(&buffer[index]);
			index += loopLength;
			bytesLeft -= loopLength;
		}
	}
}

PSIP_SystemTimeSection::~PSIP_SystemTimeSection()
{

}

uint8_t PSIP_SystemTimeSection::getProtocolVersion() const
{
	return protocol_version;
}

uint32_t PSIP_SystemTimeSection::getSystemTime() const
{
	return system_time;
}

uint8_t PSIP_SystemTimeSection::getGpsUtcOffset() const
{
	return GPS_UTC_offset;
}

time_t PSIP_SystemTimeSection::getConvertedUtcTime() const
{
	return (_UTC_OFFSET + system_time - GPS_UTC_offset);
}


PSIP_SystemTimeSection::daylight_saving* PSIP_SystemTimeSection::getDaylight_saving() const
{
	return const_cast<daylight_saving*>(&mDaylight_saving);
}

void PSIP_SystemTimeSection::dump()
{
	printf("[ -> PSIP_SystemTimeSection ]\n");
	printf("	protocol_version = 0x%X\n", protocol_version);
	printf("	system_time = 0x%X ", system_time);
	time_t _time = _UTC_OFFSET + system_time - GPS_UTC_offset;
	// TODO : korean time -9 hour : ex) _time -= (9 * 3600)
	struct tm* _localtime = localtime(&_time);
	printf(": %s", asctime (_localtime));
	printf("	GPS_UTC_offset = 0x%X\n", GPS_UTC_offset);
	printf("	daylight_savings = 0x%X\n", daylight_savings);
	printf("	daylight_saving.DS_status = 0x%X\n", mDaylight_saving.DS_status);
	printf("	daylight_saving.DS_day_of_month = 0x%X\n", mDaylight_saving.DS_day_of_month);
	printf("	daylight_saving.DS_hour = 0x%X\n", mDaylight_saving.DS_hour);

	const DescriptorList* dlist = PSIP_DescriptorContainer::getDescriptors();
	DescriptorList::const_iterator it;
	for ( it = dlist->begin(); it != dlist->end(); it++ ) {
		switch ( (*it)->getTag() ) {
		case PSIP_Descriptor::PSIP_AC3_AUDIO_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_CAPTION_SERVICE_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_CONTENT_ADVISORY_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_EXTENDED_CHANNEL_NAME_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_SERVICE_LOCATION_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_TIME_SHIFTED_SERVICE_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_COMPONENT_NAME_DESCRIPTOR:
			static_cast<PSIP_Descriptor*>(*it)->dump();
			break;
		}
	}
#if 0
	long Mjd;
	long Year, Month, Day;
	Mjd = GpsToMjd(0, 0, system_time);
	MjdToDate(Mjd, &Year, &Month, &Day);

	printf("Gps Seconds 0x%X ", system_time);
	printf(" = Mjd %ld = %.4ld-%.2ld-%.2ld\n",
			Mjd, Year, Month, Day);
#endif
	printf("[ <- PSIP_SystemTimeSection ]\n");
}
