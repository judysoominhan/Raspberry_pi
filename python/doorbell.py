#!/usr/bin/env python

from StringIO import StringIO 
from time import sleep
import pycurl, json
import os
import httplib, urllib
import RPi.GPIO as GPIO
import picamera
import datetime

camera = picamera.PiCamera()
filename = datetime.datetime.now().strftime("%Y_%m_%d-%S_%H_%M.jpg")

 
#GPIO.setmode(GPIO.BCM)
#GPIO.setup(4, GPIO.IN)
 
GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.IN)

appID = "573ab57d5659e3e56578d1a4"
appSecret = "fc60e0b932dd0bbbc7a7cd1ebd9d3ca6"
pushEvent = "Door_Alert"
pushMessage = "Ding Dong!! someone is at the door"

buffer = StringIO()
c = pycurl.Curl()
c.setopt(c.URL, 'http://api.instapush.im/post')
c.setopt(c.HTTPHEADER, ['x-instapush-appid: ' + appID, 'x-instapush-appsecret: ' + appSecret, 'Content-Type: application/json'])

json_fields = {}
json_fields['event']=pushEvent
json_fields['trackers'] = {}
json_fields['trackers']['message'] = pushMessage

postfields = json.dumps(json_fields)
c.setopt(c.POSTFIELDS, postfields)
c.setopt(c.WRITEFUNCTION, buffer.write)
c.setopt(c.VERBOSE, True)

#App-specific variables
print 'Doorbell Server Started\r'

try: 
  while True: 
     if (GPIO.input(7) == False):
        c.perform()
        body = buffer.getvalue()
        print(body)

        print 'Button Pushed!\r'
        camera.capture(filename)
        os.system('mpg321 /home/pi/doorbell.mp3')
#       InstaPush('Doorbell','Someone is  at the door!','http://192.168.1.24:8080/stream')
        sleep(0.2);
except KeyboardInterrupt:
  c.close()
  GPIO.cleanup()
