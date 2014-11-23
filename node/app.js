var express = require('express')
var app   = express();
var twit  = require('twit');
var http  = require('http');
var fs    = require('fs');
var path  = require('path');

var sprintf = require('./sprintf.js').sprintf;
var JobQueue = require('./jobqueue.js');


function main() {

	var twitterOptions = {};	
	twitterOptions.consumer_key = 'RMvVK1wDXgftuFqVwMZA1OmEG';
	twitterOptions.consumer_secret = 'OlS3UoAMA48ZEWT8Ia2cYYTpZZRWNexBVzfhK84i93BXM1wDpK';
	twitterOptions.access_token = '1241291215-fKIUjhl3LVRO9KHukvb23Srcc4rsD9y4J22ErsL';
	twitterOptions.access_token_secret = 'lECypLbF3bTOd9r09uydHKUffuSS1zF8DgtTMfaGAHtWP';

	// The Twitter API
	var twitter = new twit(twitterOptions);
	var stream  = twitter.stream('user', { include_entities : true });

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
	
	function rand(min, max) {
		return Math.floor(Math.random() * (max - min + 1)) + min;
	}
	
	function choose() {
		return arguments[rand(0, arguments.length - 1)];
	}

	
	function stopAnimation() {
		if (animation != null) {
			var process = animation;
			
			animation = null;

			console.log('Stopping animation...');
			process.kill('SIGINT');

		}
	}	

	function scheduleAnimations() {
		var rule, schedule = require('node-schedule');

		rule = new schedule.RecurrenceRule();
		rule.minute = rand(0, 59);
		rule.hour = [8, 9, 10, 17, 18, 19, 20, 21, 22, 23];
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-perlin -d 60'));
		});	

		rule = new schedule.RecurrenceRule();
		rule.hour = [8, 9, 10, 17, 18, 19, 20, 21, 22, 23];
		rule.minute = rand(0, 59);
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-circle -d 30'));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = rand(0, 59);
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-life -d 30'));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = rand(0, 59);
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-animation images/pacman.gif'));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = rand(0, 59);
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-animation images/fireplace.gif -d 30'));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = rand(0, 59);
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-animation images/bubbles.gif -d 30'));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = rand(0, 59);
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-animation images/tree.gif'));
		});		


		rule = new schedule.RecurrenceRule();
		rule.minute = rand(0, 59);
		
		schedule.scheduleJob(rule, function() {
			addCmd(sprintf('./run-twinkle -d 30'));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = [15, 30, 45];
		rule.hour = [0, 1, 2, 3, 4, 5, 6, 7, 23];
		
		schedule.scheduleJob(rule, function() {
			var now = new Date();		
			addCmd(sprintf('./run-text "%02d:%02d" -i 2 -c red', now.getHours(), now.getMinutes()));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = [15, 30, 45];
		rule.hour = [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22];
		
		schedule.scheduleJob(rule, function() {
			var now = new Date();		
			addCmd(sprintf('./run-text "%02d:%02d" -i 2 -c blue', now.getHours(), now.getMinutes()));
		});		

		rule = new schedule.RecurrenceRule();
		rule.minute = 10;
		
		schedule.scheduleJob(rule, function() {
			var now = new Date();		
			addCmd(sprintf('./run-image images/smiley.png -s '));
		});		

	}

	function startAnimation() {

		var spawn = require('child_process').spawn;
		
		stopAnimation();

		var cmd = {};
		cmd.command = './run-clock';
		cmd.args = [];
		cmd.options = { cwd: "../software"};
			
		console.log('Starting animation: "%s"', cmd.command, cmd.args, cmd.options);	
			
		animation = spawn(cmd.command, cmd.args, cmd.options);
		
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

	

	stream.on('direct_message', function (message) {
		
		console.log("Direct message:", message.direct_message.text);
		
		var texts = message.direct_message.text.split('\n');
		
		for (var index in texts) {
			var text = texts[index];
			var match = null;
			
			match = text.match(/\s*@perlin\s*(.*)/);
			
			if (match != null) {
				addCmd(sprintf('./run-perlin %s', match[1]));
				continue;
			}

			match = text.match(/\s*@circle\s*(.*)/);
			
			if (match != null) {
				addCmd(sprintf('./run-circle %s', match[1]));
				continue;
			}
			
			match = text.match(/\s*@life\s*(.*)/);
			
			if (match != null) {
				addCmd(sprintf('./run-life %s', match[1]));
				continue;
			}

			match = text.match(/\s*@wipe\s*(.*)/);
			
			if (match != null) {
				addCmd(sprintf('./run-wipe %s', match[1]));
				continue;
			}

			match = text.match(/\s*@clock\s*(.*)/);
			
			if (match != null) {
				addCmd(sprintf('./run-clock %s', match[1]));
				continue;
			}
			
			match = text.match(/\s*@animation\s+([^-]\S+)(.*)/);
			
			if (match != null) {
				addCmd(sprintf('./run-animation images/%s.gif %s', match[1], match[2]));
				continue;
			}
			
			match = text.match(/\s*@image\s+([^-]\S+)(.*)/);
			
			if (match != null) {
				addCmd(sprintf('./run-image images/%s.png %s', match[1], match[2]));
				continue;
			}

			match = text.match('^[ ]*\./run-.+');

			if (match != null) {
				addCmd(text);			
				continue;		
			}
			
			addCmd(sprintf('./run-text "%s" -c blue', text));
		} 

	});


	stream.on('tweet', function (tweet) {


		var text = tweet.text;		
		var strip = text.indexOf('http://');
		
		console.log("tweet:", tweet.text);

		var retweet = text.match(/^RT\s+@.*?:\s*(.+)/);

		if (retweet == null) {
			// Strip off the trailing #http://...
			text = text.substr(0, strip < 0 ? undefined : strip).trim();
				
			if (tweet.user != undefined && tweet.user != null) {
				var profileImageUrl = tweet.user.profile_image_url;
				var profileName = tweet.user.name;
				var profileScreenName = tweet.user.screen_name;
	
				addCmd(sprintf('./run-text "%s" -c blue', profileName));
				addCmd(sprintf('./run-text "%s" -c red', text));
			}
			
		}		
		
	});
	
	app.set('port', (process.env.PORT || 5000))
	app.use(express.static(__dirname + '/public'))
	
	app.get('/', function(request, response) {
	  response.send('Hello World!')
	})
	
	app.listen(app.get('port'), function() {
	  console.log("Node app is running at localhost:" + app.get('port'))
	  startAnimation();
	});	
	
	scheduleAnimations();
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

