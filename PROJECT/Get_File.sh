#!/bin/sh
USER=pi
IP=192.168.1.9
PW=raspberry

expect <<EOF
set timeout 1
spawn scp -o StrictHostKeyChecking=no $USER@$IP:/home/pi/received/hello.txt /home/pi/
expect "password:"
send "$PW\r"
expect eof
EOF
