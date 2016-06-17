#!/bin/sh
USER=pi #server ID
IP=192.168.1.24 #server ip
PW=raspberry #server pwd

expect <<EOF
set timeout 1
spawn scp -o StrictHostKeyChecking=no $USER@$IP:/home/pi/received/hello.txt /home/pi/
expect "password:"
send "$PW\r"
expect eof
EOF



