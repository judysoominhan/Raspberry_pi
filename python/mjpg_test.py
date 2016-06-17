import os
import subprocess
import sys

p = os.environ['HOME']
print (p)

STREAMER_PATH = os.environ['HOME/mjpg-streamer/mjpg-streamer-experimental']
#os.environ['STREAMER_PATH'] = "HOME/mjpg-streamer/mjpg-streamer-experimental"
print (STREAMER_PATH)

#os.environ['LD_LIBRARY_PATH'] = "$STREAMER_PATH"

#subprocess.call('$STREAMER_PATH/mjpg_streamer -i "input_raspicam.so -d 200 -vf" -o "output_http.so -w $STREAMER_PATH/www"')
