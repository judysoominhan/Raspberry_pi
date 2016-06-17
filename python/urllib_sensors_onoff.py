from urllib.request import urlopen
import json

url = 'http://localhost:3020/api/sensors/rpi'

fp = urlopen(url)
data = fp.read()
mystr = data.decode("utf8")
fp.close()

js = json.loads(mystr)
print(js['switch'])
