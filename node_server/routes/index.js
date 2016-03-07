var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  // res.render('index', { title: 'Express' });
  res.sendFile('weather.html', {root: 'public'});
});
router.get('/getcity', function(req,res,next){
	console.log("in getcity route");
});

module.exports = router;
