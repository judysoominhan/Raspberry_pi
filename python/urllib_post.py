import urllib.request as req
from urllib.request import urlopen
from urllib.parse import urlencode

url = "http://192.168.1.24:3000/api/sensors"

name = "DHT 11 sensor"
stype = "DHT11"
switch = "on"
temp = "20C"
humidity = "10%"

params = urlencode({
  'title': name,
  'type': stype
  'switch': switch
  'temp': temp
  'humidity':humidity
  })

data = urlopen(url, params.encode('utf8')).read()
