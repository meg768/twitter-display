#!/bin/bash

DELAY_PIC=4
DELAY_VISUAL=8

echo "setup device tree"
#SETUP DEVICE TREE##########################################
rm /lib/firmware/BB-BONE-LOGIBONX-SLOW.dtbo
dtc -O dtb -o BB-BONE-LOGIBONX-SLOW.dtbo -b 0 -@ BB-BONE-LOGIBONX-SLOW.dts
cp BB-BONE-LOGIBONX-SLOW.dtbo /lib/firmware
sh -c "echo BB-BONE-LOGIBONX:SLOW > /sys/devices/bone_capemgr.9/slots "
cat  /sys/devices/bone_capemgr.9/slots

#LOAD BIT FILE################################################
echo "loading bitstream"
logi_loader bit/beagle01.bit


echo "run demo loop"
#RUN DEMO#####################################################

while :
do

timeout $DELAY_VISUAL ./sw/runperlin

./sw/picture_beagle 
sleep $DELAY_PIC

timeout $DELAY_VISUAL ./sw/runtwinkle  

timeout $DELAY_VISUAL ./sw/runwash 

timeout $DELAY_VISUAL ./sw/runcircle

#timeout $DELAY_VISUAL ./sw/runwipe 

./sw/picture_biker 
sleep $DELAY_PIC

done    #end while

