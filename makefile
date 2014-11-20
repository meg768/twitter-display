BEAGLEBONE_IP   = 10.0.1.58
BEAGLEBONE_USER = ubuntu
BEAGLEBONE_PATH = twitter-box

all: 
	
run:
	cd logi-bone-setup && sudo ./setup.sh
	cd node-app && sudo node app.js	

install: \
	install-tools \
	install-software \
	install-node-modules \
	update-timezone

install-tools:
	sudo apt-get update
	sudo apt-get install -y git build-essential libmagick++-dev curl python-imaging nodejs

install-node-modules:
	$(MAKE) -C node-app
	
install-software:
	$(MAKE) -C software

update-kernel:
	cd /opt/scripts/tools && sudo ./update_kernel.sh && sudo reboot

update-timezone:
	sudo timedatectl set-timezone Europe/Stockholm



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

commit:
	git add -A
	git commit -m "..."
	git push -u origin master
	