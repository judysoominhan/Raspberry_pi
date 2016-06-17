#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define FBDEVICE "/dev/fb0"
typedef unsigned char ubyte;

unsigned short makepixel (unsigned char r, unsigned char g, unsigned char b) {
	return(unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));}

static int DrawFace(int fd, int start_x, int start_y, int end_x, int end_y, unsigned short color) {
  int x, y, offset;
  struct fb_var_screeninfo vinfo;

  if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo)<0){
	perror("error");
	return -1;
  }
  if(end_x==0) end_x=vinfo.xres;
  if(end_y==0) end_y=vinfo.yres;

  for(x = start_x; x<end_x; x++) {
	for(y=start_y; y<end_y; y++) {
	  offset=(x+y*vinfo.xres)*2;
	  lseek(fd, offset, SEEK_SET);
	  write(fd,&color,2);
	};
  };

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

  DrawFace(fbfd, 0,0,0,0, makepixel(255,255,0));

  close(fbfd);

  return 0;
}
