import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.IN)
GPIO.setup(23, GPIO.OUT)

try:
	while True:
		input_value = GPIO.input(18)

		if input_value == False:
			GPIO.output(23, True)
			time.sleep(1)

		else:
			GPIO.output(23, False)
			time.sleep(1)
except KeyboardInterrupt:
	GPIO.cleanup()
