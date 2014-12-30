var express = require('express')
var twit    = require('twit');
var http    = require('http');
var fs      = require('fs');
var path    = require('path');
var exec    = require('child_process').exec;
var sprintf = require('./sprintf.js').sprintf;


function main() {

	var app = express();

	// The animation to be used
	var animation = null;

	// The array of tweets
	var _cmdQueue = [];
	var _working = false;

	function shell(cmd, callback) {
		var exec = require('child_process').exec;
		
		var options = {};
		options.cwd = '../software';
		
		console.log('Executing command: "%s"...', cmd);
		
		return exec(cmd, options, function(error, stdout, stderr) {
			console.log('Command completed.', cmd);
			
			if (callback != undefined)
				callback(error);
		});
		
	}
	
	
	function stopAnimation() {
		if (animation != null) {
			var process = animation;
			
			animation = null;

			console.log('Stopping animation...');
			process.kill('SIGINT');

		}
	}	


	function startAnimation() {

		var spawn = require('child_process').spawn;
		
		stopAnimation();

		try {
			var cmd = {};
			cmd.command = './run-rain';
			cmd.args = ['-d', '-1'];
			cmd.options = { cwd: "../software"};
				
			console.log('Starting animation: "%s"', cmd.command, cmd.args, cmd.options);	
				
			animation = spawn(cmd.command, cmd.args, cmd.options);
			
			animation.on('error', function() {
				console.log("Failed to start animation...");
			});

			animation.on('close', function() {
				if (animation != null) {
					console.log('Animation finished. Restarting...');
	
					animation = null;
					startAnimation();
				}
				else {
					console.log('Animation killed.');
				}
				
			});		
			
		}
		catch (error) {
			console.log("Failed to start animation...", error);
		}
	}

	function work() {
		if (_cmdQueue.length > 0) {
			var cmd = _cmdQueue[0];
			
			if (!_working) {
				_working = true;
				
				stopAnimation();

				shell(cmd, function() {	
					_cmdQueue.shift();					
					_working = false;
					
					work();
				});				
			} 
			
		}
		else {
			_working = false;	
			startAnimation();	
		}
	}
	
	function addCmd(cmd) {
		_cmdQueue.push(cmd);
		work();
		
	}

	function queueMessage(messages, type) {
	
		try {
			// Make sure it is an array
			if (!Array.isArray(messages))
				messages = [messages];

			// Set up the dispatch table
			var messageType = {};
			
			messageType.text = function(message) {
				var cmd = "./run-text ";
				
				if (typeof message.textcolor == "string")
					cmd += sprintf("-c %s ", message.textcolor);
									
				if (typeof message.message == "string")
					cmd += sprintf('"%s" ', message.message);

				if (typeof message.options == "string")
					cmd += sprintf('%s ', message.options);

				addCmd(cmd);
			}

			messageType.image = function(message) {
				var cmd = "./run-image ";
				
				if (typeof message.name == "string")
					cmd += sprintf('"images/%s" ', message.name);

				if (typeof message.options == "string")
					cmd += sprintf('%s ', message.options);

				addCmd(cmd);
			}
			

			for (var i in messages) {
				var message = messages[i];
	
				if (message == undefined)
					continue;
					
				if (typeof type == "string")
					message.type = type;
								
				var addMessage = messageType[message.type];
				
				if (addMessage == undefined) {
					console.log("Unknown message type '%s'", message.type);
					continue;
				}
					
				addMessage(message);
			}
			
		}
		catch (error) {
			console.log("Upps!");
		}
		
	}

	function getIP(device) {
		var os = require('os');
		var ifaces = os.networkInterfaces();
	
		var iface = ifaces[device];
		
		if (iface != undefined) {
		
			for (var i in iface) {
				var item = iface[i];
				console.log(item);
		
				if (item.family == 'IPv4')
					return item.address;
			}
		}
	
		return '';
	}

	
	function enableSocketIO() {
		var serverURL  = 'http://akuru.herokuapp.com';
		var serverName = 'Heroku';
		
		var socket = require('socket.io-client')(serverURL);

		socket.on('connect', function() {
			console.log("SocketIO Connected");
			
			queueMessage({
				type: 'text',
				textcolor: 'cyan',
				message: sprintf('Connected to %s!', serverName)
			});
		});

		socket.on("message", function(data) {
			queueMessage(data);
		});
		
		socket.on("text", function(data) {
			queueMessage(data, "text");
		});

		socket.on("image", function(data) {
			queueMessage(data, "image");
		});
		
		socket.on('disconnect', function() {
			console.log("SocketIO Disconnect");

			queueMessage({
				type: 'text',
				textcolor: 'cyan',
				message: sprintf('Connection to %s lost just faded away...', serverName)
			});
			
		});		
	}
 
	function sayHello() {
		var wlan0 = getIP('wlan0');
		var eth0 = getIP('eth0');
		
		var text = '';
		
		if (wlan0 != '')
			text += sprintf('Wireless - %s ', wlan0);
			
		if (eth0 != '')
			text += sprintf('Ethernet - %s ', eth0);
		
		shell(sprintf('./run-text "%s" -i 2', text), function() {
			enableSocketIO();
			startAnimation();
			
		});
		
	}

	app.set('port', (process.env.PORT || 5000))
	
	app.listen(app.get('port'), function() {
		console.log("Node app is running at localhost:" + app.get('port'))
	});	
	
	sayHello();

}



main();





/*

	// Function to download url:s
	function download(url, dest, callback) {
	
		console.log('Downloading URL "%s"...', url);
		
		var file = fs.createWriteStream(dest);
		var request = http.get(url, function(response) {
	    
	    	response.pipe(file);
			
			file.on('finish', function() {
				console.log('Download complete.');
				file.close(callback);
			});
			
		}).on('error', function(error) {
	    	fs.unlink(dest); 
	    	
			if (callback) 
				callback(error.message);
		});
	};

*/

