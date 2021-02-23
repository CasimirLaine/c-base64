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

unsigned char indexOf(unsigned char encodedChar) {
	for (unsigned char index = 0; index < sizeof(INDEX_TABLE); index++) {
		if (INDEX_TABLE[index] == encodedChar) {
			return index;
		}
	}
	return PADDING;
}

unsigned char* b64_encode(const unsigned char* string) {
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
		const unsigned int encodeOffset = index / 3;
		const unsigned char first = string[index];
		const unsigned char second = index + 1 < lenght ? string[index + 1] : 0;
		const unsigned char third = index + 2 < lenght ? string[index + 2] : 0;
		*(b64String + index + encodeOffset) = INDEX_TABLE[first >> 2];
		*(b64String + index + 1 + encodeOffset) = INDEX_TABLE[(first & 3) << 4 | second >> 4];
		*(b64String + index + 2 + encodeOffset) = second != 0 ? INDEX_TABLE[(second & 15) << 2 | third >> 6] : PADDING;
		*(b64String + index + 3 + encodeOffset) = third != 0 ? INDEX_TABLE[third & 63] : PADDING;
	}
	b64String[encodedLenght - 1] = '\0';
	return b64String;
}

unsigned char* b64_decode(const unsigned char* b64String) {
	if (b64String == NULL) {
		return NULL;
	}
	const unsigned int encodedLenght = strlen(b64String);
	unsigned int lenght = (encodedLenght / 4 + (encodedLenght % 4 > 0)) * 3 + 1;
	unsigned char* string = malloc(lenght);
	if (string == NULL) {
		return NULL;
	}
	unsigned int negativeOffset = 0;;
	for (unsigned int index = 0; index < encodedLenght; index += 4) {
		const unsigned int encodeOffset = index / 4;
		const unsigned char first = indexOf(b64String[index]);
		const unsigned char second = indexOf(b64String[index + 1]);
		const unsigned char third = index + 2 < encodedLenght ? indexOf(b64String[index + 2]) : PADDING;
		const unsigned char fourth = index + 3 < encodedLenght ? indexOf(b64String[index + 3]) : PADDING;
		if (third == PADDING) {
			negativeOffset = 2;
		} else if (fourth == PADDING) {
			negativeOffset = 1;
		}
		*(string + index - encodeOffset) = first << 2 | ((second >> 4) & 3);
		*(string + index + 1 - encodeOffset) = second << 4 | (third != PADDING ? third >> 2 : 0);
		*(string + index + 2 - encodeOffset) = third << 6 | (fourth & 63);
	}
	if (negativeOffset > 0) {
		lenght = lenght - negativeOffset;
		string = realloc(string, lenght);
	}
	if (string != NULL) {
		string[lenght - 1] = '\0';
	}
	return string;
}
