/*
 * pseudocolor.h
 * Copyright (C) David Millán Escrivá 2010 <david@artresnet.com>
 * 
 * pseudocolor is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * pseudocolor is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "cv.h"
#include "highgui.h"

typedef struct
{
	float p[3];
	float t[3];
	unsigned char table[255][3];
}Pseudocolor;

void generate_pseudocolorTable(Pseudocolor *psdata);
void pseudocolor(IplImage *src, IplImage *dst, Pseudocolor *psdata);
//void save_pseudocolor(char* file, Pseudocolor *psdata);
//void read_pseudocolor(char* file, Pseudocolor *psdata);

#ifdef __cplusplus
}
#endif
