/*
 * PSIP_RatingRegionSection.cpp
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#include <assert.h>
#include <typeinfo>
#include "PSIP_Descriptor.h"
#include "PSIP_RatingRegionSection.h"

PSIP_RatingDimensionValue::PSIP_RatingDimensionValue()
 : abbrev_rating_value_length(0)
 , abbrev_rating_value_text(NULL)
 , rating_value_length(0)
 , rating_value_text(NULL)
{

}

PSIP_RatingDimensionValue::~PSIP_RatingDimensionValue()
{
	if ( abbrev_rating_value_text ) {
		delete[] abbrev_rating_value_text;
		abbrev_rating_value_text = NULL;
	}

	if ( rating_value_text ) {
		delete[] rating_value_text;
		rating_value_text = NULL;
	}
}

uint8_t	PSIP_RatingDimensionValue::getAbbrevRatingValueLength() const
{
	return abbrev_rating_value_length;
}

const uint8_t* PSIP_RatingDimensionValue::getAbbrevRatingValueText() const
{
	return abbrev_rating_value_text;
}

uint8_t	PSIP_RatingDimensionValue::getRatingValueLength() const
{
	return rating_value_length;
}

const uint8_t* PSIP_RatingDimensionValue::getRatingValueText() const
{
	return rating_value_text;
}

void PSIP_RatingDimensionValue::dump()
{
	printf("		[ -> PSIP_RatingDimensionValue ]\n");
	printf("			abbrev_rating_value_length = 0x%X\n", abbrev_rating_value_length);
	printf("			abbrev_rating_value_text = ");
	for ( uint32_t i = 0; i < abbrev_rating_value_length; i++ ) {
		printf(" %c", abbrev_rating_value_text[i]);
	}
	printf("\n");
	printf("			rating_value_length = 0x%X\n", rating_value_length);
	printf("			rating_value_text = ");
	for ( uint32_t i = 0; i < rating_value_length; i++ ) {
		printf(" %c", rating_value_text[i]);
	}
	printf("\n");
	printf("		[ <- PSIP_RatingDimensionValue ]\n");
}


PSIP_RatingDimensionInfo::PSIP_RatingDimensionInfo(const uint8_t* const buffer)
 : dimension_name_length(0)
 , dimension_name_text(NULL)
 , granduated_scale(0)
 , values_defined(0)
 , dimension_info_total_length(0)
{
	int index = 0;
	dimension_name_length = buffer[index++] & 0xFF;
	if ( 0 < dimension_name_length ) {
		dimension_name_text = new uint8_t[dimension_name_length];
		assert(NULL != dimension_name_text);
		for ( uint8_t i = 0; i < dimension_name_length; i++ ) {
			dimension_name_text[i] = buffer[index++] & 0xFF;
		}
	}

	granduated_scale = (buffer[index] & 0x10) >> 4;
	values_defined = buffer[index++] & 0x0F;

	for ( uint8_t i = 0; i < values_defined; i++ ) {
		PSIP_RatingDimensionValue* _value = new PSIP_RatingDimensionValue;
		assert(NULL != _value);
		_value->abbrev_rating_value_length = buffer[index++] & 0xFF;
		if ( 0 < _value->abbrev_rating_value_length ) {
			_value->abbrev_rating_value_text = new uint8_t[_value->abbrev_rating_value_length];
			assert(NULL != _value->abbrev_rating_value_text);
			for ( uint8_t j = 0; j < _value->abbrev_rating_value_length; j++ ) {
				_value->abbrev_rating_value_text[j] = buffer[index++] & 0xFF;
			}
		}

		_value->rating_value_length = buffer[index++] & 0xFF;
		if ( 0 < _value->rating_value_length ) {
			_value->rating_value_text = new uint8_t[_value->rating_value_length];
			assert(NULL != _value->rating_value_text);
			for ( uint8_t j = 0; j < _value->rating_value_length; j++ ) {
				_value->rating_value_text[j] = buffer[index++] & 0xFF;
			}
		}

		mPSIP_RatingDimensionValueList.push_back(_value);
	}

	dimension_info_total_length = index;
}

PSIP_RatingDimensionInfo::~PSIP_RatingDimensionInfo()
{
	if ( dimension_name_text ) {
		delete[] dimension_name_text;
		dimension_name_text = NULL;
	}

	PSIP_RatingDimensionValueList::iterator it;
	for ( it = mPSIP_RatingDimensionValueList.begin(); it != mPSIP_RatingDimensionValueList.end(); ++it ) {
		delete *it;
	}
}

uint8_t	PSIP_RatingDimensionInfo::getDimensionNameLength() const
{
	return dimension_name_length;
}

const uint8_t* PSIP_RatingDimensionInfo::getDimensionNameText() const
{
	return dimension_name_text;
}

const PSIP_RatingDimensionValueList* PSIP_RatingDimensionInfo::getPSIP_RatingDimensionValue() const
{
	return &mPSIP_RatingDimensionValueList;
}

void PSIP_RatingDimensionInfo::dump()
{
	printf("	[ -> PSIP_RatingDimensionInfo ]\n");
	printf("		dimension_name_length = 0x%X\n", dimension_name_length);
	printf("		dimension_name_text = ");
	for ( uint32_t i = 0; i < dimension_name_length; i++ ) {
		printf(" %c", dimension_name_text[i]);
	}
	printf("\n");
	printf("		granduated_scale = 0x%X\n", granduated_scale);
	printf("		values_defined = 0x%X\n", values_defined);
	for ( int i = 0; i < values_defined; i++ ) {
		mPSIP_RatingDimensionValueList[i]->dump();
	}
	printf("	[ <- PSIP_RatingDimensionInfo ]\n");
}

PSIP_RatingRegionSection::PSIP_RatingRegionSection(const uint8_t * const buffer)
 : LongCrcSection(buffer)
 , protocol_version(0)
 , rating_region_name_length(0)
 , rating_region_name_text(NULL)
 , dimensions_defined(0)
 , descriptors_length(0)
{
	if ( 7 < sectionLength ) {
		int index = 8;
		protocol_version = buffer[index++] & 0xFF;
		rating_region_name_length = buffer[index++] & 0xFF;
		if ( 0 < rating_region_name_length ) {
			rating_region_name_text = new uint8_t[rating_region_name_length];
			assert(NULL != rating_region_name_text);
			for ( uint8_t i = 0; i < rating_region_name_length; i++ ) {
				rating_region_name_text[i] = buffer[index++] & 0xFF;
			}
		}
		dimensions_defined = buffer[index++] & 0xFF;

		uint16_t bytesLeft = (sectionLength - 5 - 3 - rating_region_name_length);
		uint16_t loopLength = 0;

		for ( uint8_t i = 0; i < dimensions_defined; i++ ) {
			PSIP_RatingDimensionInfo* _info = new PSIP_RatingDimensionInfo(&buffer[index]);
			assert(NULL != _info);

			loopLength = _info->dimension_info_total_length;
			index += loopLength;
			bytesLeft -= loopLength;
			mPSIP_RatingDimensionInfoList.push_back(_info);
		}

		descriptors_length = (buffer[index++] & 0xFF) << 8;
		descriptors_length |= buffer[index++] & 0xFF;
		bytesLeft = descriptors_length;

		while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
			descriptorPsip(&buffer[index]);
			index += loopLength;
			bytesLeft -= loopLength;
		}
	}
}

PSIP_RatingRegionSection::~PSIP_RatingRegionSection()
{
	if ( rating_region_name_text ) {
		delete[] rating_region_name_text;
		rating_region_name_text = NULL;
	}

	PSIP_RatingDimensionInfoList::iterator it;
	for ( it = mPSIP_RatingDimensionInfoList.begin(); it != mPSIP_RatingDimensionInfoList.end(); ++it ) {
		delete *it;
	}
}

uint8_t	PSIP_RatingRegionSection::getRaingRegion() const
{
	return (tableIdExtension & 0xFF);
}

uint8_t	PSIP_RatingRegionSection::getRaingRegionNameLength() const
{
	return rating_region_name_length;
}

const uint8_t* PSIP_RatingRegionSection::getRaingRegionNameText() const
{
	return rating_region_name_text;
}

const PSIP_RatingDimensionInfoList* PSIP_RatingRegionSection::getRatingDimensionInfo() const
{
	return &mPSIP_RatingDimensionInfoList;
}

void PSIP_RatingRegionSection::dump()
{
	printf("[ -> PSIP_RatingRegionSection ]\n");
	printf("	rating_region = 0x%X\n", getRaingRegion());
	printf("	protocol_version = 0x%X\n", protocol_version);
	printf("	rating_region_name_length = 0x%X\n", rating_region_name_length);
	printf("	rating_region_name_text = ");
	for ( uint32_t i = 0; i < rating_region_name_length; i++ ) {
		printf(" %c", rating_region_name_text[i]);
	}
	printf("\n");
	printf("	dimensions_defined = 0x%X\n", dimensions_defined);
	for ( int i = 0; i < dimensions_defined; i++ ) {
		mPSIP_RatingDimensionInfoList[i]->dump();
	}
	printf("	descriptors_length = 0x%X\n", descriptors_length);
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
	printf("[ <- PSIP_RatingRegionSection ]\n");
}
