/*******************************************************************//**
 *	@file	hsv2rgb.c
 *
 *	@brief	HSV to RGB implementation(s) …
 *
 *	@author	Alexander Dahl <post@lespocky.de>
 *
 *	@copyright	2013 Alexander Dahl
 *
 *	This file is part of hsv2rgb.
 *
 *	_hsv2rgb_ is free software: you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	_hsv2rgb_ is distributed in the hope that it will be useful, but
 *	WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *	General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with _hsv2rgb_. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#include "hsv2rgb.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const uint8_t buckets_8[6] = { 0, 43, 86, 128, 171, 214 };
const uint16_t buckets_10[6] = { 0, 171, 342, 512, 683, 854 };
uint8_t bsize_8[6];

int16_t f8( uint8_t h ) {
	uint8_t hi = hi8( h );

	return (uint16_t) (h - buckets_8[hi]) * 255 / bsize_8[hi];
}

int16_t hi8( uint8_t h ) {
	if ( h < buckets_8[3] ) {
		if ( h < buckets_8[1] ) {
			return 0;
		} else if ( h < buckets_8[2] ) {
			return 1;
		} else {
			return 2;
		}
	} else {
		if ( h < buckets_8[4] ) {
			return 3;
		} else if ( h < buckets_8[5] ) {
			return 4;
		} else {
			return 5;
		}
	}
}

int16_t hi10( uint16_t h ) {
	return -1;
}

void init_hsv2rgb( void ) {
	uint8_t lpc;
	for ( lpc = 0; lpc < 5; lpc++ ) {
		bsize_8[lpc] = buckets_8[lpc+1] - buckets_8[lpc] - 1;
	}
	bsize_8[5] = 256 - buckets_8[5] - 1;
#ifndef NDEBUG
	(void) printf( "bsize_8:" );
	for ( lpc = 0; lpc < 6; lpc++ ) {
		(void) printf( " %u", bsize_8[lpc] );
	}
	(void) printf( "\n" );
#endif
}

int16_t p8( uint8_t v, uint8_t s ) {
	uint16_t _v = v, _s = s;

//	return (int16_t) ( _v * ( (uint16_t) 255 - _s ) / 255 );
	return (int16_t) ( _v * ( 255 - _s ) / 255 );
}

int16_t q8( uint8_t v, uint8_t s, uint8_t f ) {
	uint16_t _v = v, _s = s, _f = f;

	return (int16_t) (_v * ( 255 - ( ( _s * _f ) / 255 ) ) / 255);
}

void rgb( uint8_t h, uint8_t s, uint8_t v,
		uint8_t *r, uint8_t *g, uint8_t *b )
{
	uint8_t f, hi;
    int16_t p, q, t;

	hi = hi8( h );
    f = f8( h );
    p = p8( v, s );
    q = q8( v, s, f );
    t = t8( v, s, f );

	switch ( hi ) {
	case 0:
		*r = v;
        *g = t;
        *b = p;
        break;
	case 1:
        *r = q;
		*g = v;
        *b = p;
		break;
	case 2:
        *r = p;
		*g = v;
        *b = t;
		break;
	case 3:
        *r = p;
        *g = q;
		*b = v;
		break;
	case 4:
        *r = t;
        *g = p;
		*b = v;
		break;
	case 5:
		*r = v;
        *g = p;
        *b = q;
		break;
	default:
		exit( EXIT_FAILURE );
		break;
	}
}

int16_t t8( uint8_t v, uint8_t s, uint8_t f ) {
	uint16_t _v = v, _s = s, _f = f;

	return (int16_t) (_v * ( 255 - ( ( _s * (255 - _f) ) / 255 ) ) / 255);
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
