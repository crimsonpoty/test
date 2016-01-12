/*
 * PSIP_ContentAdvisoryDescriptor.cpp
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#include <assert.h>
#include "PSIP_ContentAdvisoryDescriptor.h"

PSIP_ContentRatingDimensionInfo::PSIP_ContentRatingDimensionInfo()
 : rating_dimension_j(0)
 , rating_value(0)
{

}

PSIP_ContentRatingDimensionInfo::~PSIP_ContentRatingDimensionInfo()
{

}

uint8_t PSIP_ContentRatingDimensionInfo::getRatingDimensionJ() const
{
	return rating_dimension_j;
}

uint8_t PSIP_ContentRatingDimensionInfo::getRatingValue() const
{
	return rating_value;
}

PSIP_ContentRatingRegionInfo::PSIP_ContentRatingRegionInfo()
 : rating_region(0)
 , rated_dimensions(0)
 , rating_description_length(0)
 , rating_description_text(NULL)
{

}

PSIP_ContentRatingRegionInfo::~PSIP_ContentRatingRegionInfo()
{
	if ( rating_description_text ) {
		delete[] rating_description_text;
		rating_description_text = NULL;
	}

	PSIP_ContentRatingDimensionInfoList::iterator it;
	for ( it = mPSIP_ContentRatingDimensionInfoList.begin(); it != mPSIP_ContentRatingDimensionInfoList.end(); ++it ) {
		delete *it;
	}
}

uint8_t	PSIP_ContentRatingRegionInfo::getRatingRegion() const
{
	return rating_region;
}

const PSIP_ContentRatingDimensionInfoList* PSIP_ContentRatingRegionInfo::getPSIP_ContentRatingDimensionInfo() const
{
	return &mPSIP_ContentRatingDimensionInfoList;
}

uint8_t	PSIP_ContentRatingRegionInfo::getRatingDescriptionLength() const
{
	return rating_description_length;
}

const uint8_t* PSIP_ContentRatingRegionInfo::getRatingDescriptionText() const
{
	return rating_description_text;
}

void PSIP_ContentRatingRegionInfo::dump()
{
	printf("			[ -> PSIP_ContentRatingRegionInfo ]\n");
	printf("				rating_region = 0x%X\n", rating_region);
	printf("				rated_dimensions = 0x%X\n", rated_dimensions);
	for ( int i = 0; i < rated_dimensions; i++ ) {
		printf("					rating_dimension_j = 0x%X\n", mPSIP_ContentRatingDimensionInfoList[i]->rating_dimension_j);
		printf("					rating_value = 0x%X\n", mPSIP_ContentRatingDimensionInfoList[i]->rating_value);
	}
	printf("				rating_description_length = 0x%X\n", rating_description_length);
	printf("				rating_description_text = ");
	for ( uint32_t i = 0; i < rating_description_length; i++ ) {
		printf(" %c", rating_description_text[i]);
	}
	printf("\n");
	printf("			[ <- PSIP_ContentRatingRegionInfo ]\n");
}


PSIP_ContentAdvisoryDescriptor::PSIP_ContentAdvisoryDescriptor(const uint8_t * const buffer)
 : PSIP_Descriptor(buffer)
 , rating_region_count(0)
{
	if ( 2 < descriptorLength ) {
		int index = 2;
		rating_region_count = buffer[index++] & 0x3F;
		for ( uint8_t i = 0; i < rating_region_count; i++ ) {
			PSIP_ContentRatingRegionInfo* _info = new PSIP_ContentRatingRegionInfo;
			assert(NULL != _info);
			_info->rating_region = buffer[index++] & 0xFF;
			_info->rated_dimensions = buffer[index++] & 0xFF;
			if ( 0 < _info->rated_dimensions ) {
				for ( uint8_t j = 0; j < _info->rated_dimensions; j++ ) {
					PSIP_ContentRatingDimensionInfo* _d = new PSIP_ContentRatingDimensionInfo;
					assert(NULL != _d);
					_d->rating_dimension_j = buffer[index++] & 0xFF;
					_d->rating_value = buffer[index++] & 0x0F;
					_info->mPSIP_ContentRatingDimensionInfoList.push_back(_d);
				}
			}
			mPSIP_ContentRatingRegionInfoList.push_back(_info);
		}
	}
}

PSIP_ContentAdvisoryDescriptor::~PSIP_ContentAdvisoryDescriptor()
{
	PSIP_ContentRatingRegionInfoList::iterator it;
	for ( it = mPSIP_ContentRatingRegionInfoList.begin(); it != mPSIP_ContentRatingRegionInfoList.end(); ++it ) {
		delete *it;
	}
}

const PSIP_ContentRatingRegionInfoList* PSIP_ContentAdvisoryDescriptor::getPSIP_ContentRatingRegionInfo() const
{
	return &mPSIP_ContentRatingRegionInfoList;
}

void PSIP_ContentAdvisoryDescriptor::dump()
{
	printf("		[ -> PSIP_ContentAdvisoryDescriptor ]\n");
	printf("			rating_region_count = 0x%X\n", rating_region_count);
	for ( int i = 0; i < rating_region_count; i++ ) {
		mPSIP_ContentRatingRegionInfoList[i]->dump();
	}
	printf("		[ <- PSIP_ContentAdvisoryDescriptor ]\n");
}
