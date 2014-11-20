var sprintf = require('./sprintf.js').sprintf;
var spawn = require('child_process').spawn;
var JobQueue = require('./jobqueue.js');

var jobs = new JobQueue();
var jobID = 1;

function addJob() {
	
	function run(callback) {
	
		var jobName = sprintf('Job #%d', jobID++);
		
		var cmd = sprintf('ls', Math.floor((Math.random() * 5) + 1));

		var process = spawn(cmd);
		
		console.log('%s started.', jobName);
		
		process.on('close', function() {
			console.log('%s CLOSE.', jobName);	
			callback();
		});
		/*
		process.on('exit', function() {
			console.log('%s EXIT.', jobName);	
			callback();
		});
		*/
		process.on('error', function() {
			console.log('%s ERROR.', jobName);	
			callback();
		});

	}
	
	function completed() {
		console.log('Completed!');	
	}
	
	jobs.add(run, completed);
}

addJob();
addJob();
addJob();
addJob();

/*

setTimeout(function(){
    console.log('Trying to kill it');
//	child.stdin.pause();
  child.kill('SIGINT');
   // console.log('Should be killed now...');
//    exec('kill run-static');

}, 1200);

*/