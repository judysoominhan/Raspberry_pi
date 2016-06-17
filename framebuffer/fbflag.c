#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FBDEVICE "/dev/fb0"
typedef unsigned char ubyte;

unsigned short makepixel (unsigned char r, unsigned char g, unsigned char b) {
	return(unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));}

static int DrawFaceMMAP(int fd, int start_x, int start_y, int end_x, int end_y, unsigned short color) {
  int x, y, offset;
  struct fb_var_screeninfo vinfo;
  unsigned short *pfb;

  if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo)<0){
	perror("error");
	return -1;
  }
  
  if(end_x==0) end_x=vinfo.xres;
  if(end_y==0) end_y=vinfo.yres;

  pfb = (unsigned short *)mmap(0, vinfo.xres*vinfo.yres*2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  for(x = start_x; x<end_x; x++) {
    for(y=start_y; y<end_y; y++) {
		*(pfb +x+y*vinfo.xres)=color;
	};
  };

  printf("%d", vinfo.xres);
  printf("%d", vinfo.yres);
  munmap(pfb, vinfo.xres*vinfo.yres*2);

  return 0;
}

int main(int argc, char** argv){
  int fbfd, status, offset;
  unsigned short pixel;
  fbfd = open(FBDEVICE, O_RDWR);
  if(fbfd<0){
	perror("error");
	return -1;
  }

  DrawFaceMMAP(fbfd, 0,0,250,0, makepixel(0,0,255));
  DrawFaceMMAP(fbfd, 250,0,500,0, makepixel(255,255,255));
  DrawFaceMMAP(fbfd, 500,0,750,0, makepixel(255,0,0));

  close(fbfd);

  return 0;
}
