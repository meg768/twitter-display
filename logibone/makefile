all: clean upload

upload: /lib/firmware/BB-BONE-LOGIBONX-SLOW.dtbo
	sh -c "echo -0 > /sys/devices/bone_capemgr.9/slots "
	sh -c "echo BB-BONE-LOGIBONX:SLOW > /sys/devices/bone_capemgr.9/slots "
	logi_loader beagle01.bit

clean:
	rm /lib/firmware/BB-BONE-LOGIBONX-SLOW.dtbo


/lib/firmware/BB-BONE-LOGIBONX-SLOW.dtbo: BB-BONE-LOGIBONX-SLOW.dts
	dtc -O dtb -o /lib/firmware/BB-BONE-LOGIBONX-SLOW.dtbo -b 0 -@ BB-BONE-LOGIBONX-SLOW.dts


