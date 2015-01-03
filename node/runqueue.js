

function Module() {
	
	var _this = this;

	var _cmdQueue = [];
	var _working = false;

	function work() {
		if (_cmdQueue.length > 0) {
			
			if (!_working) {
				var item = _cmdQueue[0];

				_working = true;

				_this.emit('process', item, function() {
					// Remove current job
					_cmdQueue.shift();					
					_working = false;
					
					// And continue with the next, when finished
					work();
					
					
				});
			} 
			
		}
		else {
			_working = false;	
			_this.emit('idle');
		}
	}
	
	_this.push = function(item) {
		_cmdQueue.push(item);
				
		work();
		
	}
};


function main() {
	module.exports = Module;
	
	var util   = require('util');
	var events = require('events');
	util.inherits(Module, events.EventEmitter);
}


main();