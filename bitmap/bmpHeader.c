#include <stdio.h>
#include "bmpHeader.h"

int readBmp(char *filename, unsigned char **data, int *cols, int *rows) {

  BITMAPFILEHEADER bmpHeader;
  BITMAPINFOHEADER bmpInfoHeader;
  unsigned char ID[2];
  FILE *fp;

  fp=fopen(filename, "rb");
  if(fp==NULL) {
	printf("error\n");
	return -1;
  }

  fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1,fp);
  fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER),1,fp);
  
  if(bmpInfoHeader.biBitCount!=24) {
	printf("This image file doesn't supprt 24 bit color \n");
	return -1;
  }

  *cols=bmpInfoHeader.biWidth;
  *rows=bmpInfoHeader.biHeight;

  printf("resolution: %d*%d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
  printf("bit count: %d\n", bmpInfoHeader.biBitCount);

  fseek(fp, bmpHeader.bfOffBits, SEEK_SET);
  fread(*data, 1, bmpHeader.bfSize-bmpHeader.bfOffBits, fp);

  fclose(fp);
  return 0;
}
