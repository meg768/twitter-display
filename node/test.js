

	var app = require('express')();


	
	app.listen(3000, function() {
		console.log("Node app is running at localhost:" + 3000);
	});	
	

var request = require('request');

var url = 'https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quote%20where%20symbol%3D%22AAPL%22&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys&callback=';

request(url, function (error, response, body) {
  if (!error && response.statusCode == 200) {
  	var json = JSON.parse(body);
  	console.log('A');
    console.log(json.query.results) // Show the HTML for the Google homepage.
  	console.log('A');
  }
})