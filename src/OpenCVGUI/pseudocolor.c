/*
 * pseudocolor.c
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

#include "pseudocolor.h"
#include <stdio.h>

#define PI 3.14159265

void generate_pseudocolorTable(Pseudocolor *psdata)
{
	int i;

	for(i=0;i<255;i++){
		unsigned char *data=psdata->table[i];
		float gray=(float)i/255.0;
		data[2]=(int)(255*fabs(sin(gray*psdata->p[2]*PI+psdata->t[2]*PI)));
		data[1]=(int)(255*fabs(sin(gray*psdata->p[1]*PI+psdata->t[1]*PI)));
		data[0]=(int)(255*fabs(sin(gray*psdata->p[0]*PI+psdata->t[0]*PI)));
	}
	
}

void pseudocolor(IplImage *src, IplImage *dst, Pseudocolor *psdata)
{
	uchar *data;
	int x,y;

	for(x=0; x<src->width; x++)
	{
		for(y=0;y<src->height; y++)
		{
			data=&((uchar*)(dst->imageData + dst->widthStep*y ))[x*3];
			uchar gray=((uchar*)(src->imageData + src->widthStep*y))[x];
			data[2]=psdata->table[gray][2];
			data[1]=psdata->table[gray][1];
			data[0]=psdata->table[gray][0];
		}
	}
}
/*
void save_pseudocolor(char* file, Pseudocolor *psdata)
{
	CvFileStorage* storage = cvOpenFileStorage( file, 0, CV_STORAGE_WRITE_TEXT );
	if(storage==NULL){
		fprintf(stderr, "Error open file %s to write \n",file);
		return;
	}	
	cvStartWriteStruct(storage, "p", CV_NODE_SEQ, NULL, cvAttrList(0,0));
	cvWriteRawData(storage, psdata->p,3,"f");
	cvEndWriteStruct(storage);
	
	cvStartWriteStruct(storage, "t", CV_NODE_SEQ, NULL, cvAttrList(0,0));
	cvWriteRawData(storage, psdata->t,3,"f");
	cvEndWriteStruct(storage);
	
	cvStartWriteStruct(storage, "table", CV_NODE_SEQ, NULL, cvAttrList(0,0));
	cvWriteRawData(storage, psdata->table,255,"u");
	cvEndWriteStruct(storage);
	
	cvReleaseFileStorage( &storage );
}

void read_pseudocolor(char* file, Pseudocolor *psdata)
{
	CvFileStorage* storage = cvOpenFileStorage( file, 0, CV_STORAGE_READ );
	if(storage==NULL){
		fprintf(stderr, "Error reading file %s\n",file);
		return;
	}	
	CvFileNode* p_node = cvGetFileNodeByName( storage, NULL, "p" );
	CvFileNode* t_node = cvGetFileNodeByName( storage, NULL, "t" );
	CvFileNode* table_node = cvGetFileNodeByName( storage, NULL, "table" );
	
	cvReadRawData(storage, p_node, psdata->p,"f");
	cvReadRawData(storage, t_node, psdata->t,"f");
	cvReadRawData(storage, table_node, psdata->table,"u");
	cvReleaseFileStorage( &storage );
}
*/