from subprocess import call
call['home/pi/mjpg-streamer/mjpg-streamer-experimental/mjpg_streamer', '-i', 'input_raspicam.so -d 200 -vf', '-o', 'output_http.so -w /home/pi/mjpg-streamer-experimental/www']
