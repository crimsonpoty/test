/*
 * PSIP_MultipleStringStructure.h
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#ifndef PSIP_MULTIPLESTRINGSTRUCTURE_H_
#define PSIP_MULTIPLESTRINGSTRUCTURE_H_

#include <vector>
#include <stdint.h>

class PSIP_MultipleString;
class PSIP_MultipleStringSegment;
class PSIP_MultipleStringStructure;

typedef std::vector<PSIP_MultipleString*>			PSIP_MultipleStringList;
typedef std::vector<PSIP_MultipleStringSegment*>	PSIP_MultipleStringSegmentList;


class PSIP_MultipleStringSegment
{
protected:
	PSIP_MultipleStringSegment();
	virtual ~PSIP_MultipleStringSegment();
	friend class PSIP_MultipleString;
	friend class PSIP_MultipleStringStructure;

public:
	uint8_t		getCompressionType() const;
	uint8_t		getMode() const;
	uint8_t*	getCompressedStringBytes() const;

protected:
	uint8_t		compression_type;
	uint8_t		mode;
	uint8_t		number_bytes;
	uint8_t*	compressed_string_bytes;
};


class PSIP_MultipleString
{
protected:
	PSIP_MultipleString();
	virtual ~PSIP_MultipleString();
	friend class PSIP_MultipleStringStructure;

public:
	const uint8_t* getIso639LanguageCode() const;
	const PSIP_MultipleStringSegmentList* getMultipleStringSegments() const;

protected:
	uint8_t	iso_639_language_code[4];
	uint8_t	number_segments;
	PSIP_MultipleStringSegmentList mPSIP_MultipleStringSegmentList;
};


class PSIP_MultipleStringStructure
{
protected:
	PSIP_MultipleStringStructure(const uint8_t * const buffer, size_t length);
	virtual ~PSIP_MultipleStringStructure();

public:
	const PSIP_MultipleStringList* getMultipleStrings() const;
	void dump();

protected:
	uint8_t		number_strings;
	PSIP_MultipleStringList mPSIP_MultipleStringList;
};

#endif /* PSIP_MULTIPLESTRINGSTRUCTURE_H_ */
