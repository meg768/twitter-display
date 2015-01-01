

function main() {

	// The array of tweets
	var _cmdQueue = [];
	var _working = false;

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
	
	function add(cmd) {
		_cmdQueue.push(cmd);
		work();
		
	}
}



main();
