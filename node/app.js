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
	
	var stream = twitter.stream('user', { include_entities : true });

	// The animation to be used
	var animation = null;

	// The array of tweets
	var inputQueue = new JobQueue();
	var outputQueue = new JobQueue();

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

		
		var animations = [
			{
				command: './run-perlin',
				args: ['-d', '120']
			},
			{
				command: './run-circle',
				args: ['-d', '60']
			},
			{
				command: './run-life',
				args: ['-d', '15']
			},
			{
				command: './run-clock',
				args: ['-d', '900']
			},
			{
				command: './run-animation',
				args: ['-i', '1', './images/pacmac.gif']
			},
			{
				command: './run-animation',
				args: ['-d', '30', './images/snow.gif']
			},
			{
				command: './run-animation',
				args: ['-d', '30', './images/boxer.gif']
			},
			{
				command: './run-animation',
				args: ['-i', '1', './images/dancer.gif']
			},
			{
				command: './run-animation',
				args: ['-i', '2', './images/tree.gif']
			},
			{
				command: './run-animation',
				args: ['-d', '30', './images/boat.gif']
			},
			{
				command: './run-animation',
				args: ['-d', '30', './images/squares.gif']
			},
			{
				command: './run-animation',
				args: ['-d', '30', './images/fireplace.gif']
			},
			{
				command: './run-animation',
				args: ['-d', '30', './images/rain.gif']
			},
			{
				command: './run-animation',
				args: ['-i', '2', './images/bubbles.gif']
			}
		];			
	
		function rand(min, max) {
			return Math.floor(Math.random() * (max - min + 1)) + min;
		}
		
		function choose(object) {
			return object[rand(0, object.length - 1)];
		}
		
		stopAnimation();

		var cmd = choose(animations);
		
		cmd = animations[3];

		
		cmd.options = {
			
			cwd: '../software'			
		};
		
		cmd.options = { cwd: "../software"};
			
		console.log('Starting animation: "%s"', cmd.command, cmd.args, cmd.options);	
			
		animation = spawn(cmd.command, cmd.args, cmd.options); //, cmd.args, cmd.options);
		
		animation.on('close', function() {
			if (animation != null) {
				console.log('Animation finished. Restarting...');

				animation = null;
				startAnimation(choose(animations));
			}
			else {
				console.log('Animation killed.');
			}
			
		});		
	}

	function displayText(text, color) {
	
		outputQueue.add(function(callback) {
			shell(sprintf('./run-text "%s" -c "%s"', text, color), callback);
		});
	}
	
	stream.on('direct_message', function (message) {
		console.log("****************************");
		console.log(message.direct_message.entities.urls);
		console.log("****************************");
		console.log(message.direct_message.entities.media);
		console.log("****************************");
	});
	stream.on('tweet', function (tweet) {


		function displayTweet(finished) {
			var text = tweet.text;		
			var strip = text.indexOf('http://');
			
			// Strip off the trailing #http://...
			text = text.substr(0, strip < 0 ? undefined : strip).trim();
				
			if (tweet.user != undefined && tweet.user != null) {
				var profileImageUrl = tweet.user.profile_image_url;
				var profileName = tweet.user.name;
				var profileScreenName = tweet.user.screen_name;
		
				stopAnimation();

				displayText(profileName, "blue");
				displayText(text, "red");
				
				outputQueue.add(function(callback) {
					finished();
					callback();
				
					console.log('Finished. %d tweets left before animation...', inputQueue.length());
					 
					if (inputQueue.length() == 0)
						startAnimation();	
					
				});

			}
			else
				finished();
			
		}

		inputQueue.add(displayTweet);
		
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

