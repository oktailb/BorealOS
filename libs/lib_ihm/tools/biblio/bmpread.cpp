#define RGB_RESET 0x03C6
#define RGB_READ 0x03C7
#define RGB_WRITE 0x03C8
#define RGB_DATA 0x03C9

#include <dir.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <process.h>
#include <graphics.h>
#include <iostream.h>
#include "svgautil.h"
#include "svgautil.c"
#include "svga256.h"

typedef struct tagBITMAPFILEHEADER
 {
  unsigned int  bfType;
  unsigned long bfSize;
  unsigned int  bfReserved1;
  unsigned int  bfReserved2;
  unsigned long bfOffBits;
 } BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
 {
  unsigned long biSize;
  unsigned long biWidth;
  unsigned long biHeight;
  unsigned int  biPlanes;
  unsigned int  biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  unsigned long biXPelsPerMeter;
  unsigned long biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
 } BITMAPINFOHEADER;

typedef struct tagRGBQUAD
 {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
 } RGBQUAD;

FILE             *ptr;
BITMAPFILEHEADER bmfh;
BITMAPINFOHEADER bmih;
RGBQUAD          rgbq[256];
char files[100][14];
int height;


main()
 {
  FILE *p;
  unsigned char tmp1, tmp2;
  int i, j, k, flag, no_files;          // flag checks if color table is of max. size.
  char filename[14], ch;
  char *path, drive[2], dir[100], file[10], ext[4];

// start reading the file.

  ptr = fopen(filename, "rb");
  flag = 0;

  fread(&bmfh, sizeof(bmfh), 1, ptr);  // read bitmap file header.
  fread(&bmih, sizeof(bmih), 1, ptr);  // read bitmap information header.

  if (bmfh.bfType != 19778)           // BMP signature.
   {
    cprintf("\r\nบ   %s is not a BMP file!", filename);
    gotoxy(3, 23);
    cprintf("Esc - exit program; Any other key - continue");
    tmp1 = getch();
    if (tmp1 == 27)
     {
      clrscr();
      exit(0);
     }
     else
      goto start;
   }
  cprintf("\r\nบ   Image information\r\n");
  cprintf("บ   อออออออออออออออออ\r\n\r\n");
  cout << "บ   File size   : " << bmfh.bfSize << " bytes.\n";
  cout << "บ   Image size  : " << bmih.biWidth << " X " << bmih.biHeight << " pixels.\n";
  cout.setf(ios::fixed, ios::floatfield);
  cout << "บ   Colors      : " << pow(2, bmih.biBitCount) << " (" << bmih.biBitCount << " bits per pixel).\n";
  if (bmih.biBitCount > 8)
   {
    cprintf("\r\nบ   Cannot read 24-bit BMP!");
    gotoxy(3, 23);
    cprintf("Esc - exit program; Any other key - continue          ");
    tmp1 = getch();
    if (tmp1 == 27)
     {
      clrscr();
      exit(0);
     }
     else
      goto start;
   }

  cprintf("บ   Compression : ");
  if (bmih.biCompression == 0) cprintf("None.\r\n\r\n");
   else cprintf("Run Length Encoded.\r\n\r\n");

  if (bmih.biClrUsed == 0) bmih.biClrUsed = pow(2, bmih.biBitCount);
  if (bmih.biClrUsed > 254) flag = 1;

  rgbq[0].rgbRed = rgbq[0].rgbBlue = rgbq[0].rgbGreen = 0;  // set first entry in palette to black for black background.
  for (i = 1 - flag; i < bmih.biClrUsed + 1 - flag; i++)    // storing the color table.
   {
    rgbq[i].rgbBlue = getc(ptr);
    rgbq[i].rgbGreen = getc(ptr);
    rgbq[i].rgbRed = getc(ptr);
    tmp1 = getc(ptr);
   }
  for (i = bmih.biClrUsed + 1 - flag; i < 255 + flag; i++)    // initialising unused entries.
   rgbq[i].rgbRed = rgbq[i].rgbBlue = rgbq[i].rgbGreen = 0;
  if (!flag) rgbq[255].rgbRed = rgbq[255].rgbBlue = rgbq[255].rgbGreen = 255; // set last entry in palette to white for white foreground.


  for (i = 0; i < 256; i++)
   setPalette(i, rgbq[i].rgbRed / 4, rgbq[i].rgbGreen / 4, rgbq[i].rgbBlue / 4);
  clearviewport();




  i = (getmaxy() + bmih.biHeight) / 2;        // start display of image.
  j = (getmaxx() - bmih.biWidth) / 2;



  height = 0;                           // height keeps track of how many lines have been drawn.
  while (!feof(ptr))
   {
    tmp1 = getc(ptr);               // read character from file.
    switch(bmih.biBitCount)
     {
      case 8 : if (height == bmih.biHeight) break;
	       if (bmih.biCompression == 1)       // for 8-bit BMP.
		{                                 // (RLE encoded).
		 tmp2 = getc(ptr);
		 if (tmp1 == 0)
		  {

		   switch(tmp2)
		    {
		     case 0  : i--;               // end of line.
			       j = (getmaxx() - bmih.biWidth) / 2;
			       break;
		     case 1  : goto finish;       // end of BMP.
			       break;
		     case 2  : j += getc(ptr);    // offset of next pixel.
			       i -= getc(ptr);
			       break;
		     default : for (k = 0; k < tmp2; k++)
				putPixel(i, j, getc(ptr), flag);
			       if ((tmp2 % 2) != 0) getc(ptr);
		    }
		  }
		  else
		   {
		    for (k = 0; k < tmp1; k++)
		     putPixel(i, j, tmp2, flag);
		   }
		}
		else                              // not encoded.
		 {
		  tmp2 = tmp1;
		  putPixel(i, j, tmp2, flag);
		 }
	       break;
      case 4 : if (height == bmih.biHeight) break;   // for 4-bit BMP.
	       tmp2 = tmp1 >> 4;
	       putPixel(i, j, tmp2, flag);
	       if (height == bmih.biHeight) break;
	       tmp2 = tmp1 << 4;
	       tmp2 >>= 4;
	       putPixel(i, j, tmp2, flag);
	       break;
      case 1 : for (k = 0; k < 8; k++)       // for 1-bit BMP.
		{
		 if (height == bmih.biHeight) break;     // if BMP ends in middle of byte.
		 tmp2 = tmp1 & 128;
		 tmp1 <<= 1;
		 if (tmp2 == 0) ;
		  else tmp2 = 1;
		 putPixel(i, j, tmp2, flag);
		}
	       break;
     }
   }
 }
