
Flash your Beaglebone Black with an image that boots from an SD-card if this is plugged in. This prevents
you from holding the reset button down while booting. With the LOGI-Bone attached, this is quite difficult.


See http://valentfx.com/vanilla/discussion/70/logi-image-sd-card-boot. I followed Ize's post
from August 23, 2014 and it all worked.	Make sure to remove the LOGI-bone when doing this and connect
to DC power. Otherwise it will fail.

There is a tool "Pi Filler" that writes images to SD-cards. Download it http://ivanx.com/raspberrypi.

Download the image 'logi_ubuntu-14.04-console-armhf-2014-08-13.img' and create an SD card with this image. 
This is where I found it http://valentfx.com/doc/logi-image/logibone as of 2014-11-20.

The SD card now contains two partitions. On the BOOT partion there is a file named **bbb-uEnv.txt**. Rename this to **uEnv.txt**
and open it in a text editor and insert the following:

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

Find the IP-address of the Beaglebone. Try out "LanScan" on the App-Store, it is free. In this README, it is 
assumed to be 10.0.1.58.

Start a Terminal window on your Mac and type:

	$ ssh ubuntu@10.0.1.58

The password is "temppwd". Once logged in, type this:

	$ git clone https://github.com/meg768/twitter-display.git
	$ cd twitter-display
	$ make install

This will take about 10 minutes. 

If everything is OK, type

	$ make run
	
Hopefully you will see something on the LED matrix... ;)

If you get tired of always logging in with a password with **ssh**, type this on your Mac:

	$ ssh-keygen -t dsa && cat ~/.ssh/id_rsa.pub | ssh ubuntu@10.0.1.61 "mkdir ~/.ssh; cat >> ~/.ssh/authorized_keys"

Press enter untill you see the '$' prompt again. When asked for a password, type

	temppwd
	
Now, the next time you connect to the Beaglebone you will not have to type in the password.

----------------

Connect to it and make sure all is OK.

$ ssh debian@10.0.1.58

Logout

--------------
Make sure /dev/logibone and /dev/logibone_mem exists (and spi-dev1??)

-------

On your mac, type:

$ ssh-keygen -t dsa && cat ~/.ssh/id_rsa.pub | ssh ubuntu@10.0.1.61 "mkdir ~/.ssh; cat >> ~/.ssh/authorized_keys"

------------------
Expand partition on the Beaglebone

http://elinux.org/Beagleboard:Expanding_File_System_Partition_On_A_microSD

------------
Upgrade kernel

$ cd /opt/scripts/tools/
$ sudo ./update_kernel.sh

-------------

Install git, compilers, Magick++ and curl
$ sudo apt-get update && sudo apt-get install -y git && sudo apt-get install -y build-essential && sudo apt-get install -y libmagick++-dev && sudo apt-get install -y curl



	---------
	Install logi tools
	
	git clone https://github.com/fpga-logi/logi-tools.git
	cd logi-tools
	sudo ./install_logibone.sh
	sudo shutdown -r now
	
	--------------
	Install logi-apps
	
	Test the blink app
	

--------------
Install DTC?
wget -c https://raw.githubusercontent.com/RobertCNelson/tools/master/pkgs/dtc.sh
chmod +x dtc.sh
./dtc.sh

----------------
Disable the BB-BONE-LOGIBONE device at startup

optargs=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONE-EMMC-2G,BB-BONELT-HDMIN,BB-BONE-LOGIBONE

	
	$ sudo nano /boot/uboot/uEnv.txt
	$ sudo reboot
	
-------------------

---------------
Compile the software

	cd
	cd software
	make
	
------------


Install node:
To use nodejs, check out https://github.com/joyent/node/wiki/installing-node.js-via-package-manage. 
For Ubuntu use the following:

  * curl -sL https://deb.nodesource.com/setup | sudo bash -
  * sudo apt-get install -y nodejs


-------
Update the node modules

	$ cd node-app
	$ npm update  

----------
Load the bit file
$ cd logi-bone
$ sudo ./setup.sh
  
  
  
