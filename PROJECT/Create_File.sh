#!/bin/bash

#loop: create hello.txt every 30sec
while [ : ];
do
  touch hello.txt
  echo "new file created"
  sleep 30;
done
