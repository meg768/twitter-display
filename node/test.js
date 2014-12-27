function getIP(device) {
	var os = require('os');
	var ifaces = os.networkInterfaces();

	var iface = ifaces[device];
	
	if (iface != undefined) {
	
		for (var i in ifaces) {
			var item = ifaces[i];
	
			if (item.family == 'IPv4')
				return item.address;
		}
	}

	return '';
}


function waitForIP(device) {
	var ip = getIP(device);
	
	if (ip == '') {
		setTimeout(waitForIP, 1000);
	}	
}

//waitForIP("wlan1");
console.log("IP: ", getIP("wlan0"));
