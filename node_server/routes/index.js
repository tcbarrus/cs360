var express = require('express');
var fs = require('fs');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendFile('weather.html', {root: 'public'});
});
router.get('/getcity', function(req,res,next){
	fs.readFile(__dirname + '/cities.dat.txt', function(err,data){
		if(err) throw err;
		var cities = data.toString().split("\n");
		var myRe = new RegExp("^" + req.query.q);
		console.log(meRe);
		var json = [];
		for(var i = 0; i < cities.length; i++){
			var result = cities[i].search(myRe);
			if(result != -1){
				// console.log(cities[i]);
				json.push({city:cities[i]});
			}
		}
		res.status(200).json(json);
	});
});

module.exports = router;
