#!/bin/sh
USER=pi
IP=192.168.1.24
PW=raspberry
while [ : ];
do
expect <<EOF
set timeout 1
spawn scp -o StrictHostKeyChecking=no hello.txt $USER@$IP:/home/pi/
expect "password:"
send "$PW\r"
expect eof
EOF
sleep 2;
done
