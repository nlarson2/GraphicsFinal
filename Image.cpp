#ifndef IMAGE_CPP
#define IMAGE_CPP

#include "Image.h"
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

Image::~Image() { delete [] data; }
Image::Image(const char *fname) {
    if (fname[0] == '\0')
        return;
    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
        ppmFlag = 1;
    if (ppmFlag) {
        strcpy(ppmname, name);
    } else {
        name[slen-4] = '\0';
        sprintf(ppmname,"%s.ppm", name);
        char ts[100];
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
    }

    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);
        //skip comments and blank lines
        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);
        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);
        //get pixel data
        int n = width * height * 3;
        data = new unsigned char[n];

        for (int i=0; i<n; i++){
            data[i] = fgetc(fpi);
        }
        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n",ppmname);
        exit(0);
    }
    if (!ppmFlag)
        unlink(ppmname);

}
void Image::invertY()
{
	unsigned char flip[height][width*3];
	int iter = 0, i, j;
	for ( i = 0 ; i < height ; i++ ) {
		for ( j = 0 ; j < width * 3 ; j++ ) {
			flip[i][j] = data[iter++];
		}
	}

	iter = 0;
	for ( i = height - 1 ; i >= 0 ; i-- ) {
		for ( j = 0 ; j < width * 3 ; j++ ) {
			data[iter++] = flip[i][j];
		}
	}
}



#endif