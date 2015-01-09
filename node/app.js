

function main() {

	var sprintf = require('./sprintf.js').sprintf;
	var app = require('express')();
	var Queue = require('./runqueue.js');

	// The animation to be used
	var _animation = null;
	var _queue = new Queue();
	var _defaultAnimation = {};
	
	_defaultAnimation.command = './run-rain';
	_defaultAnimation.args = ['-d', '-1'];
	

	function stopAnimation() {
		if (_animation != null) {
			var process = _animation;
			
			_animation = null;

			process.kill('SIGINT');

		}
	}	

	function startAnimation(cmd, callback) {

		try {
			function NO(error) {
				console.log("Failed to start animation...", error == undefined ? '' : error);
				callback();				
			}

			function YES() {
				callback();				
			}

			stopAnimation();
	
			var spawn = require('child_process').spawn;
			var animation = null;
			
			if (cmd == undefined)
				cmd = _defaultAnimation;
				
			if (callback == undefined) {
				callback = function() {
				};
			}

			console.log('Running animation: %s', cmd.command, cmd.args);					
			animation = spawn(cmd.command, cmd.args, { cwd: "../software"});
			
			if (animation == null) {
				NO();
			}
			else {
				animation.on('error', function() {
					NO();
				});
	
				animation.on('close', function() {
					YES();
				});		
			}
		}
		catch (error) {
			NO(error);
		}

		_animation = animation;		
		
	}
	

	_queue.on('idle', function() {
		startAnimation();
	});
	
	
	_queue.on('process', function(cmd, callback) {
		startAnimation(cmd, callback);
	});
	
	
	function addCommand(command, args) {
		_queue.push({command:command, args:args});
	}


	function queueMessage(messages, type) {
	
		try {
			// Make sure it is an array
			if (!Array.isArray(messages))
				messages = [messages];

			// Set up the dispatch table
			var messageType = {};
			
			messageType.settings = function(message) {
				
				if (message.animation != undefined) {
					stopAnimation();
					
					_defaultAnimation = message.animation;
					
					startAnimation();
				}

			}

			messageType.command = function(message) {
				
				if (message.name != undefined && message.args != undefined)
					addCommand(message.name, message.args);
					
			}
			
			messageType.text = function(message) {
				var args = [];
				
				if (message.textcolor != undefined)
					args.push('-c'), args.push(message.textcolor);

				if (message.color != undefined)
					args.push('-c'), args.push(message.color);

				if (message.font != undefined)
					args.push('-f'), args.push(message.font);

				if (message.size != undefined)
					args.push('-p'), args.push(message.size);
									
				if (message.iterations != undefined)
					args.push('-i'), args.push(message.iterations);

				if (message.message != undefined)
					args.push(message.message);

				addCommand('./run-text', args);
			}

			messageType.image = function(message) {
				var args = [];

				if (message.name != undefined)
					args.push(sprintf('images/%s', message.name));

				if (message.duration != undefined)
					args.push('-d'), args.push(message.duration);

				addCommand('./run-image', args);
			}

			messageType.animation = function(message) {
				var args = [];
				
				if (message.name != undefined)
					args.push(sprintf('animations/%s', message.name));

				if (message.iterations != undefined)
					args.push('-i'), args.push(message.iterations);

				if (message.duration != undefined)
					args.push('-d'), args.push(message.duration);

				addCommand('./run-animation', args);
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
			console.log("Upps!", error);
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
				message: sprintf('Connected to %s...', serverName)
			});
		});

		socket.on('disconnect', function() {
			console.log("SocketIO Disconnect");

			queueMessage({
				type: 'text',
				message: sprintf('Connection to %s lost.', serverName)
			});
			
		});		

		socket.on("command", function(data) {
			console.log("Got command", data);
			queueMessage(data, "command");
		});

		socket.on("message", function(data) {
			console.log("Got message", data);
			queueMessage(data);
		});
		
		socket.on("text", function(data) {
			console.log("Got text", data);
			queueMessage(data, "text");
		});

		socket.on("image", function(data) {
			console.log("Got image", data);
			queueMessage(data, "image");
		});

		socket.on("animation", function(data) {
			console.log("Got animation", data);
			queueMessage(data, "animation");
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
			
		queueMessage({
			type: 'text',
			textcolor: 'red',
			iterations: 2,
			message: text
		});
		
	}

	app.set('port', (process.env.PORT || 5000))
	
	app.listen(app.get('port'), function() {
		console.log("Node app is running at localhost:" + app.get('port'))
	});	
	
	enableSocketIO();
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

