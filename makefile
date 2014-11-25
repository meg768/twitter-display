X=000
Y = 0
BEAGLEBONE_IP   = 10.0.1.61
BEAGLEBONE_USER = ubuntu
BEAGLEBONE_PATH = twitter-display

all: 
	
startup:
	cd wifi-reset && sudo ./wifi-reset.sh
	cd logibone && sudo ./setup.sh
	cd node && sudo node app.js > app.log	

run:
	cd node && sudo node app.js	

setup:
	cd logibone && sudo ./setup.sh

install: \
	install-tools \
	install-dtc \
	install-software \
	install-node-modules

install-tools:
	sudo apt-get update
	sudo apt-get install -y git
	sudo apt-get install -y build-essential
	sudo apt-get install -y libmagick++-dev
	sudo apt-get install -y curl
	sudo apt-get install -y python-imaging
	curl -sL https://deb.nodesource.com/setup | sudo bash -
	sudo apt-get install -y nodejs

install-node-modules:
	$(MAKE) -C node
	
install-software:
	$(MAKE) -C software

install-dtc:
	sudo wget -c https://raw.githubusercontent.com/RobertCNelson/tools/master/pkgs/dtc.sh
	sudo chmod +x dtc.sh
	sudo ./dtc.sh
	sudo rm ./dtc.sh

update-kernel:
	cd /opt/scripts/tools && sudo ./update_kernel.sh && sudo reboot

commit:
	git add -A
	git commit -m "..."
	git push -u origin master
	

#
#
# Following commands are to be run from my Mac, not the Beaglebone
#
#

# Uploads everything to the Beaglebone
upload:
	rsync -rav * $(BEAGLEBONE_USER)@$(BEAGLEBONE_IP):/home/$(BEAGLEBONE_USER)/$(BEAGLEBONE_PATH)

# Temp directory for mounting
/tmp/Beaglebone:
	mkdir /tmp/Beaglebone

# Sets up a drive on my Mac desktop
mount: /tmp/Beaglebone
	sshfs $(BEAGLEBONE_USER)@$(BEAGLEBONE_IP):/ /tmp/Beaglebone

