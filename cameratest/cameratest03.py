import time
import picamera

with picamera.PiCamera() as camera:
    camera.vflip=True
    camera.start_preview()
    time.sleep(5)
    camera.capture('/home/pi/image.jpg')
    camera.stop_preview()
