/*
 * PSIP_RatingRegionSection.h
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#ifndef PSIP_RATINGREGIONSECTION_H_
#define PSIP_RATINGREGIONSECTION_H_

#include "long_crc_section.h"
#include "PSIP_DescriptorContainer.h"

class PSIP_RatingDimensionInfo;
class PSIP_RatingDimensionValue;

typedef std::vector<PSIP_RatingDimensionInfo*> 	PSIP_RatingDimensionInfoList;
typedef std::vector<PSIP_RatingDimensionValue*>	PSIP_RatingDimensionValueList;


class PSIP_RatingDimensionValue
{
protected:
	PSIP_RatingDimensionValue();
	virtual ~PSIP_RatingDimensionValue();
	friend class PSIP_RatingDimensionInfo;

public:
	uint8_t		getAbbrevRatingValueLength() const;
	const uint8_t* getAbbrevRatingValueText() const;
	uint8_t		getRatingValueLength() const;
	const uint8_t* getRatingValueText() const;
	void 		dump();

protected:
	uint8_t		abbrev_rating_value_length;
	uint8_t*	abbrev_rating_value_text;
	uint8_t		rating_value_length;
	uint8_t*	rating_value_text;
};


class PSIP_RatingDimensionInfo
{
protected:
	PSIP_RatingDimensionInfo(const uint8_t* const buffer);
	virtual ~PSIP_RatingDimensionInfo();
	friend class PSIP_RatingRegionSection;

public:
	uint8_t		getDimensionNameLength() const;
	const uint8_t* getDimensionNameText() const;
	const PSIP_RatingDimensionValueList* getPSIP_RatingDimensionValue() const;
	void dump();

protected:
	uint8_t		dimension_name_length;
	uint8_t*	dimension_name_text;
	uint8_t		:3;
	uint8_t		granduated_scale:1;
	uint8_t		values_defined:4;

protected:
	uint32_t	dimension_info_total_length;

protected:
	PSIP_RatingDimensionValueList mPSIP_RatingDimensionValueList;
};


class PSIP_RatingRegionSection : public LongCrcSection, public PSIP_DescriptorContainer
{
public:
	PSIP_RatingRegionSection(const uint8_t * const buffer);
	virtual ~PSIP_RatingRegionSection();

public:
	static const int PID = 0x1FFB;
	static const int TID = 0xCA;

public:
	uint8_t		getRaingRegion() const;
	uint8_t		getRaingRegionNameLength() const;
	const uint8_t* getRaingRegionNameText() const;
	const PSIP_RatingDimensionInfoList* getRatingDimensionInfo() const;
	void 		dump();

protected:
	uint8_t 	protocol_version;
	uint8_t 	rating_region_name_length;
	uint8_t* 	rating_region_name_text;
	uint8_t 	dimensions_defined;
	uint16_t 	:6;
	uint16_t	descriptors_length:10;

protected:
	PSIP_RatingDimensionInfoList mPSIP_RatingDimensionInfoList;
};

#endif /* PSIP_RATINGREGIONSECTION_H_ */
