#!/bin/bash

rm /lib/firmware/BB-BONE-LOGIBONX-SLOW.dtbo
dtc -O dtb -o BB-BONE-LOGIBONX-SLOW.dtbo -b 0 -@ BB-BONE-LOGIBONX-SLOW.dts
cp BB-BONE-LOGIBONX-SLOW.dtbo /lib/firmware
sh -c "echo -0 > /sys/devices/bone_capemgr.9/slots "
sh -c "echo BB-BONE-LOGIBONX:SLOW > /sys/devices/bone_capemgr.9/slots "
logi_loader beagle01.bit
exit 0