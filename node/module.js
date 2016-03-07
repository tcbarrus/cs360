module.exports = function(dir, fileType, callback){
var fs = require('fs');
fs.readdir(dir, function(err, data){
	if(err)
		return callback(err);
	var ext = '.' + fileType;
	var files = new Array();
	for(var x in data){
		if(data[x].indexOf(ext) !== -1){
			files.push(data[x]);	
		}
	}
	//exports.files = files;
//	console.log(files);
	return callback(false, files);
});
}
