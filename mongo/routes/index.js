var express = require('express');
var router = express.Router();
var mongoose = require('mongoose');

mongoose.connect('mongodb://localhost/commentDB');

var commentSchema = mongoose.Schema({
	Name: String,
	Comment: String
});

var comment = mongoose.model('Comment', commentSchema);
var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', function(){
	console.log('Connected');
});

router.post('/comment', function(req, res, next) {
  var newComment = new comment(req.body);
  console.log("POST comment route"); //[1]
  console.log(newComment);
  newComment.save(function(err,post){
	if(err){
		return console.err(err);
	}
	console.log(post);
	res.sendStatus(200);
  });
});

router.get('/comment', function(req, res, next){
	comment.find(function(err, commentList){
		if(err) return console.error(err);
		else{
			res.json(commentList);
		}
	});
});

module.exports = router;
