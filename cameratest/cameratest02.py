import socket
import time
import picamera

with picamera.PiCamera() as camera:
  camera.resolution = (640, 480)
  camera.framerate = 24
  camera.vflip = True
  camera.hflip = True

  server_socket = socket.socket()
  server_socket.bind(('0.0.0.0', 2000))
  server_socket.listen(0)

  # Accept a single connection and make a file-like object out of it
  connection = server_socket.accept()[0].makefile('wb')
  print("camera is on")

  try:
    camera.start_recording(connection, format='h264')
    camera.wait_recording(200)
  except KeyboardInterrupt:
    camera.stop_recording()
    connection.close()
    server_socket.close()
