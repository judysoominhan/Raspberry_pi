from urllib import urlopen
import RPi.GPIO as GPIO
import time
import json



url = "http://192.168.1.24:3000/api/sensors"


GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.cleanup()


instance = dht.DHT11(pin=10)

while True:
  result = instance.read()

  if result.is_valid():
    temp = result.temperature
    humidity = result.humidity
    name = "DHT11 sensor"
    stype = "DHT11"
    switch = "on"

    params = urlencode({
      'title':name,
      'type':stype,
      'switch':switch,
      'temp':temp,
      'humidity': humidity
    })

    data = urlopen(url, params.encode()).read()

    print("Temperature: %d C" % temp)
    print("Humidity: %d %%" %humidity)
  else:
    print("Error: %d" % result.error_code)

  time.sleep(3)
