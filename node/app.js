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
			cmd.command = './run-clock';
			cmd.args = [];
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

	function addMessages(messages, type) {
	
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

	function enablePusher() {
		var Pusher      = require('pusher-client');
		var channelName = "test_channel";
		
		console.log("Initiating Pusher...");
		var socket = new Pusher('062bc67be8d42e4ded9b');
		
		console.log("Subscribing to channel '%s'", channelName);
		var channel = socket.subscribe(channelName);
		
		socket.bind("command", function(data) {
		
			console.log("Got command: ", data);
			
			if (typeof data == "string")
				addCmd(data);					

		});

		
		socket.bind("message", function(data) {
			addMessages(data);
		});
		
		socket.bind("text", function(data) {
			addMessages(data, "text");
		});
		
	}
 
	function sayHello() {
		var ip = getIP('wlan0');
		var text = ip != '' ? sprintf('%s      %s      %s', ip, ip, ip) : 'Wi-fi connection is missing...';
		
		shell(sprintf('./run-text "%s      %s      %s"', ip, ip, ip), function() {
			enablePusher();
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

