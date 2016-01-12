/*
 * PSIP_ContentAdvisoryDescriptor.h
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#ifndef PSIP_CONTENTADVISORYDESCRIPTOR_H_
#define PSIP_CONTENTADVISORYDESCRIPTOR_H_

#include "PSIP_Descriptor.h"

class PSIP_ContentRatingRegionInfo;
class PSIP_ContentRatingDimensionInfo;

typedef std::vector<PSIP_ContentRatingRegionInfo*>		PSIP_ContentRatingRegionInfoList;
typedef std::vector<PSIP_ContentRatingDimensionInfo*>	PSIP_ContentRatingDimensionInfoList;


class PSIP_ContentRatingDimensionInfo
{
protected:
	PSIP_ContentRatingDimensionInfo();
	virtual ~PSIP_ContentRatingDimensionInfo();
	friend class PSIP_ContentAdvisoryDescriptor;
	friend class PSIP_ContentRatingRegionInfo;

public:
	uint8_t		getRatingDimensionJ() const;
	uint8_t		getRatingValue() const;

protected:
	uint8_t		rating_dimension_j;
	uint8_t		:4;
	uint8_t		rating_value:4;
};


class PSIP_ContentRatingRegionInfo
{
protected:
	PSIP_ContentRatingRegionInfo();
	virtual ~PSIP_ContentRatingRegionInfo();
	friend class PSIP_ContentAdvisoryDescriptor;

public:
	uint8_t		getRatingRegion() const;
	const PSIP_ContentRatingDimensionInfoList* getPSIP_ContentRatingDimensionInfo() const;
	uint8_t		getRatingDescriptionLength() const;
	const uint8_t* getRatingDescriptionText() const;
	void 		dump();

protected:
	uint8_t		rating_region;
	uint8_t		rated_dimensions;
	uint8_t		rating_description_length;
	uint8_t*	rating_description_text;

protected:
	PSIP_ContentRatingDimensionInfoList mPSIP_ContentRatingDimensionInfoList;
};


class PSIP_ContentAdvisoryDescriptor : public PSIP_Descriptor
{
protected:
	PSIP_ContentAdvisoryDescriptor(const uint8_t * const buffer);
	virtual ~PSIP_ContentAdvisoryDescriptor();
	friend class PSIP_DescriptorContainer;

public:
	const PSIP_ContentRatingRegionInfoList* getPSIP_ContentRatingRegionInfo() const;
	virtual void dump();

public:
	uint8_t		:2;
	uint8_t		rating_region_count:6;

protected:
	PSIP_ContentRatingRegionInfoList mPSIP_ContentRatingRegionInfoList;
};

#endif /* PSIP_CONTENTADVISORYDESCRIPTOR_H_ */
