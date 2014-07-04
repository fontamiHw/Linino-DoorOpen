Linino-DoorOpen
===============

Linino program based on mjpg-streamer library.

The program runs mjpg-streamer library with some fixed parameter :

The code that dose the output configuration is on 
mjpg-streamer.c file 

around line 206
	output[0] = "output_file.so --folder /opt/DoorAlarm/ --delay 250 --size 100";



