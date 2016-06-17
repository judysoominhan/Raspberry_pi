#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define FBDEVICE "/dev/fb0"

int main(int argc, char** argv) {
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;

  fbfd = open(FBDEVICE, O_RDWR);
  if(fbfd<0) {
	perror("Error: cannot open framebuffer device");
	return -1;
  }
  printf("the Framebuffer device was opened successfully\n");

  if(ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)<0){
	perror("error reading fixed info");
	return -1;
  }

  if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)<0) {
	perror("error reading variable info");
	return -1;
  }

  printf("resolution: %dx%d, %dbpp\n", vinfo.yres, vinfo.bits_per_pixel);
  printf("virtual resolution: %dx%d\n", vinfo.xres_virtual, vinfo.yres_virtual);
  printf("length of frame buffer memory: %d\n", finfo.smem_len);

  vinfo.xres = 800;
  vinfo.yres = 600;
  if(ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)<0){
	perror("fbdev ioctl(PUT)");
	return -1;
  }

  printf("resolution: %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
  close(fbfd);
  return 0;
}
