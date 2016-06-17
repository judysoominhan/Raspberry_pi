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

static int DrawPoint(int fd, int x, int y, unsigned short color) {
  struct fb_var_screeninfo vinfo;

  if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) <0){
	perror("error");
	return -1;
  }

  int offset = (x+y*vinfo.xres)*2;
  lseek(fd, offset, SEEK_SET);
  write(fd, &color, 2);
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

  DrawPoint(fbfd, 50, 50, makepixel(255, 0,0));
  DrawPoint(fbfd, 100, 100, makepixel(0, 255, 0));
  DrawPoint(fbfd, 150, 150, makepixel(0, 0,255));


//DrawLine(fbfd, 0, 100, 200, makepixel(0,255,255));

  close(fbfd);

  return 0;
}

DrawLine(fbfd, 0, 100, 200, makepixel(0,255,255));

static int DrawLine(int fd, int start_x, int end_x, int y, unsigned short color){
  int x, offset;
  struct fb_var_screeninfo vinfo;

  if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) <0){
	perror("error");
	return -1;
  }

  for (x=start_x; x<end_x; x++) {
	offset=(x+y*vinfo.xres)*2;
	lsek(fd, offset, SEEK_SET);
	write(fd, &color, 2);
  };

  return 0;
}
