
module.exports = function() {

	var queue = [];
	
	function work() {
		if (queue.length > 0) {
			// Get 'top' command
			var job = queue[0];
			
			if (!job.running) {
				job.running = true;
				
				job.run(function() {
					queue.shift();

					if (job.callback != undefined)
						job.callback();
					
					work();
				});
			} 
			
		}
		
	}
	
	this.length = function() {
		return queue.length;
	}
	
	this.add = function(run, callback) {
		
		var job = {};
		job.running = false;
		job.run = run;
		job.callback = callback;
		
		queue.push(job);
		
		work();
	}

}

