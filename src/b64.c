#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "b64.h"

static const unsigned char INDEX_TABLE[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
	'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', '+', '/'
};
static const unsigned char PADDING = '=';

unsigned char indexOf(const unsigned char encodedChar) {
	for (unsigned char index = 0; index < sizeof(INDEX_TABLE); index++) {
		if (INDEX_TABLE[index] == encodedChar) {
			return index;
		}
	}
	return PADDING;
}

unsigned char* b64_encode(const unsigned char const* string) {
	if (string == NULL) {
		return NULL;
	}
	const unsigned int lenght = strlen(string);
	const unsigned int encodedLenght = (lenght / 3 + (lenght % 3 > 0)) * 4 + 1;
	unsigned char* b64String = malloc(encodedLenght);
	if (b64String == NULL) {
		return NULL;
	}
	for (unsigned int index = 0; index < lenght; index += 3) {
		const unsigned char decodedBytes[3] = {
			string[index],
			index + 1 < lenght ? string[index + 1] : 0,
			index + 2 < lenght ? string[index + 2] : 0
		};
		const unsigned int encodeOffset = index / 3;
		unsigned char* sequenceOffset = b64String + index + encodeOffset;
		*(sequenceOffset) = INDEX_TABLE[decodedBytes[0] >> 2];
		*(sequenceOffset + 1) = INDEX_TABLE[(decodedBytes[0] & 3) << 4 | decodedBytes[1] >> 4];
		*(sequenceOffset + 2) = decodedBytes[1] != 0 ? INDEX_TABLE[(decodedBytes[1] & 15) << 2 | decodedBytes[2] >> 6] : PADDING;
		*(sequenceOffset + 3) = decodedBytes[2] != 0 ? INDEX_TABLE[decodedBytes[2] & 63] : PADDING;
	}
	b64String[encodedLenght - 1] = '\0';
	return b64String;
}

unsigned char* b64_decode(const unsigned char const* b64String) {
	if (b64String == NULL) {
		return NULL;
	}
	const unsigned int encodedLenght = strlen(b64String);
	unsigned int paddingCount;
	if (encodedLenght > 1 && b64String[encodedLenght - 2] == PADDING) {
		paddingCount = 2;
	} else if (encodedLenght > 0 && b64String[encodedLenght - 1] == PADDING) {
		paddingCount = 1;
	} else {
		paddingCount = 0;
	}
	const unsigned int lenght = (encodedLenght / 4 + (encodedLenght % 4 > 0)) * 3 + 1 - paddingCount;
	unsigned char* string = malloc(lenght);
	if (string == NULL) {
		return NULL;
	}
	for (unsigned int index = 0; index < encodedLenght; index += 4) {
		const unsigned char encodedBytes[4] = {
			indexOf(b64String[index]),
			indexOf(b64String[index + 1]),
			index + 2 < encodedLenght ? indexOf(b64String[index + 2]) : PADDING,
			index + 3 < encodedLenght ? indexOf(b64String[index + 3]) : PADDING
		};
		const unsigned int encodeOffset = index / 4;
		unsigned char* sequenceOffset = string + index - encodeOffset;
		*(sequenceOffset) = encodedBytes[0] << 2 | ((encodedBytes[1] >> 4) & 3);
		*(sequenceOffset + 1) = encodedBytes[1] << 4 | (encodedBytes[2] != PADDING ? encodedBytes[2] >> 2 : 0);
		*(sequenceOffset + 2) = encodedBytes[2] << 6 | (encodedBytes[3] & 63);
	}
	string[lenght - 1] = '\0';
	return string;
}
