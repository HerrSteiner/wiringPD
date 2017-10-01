# wiringPD
Externals for usage of Pure Data on Raspberry Pi

Malte Steiner 2017

first external is to read data through hardware SPI from MCP3208 12bit converter chip which seems now more common as the MCP3008. To run it wiring needs to be installed and hardware SPI enabled with raspi-config. On current Raspberry Pi it seems to work in PD even without running it as superuser.

A schematic can be find at http://block4.com/index.php?id=167

To build it on Raspberry Pi, you need to have the build tools, wiringpi and puredata-dev installed. Go into the subfolder and run make. Copy or move the external to /usr/lib/pd/extra/wiring/ so PD can use it. 

We use this to control a multieffect PD patch on Raspberry Pi with diverse sensors for our project TMS: http://tms.tmkm.dk


based on Wiring, PD-Wiring Pi by Jaime Oliver La Rosa (http://nyu-waverlylabs.org/rpi-gpio), diverse forum threads and PiCollider by Constantin 3000
