/*
 * PSIP_MultipleStringStructure.cpp
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "PSIP_MultipleStringStructure.h"

// PSIP_MultipleStringSegment
PSIP_MultipleStringSegment::PSIP_MultipleStringSegment()
 : compression_type(0)
 , mode(0)
 , number_bytes(0)
 , compressed_string_bytes(NULL)
{

}

PSIP_MultipleStringSegment::~PSIP_MultipleStringSegment()
{
	if ( compressed_string_bytes ) {
		delete[] compressed_string_bytes;
		compressed_string_bytes = NULL;
	}
}

uint8_t	PSIP_MultipleStringSegment::getCompressionType() const
{
	return compression_type;
}

uint8_t	PSIP_MultipleStringSegment::getMode() const
{
	return mode;
}

uint8_t* PSIP_MultipleStringSegment::getCompressedStringBytes() const
{
	return compressed_string_bytes;
}

// PSIP_MultipleString
PSIP_MultipleString::PSIP_MultipleString()
 : number_segments(0)
{
	memset(&iso_639_language_code[0], 0x00, 4);
}

PSIP_MultipleString::~PSIP_MultipleString()
{
	PSIP_MultipleStringSegmentList::iterator it;
	for ( it = mPSIP_MultipleStringSegmentList.begin(); it != mPSIP_MultipleStringSegmentList.end(); ++it ) {
		delete (*it);
		(*it) = NULL;
	}
}

const uint8_t* PSIP_MultipleString::getIso639LanguageCode() const
{
	return &iso_639_language_code[0];
}

const PSIP_MultipleStringSegmentList* PSIP_MultipleString::getMultipleStringSegments() const
{
	return &mPSIP_MultipleStringSegmentList;
}

// PSIP_MultipleStringStructure
PSIP_MultipleStringStructure::PSIP_MultipleStringStructure(const uint8_t * const buffer, size_t length)
{
	if ( buffer && 0 < length ) {
		int index = 0;
		number_strings = buffer[index++] & 0xFF;
		for ( int i = 0; i < number_strings; i++ ) {
			PSIP_MultipleString* _string = new PSIP_MultipleString;
			assert(NULL != _string);
			_string->iso_639_language_code[0] = buffer[index++] & 0xFF;
			_string->iso_639_language_code[1] = buffer[index++] & 0xFF;
			_string->iso_639_language_code[2] = buffer[index++] & 0xFF;
			_string->number_segments = buffer[index++] & 0xFF;
			for ( int j = 0; j < (int) _string->number_segments; j++ ) {
				PSIP_MultipleStringSegment* _segment = new PSIP_MultipleStringSegment;
				assert(NULL != _segment);
				_segment->compression_type = buffer[index++] & 0xFF;
				_segment->mode = buffer[index++] & 0xFF;
				_segment->number_bytes = buffer[index++] & 0xFF;
				if ( 0 < _segment->number_bytes ) {
					_segment->compressed_string_bytes = new uint8_t[_segment->number_bytes];
					assert(NULL != _segment->compressed_string_bytes);
				}

				for ( int k = 0; k < _segment->number_bytes; k++ ) {
					_segment->compressed_string_bytes[k] = buffer[index++] & 0xFF;
				}

				_string->mPSIP_MultipleStringSegmentList.push_back(_segment);
			}
			mPSIP_MultipleStringList.push_back(_string);
		}
	}
}

PSIP_MultipleStringStructure::~PSIP_MultipleStringStructure()
{
	PSIP_MultipleStringList::iterator it;
	for ( it = mPSIP_MultipleStringList.begin(); it != mPSIP_MultipleStringList.end(); ++it ) {
		delete (*it);
		(*it) = NULL;
	}
}

const PSIP_MultipleStringList* PSIP_MultipleStringStructure::getMultipleStrings() const
{
	return &mPSIP_MultipleStringList;
}

void PSIP_MultipleStringStructure::dump()
{
	printf("			[ -> PSIP_MultipleStringStructure ]\n");
	for ( int i = 0; i < number_strings; i++ ) {
		printf("				iso_639_language_code = %s\n", \
				mPSIP_MultipleStringList[i]->iso_639_language_code);
		for ( int j = 0; j < (int) mPSIP_MultipleStringList[i]->number_segments; j++ ) {
			PSIP_MultipleStringSegment* segment = mPSIP_MultipleStringList[i]->mPSIP_MultipleStringSegmentList[j];
			if ( segment ) {
				printf("				compression_type = 0x%X\n", segment->compression_type);
				printf("				mode = 0x%X\n", segment->mode);
				printf("				number_bytes = 0x%X\n", segment->number_bytes);
				printf("				compressed_string_bytes =");
				for ( int k = 0; k < (int) segment->number_bytes; k++ ) {
					printf(" 0x%X", segment->compressed_string_bytes[k]);
				}
				printf("\n");
			}
		}
	}

	printf("			[ <- PSIP_MultipleStringStructure ]\n");
}
