var express = require('express');
var fs = require('fs');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  // res.render('index', { title: 'Express' });
  res.sendFile('weather.html', {root: 'public'});
});
router.get('/getcity', function(req,res,next){
	console.log("in getcity route");
	fs.readFile(__dirname + '/cities.dat.txt', function(err,data){
		if(err) throw err;
		var cities = data.toString().split("\n");
		for(var i = 0; i < cities.length; i++){
			console.log(cities[i]);
		}
	});
});

module.exports = router;
