

	var app = require('express')();


	
	app.listen(3000, function() {
		console.log("Node app is running at localhost:" + 3000);
	});	
	

var Queue = require('./runqueue.js');
var queue = new Queue();

var i = 0;

queue.on('idle', function() {
	i++;
	console.log('IDLE, adding new text ', i);
	this.add(addText, "Text " + i, "OLLE");
});

queue.on('busy', function() {
});


function addText(text, text2, finished) {
	console.log("addText", text, text2);
	setTimeout(finished, 1000);
	
}

function addAnimation(animation, finished) {
	console.log("starting animation", animation);
	setTimeout(finished, 1000);
	
}

/*punycode = require('punycode');
console.log(punycode.ucs2.decode('Hej 😄 😄 😄'));
console.log(punycode.ucs2.encode([ 128520, 101, 106, 128517, 32, 128516 ]));
*/

queue.add(addText, "Hej Magnus", "XXX");
queue.add(addAnimation, "pacman version 1");
queue.add(addAnimation, "pacman version 2");
queue.add(addAnimation, "pacman version 3");
queue.add(addText, "Hej Olog", "YYY");
queue.add(addText, "Hej Egelberg", "ZZZ");


/*

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
*/