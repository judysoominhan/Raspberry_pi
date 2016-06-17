from urllib.request import urlopen
import json
import time

url = 'http://localhost:3020/api/sensors/573428491c7c44310a5856f6'

while True:
	fp = urlopen(url)  # u is a file like object
	data = fp.read() # bytearray
	mystr = data.decode("utf8")
	fp.close()
	#print(mystr)

	js = json.loads(mystr)
	print(js['switch'] )

	time.sleep(5)
