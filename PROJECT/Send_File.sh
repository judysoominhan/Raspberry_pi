#!/bin/sh
USER=pi #server ID
IP=192.168.1.24 #server ip
PW=raspberry #server pwd
SENDING_FILE=/home/pi/ssum/hello.txt #file

#method: sending a file to server
sending_file() {
expect <<EOF
  set timeout 1
  spawn scp -o StrictHostKeyChecking=no $SENDING_FILE $USER@$IP:/home/pi/
  expect "password:"
  send "$PW\r"
  expect eof
EOF
}

#loop: if file exist, send a file and remove(every 10 sec); if not, print error msg
while [ : ];
do
if [ -f "$SENDING_FILE" ] 
then
  sending_file
  rm -rf $SENDING_FILE
  sleep 10
else
  echo "file does not exist"
  sleep 10
fi
done
