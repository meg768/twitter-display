Twitter Display
=============

This project uses much code provided by Glen Akin's tutorial 
(http://bikerglen.com/blog/driving-a-32x32-rgb-led-matrix-with-a-beaglebone-black-and-an-fpga)
on how to display images on an AdaFruit 32 x 32 matrix LED display. To follow this
you need all the hardware and wireing described in his tutorial.

I have added some code to make it listen to Twitter, so all tweets are displayed as scrolling text on the display.
It also uses a later version of the kernel, since I wanted to connect a wi-fi dongle to the Beaglebone.

Since it listens to Twitter it is campatible with IFTTT. In other words, the display can display a message at sunrise
telling you to wake up.

EDIT: It now listens to a server on Heroku using socket-io. The server itself listens to Google Talk (or Hangouts), Twitter, RSS and
other things...


Flashing the Beaglebone
-----------------------

Flash your Beaglebone Black with an image that boots from an SD-card if this is plugged in. This prevents
you from holding the reset button down while booting. With the LOGI-Bone attached, this is quite difficult.

See http://valentfx.com/vanilla/discussion/70/logi-image-sd-card-boot. I followed Ize's post
from August 23, 2014 and it all worked.	Make sure to remove the LOGI-bone when doing this and connect
to DC power. Otherwise it will fail.

Creating the SD Card Image
--------------------------

There is a tool called **Pi Filler** that writes images to SD-cards. You can download it here http://ivanx.com/raspberrypi.
It is easy to use and you don't have to worry about specifying the wrong disk to format. 
It was written to create Raspberry PI SD cards but it is all the same.

Download the image 'logi_ubuntu-14.04-console-armhf-2014-08-13.img' and create an SD card with this image. 
This is where I found it http://valentfx.com/doc/logi-image/logibone as of 2014-11-20.

After running **Pi Filler**, the SD card now contains two partitions. On the BOOT partion there is a file 
named **bbb-uEnv.txt**. Rename this to **uEnv.txt** and open it in a text editor and insert the following:

	optargs=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONE-EMMC-2G,BB-BONELT-HDMIN,BB-BONE-LOGIBONE

The file content of **uEnv.txt** should now look this:

	##These are needed to be compliant with Debian 2014-05-14 u-boot.
	
	optargs=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONE-EMMC-2G,BB-BONELT-HDMIN,BB-BONE-LOGIBONE
	
	loadximage=load mmc 0:2 ${loadaddr} /boot/vmlinuz-${uname_r}
	loadxfdt=load mmc 0:2 ${fdtaddr} /boot/dtbs/${uname_r}/${fdtfile}
	loadxrd=load mmc 0:2 ${rdaddr} /boot/initrd.img-${uname_r}; setenv rdsize ${filesize}
	loaduEnvtxt=load mmc 0:2 ${loadaddr} /boot/uEnv.txt ; env import -t ${loadaddr} ${filesize};
	loadall=run loaduEnvtxt; run loadximage; run loadxrd; run loadxfdt;
	
	mmcargs=setenv bootargs console=tty0 console=${console} ${optargs} ${cape_disable} ${cape_enable} root=${mmcroot} rootfstype=${mmcrootfstype} ${cmdline}
	
	uenvcmd=run loadall; run mmcargs; bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr};

Insert the SD card into the Beaglebone, and power up.

Installing the Software
-----------------------

Find the IP-address of the Beaglebone. Try out **LanScan** on the App-Store, it is free. In this README, it is 
assumed to be 10.0.1.58.

Start a Terminal window on your Mac and type:

	$ ssh ubuntu@10.0.1.58

The password is "temppwd". Once logged in, type this:

	$ git clone https://github.com/meg768/twitter-display.git
	$ cd twitter-display
	$ make install

This will take about 20 minutes. 

If everything is OK, type:

	$ make setup run
	
Hopefully you will see something on the LED matrix... ;)


Auto Run on Boot
----------------

To make this run every time you boot the Beaglebone, edit the file **/etc/rc.local** 

	sudo nano /etc/rc.local

and type in

	cd /home/ubuntu/twitter-display && sudo make setup run

Skipping Password Login
-----------------------

If you get tired of always logging in with a password with **ssh**, type this on your Mac:

	$ ssh-keygen -t dsa && cat ~/.ssh/id_rsa.pub | ssh ubuntu@10.0.1.58 "mkdir ~/.ssh; cat >> ~/.ssh/authorized_keys"

Press enter untill you see the '$' prompt again. When asked for a password, type

	temppwd
	
Now, the next time you connect to the Beaglebone you will not have to type in the password.


Updating the Kernel
-------------------

If you want use wifi, you probably will have to update the kernel.
To do so, type this on the Beaglebone:

	$ cd /opt/scripts/tools && sudo ./update_kernel.sh && sudo reboot

Expanding the Root Partition
----------------------------

Here is a easy tutorial on how to expand the **rootfs** partition on the SD card,  
http://elinux.org/Beagleboard:Expanding_File_System_Partition_On_A_microSD




