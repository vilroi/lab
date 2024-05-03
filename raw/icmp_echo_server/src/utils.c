#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "error.h"

static bool is_odd(int);

/*
 * Calculates the checksum.
 * This function assumes that the checksum field of the respective
 * headers are cleared prior to being called.
 */
uint16_t 
calc_checksum(uint8_t *buf, size_t len)
{
	uint32_t		tmp = 0;
	uint16_t		*p, checksum;

	if (buf == NULL)
		ERROR_EXIT("null pointer supplied for buf");

	if (is_odd(len)) {
		len--;
		tmp = (buf[len] << 8);
	}

	p = (uint16_t *) buf;
	for (int i = 0; i < len / 2; i++, p++) 
		tmp += *p;

	checksum = (tmp & 0xffff) + (tmp >> 16);

	return ~checksum;
}


uint16_t 
genid()
{
	uint16_t	val;

	srand(time(NULL));
	val = rand() % 65535;
	return htons(val);
}

static bool 
is_odd(int x)
{
	return ((x % 2) != 0);
}
