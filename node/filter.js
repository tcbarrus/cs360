var fs = require('fs');
var files = require('./module.js');
var stuff = files(process.argv[2], process.argv[3], function(err, data){
	if(err)
		throw err;
	//console.log(data);
	for(var x in data){
		console.log(data[x]);
	}
});
//console.log(stuff);
/*fs.readdir(process.argv[2], function read(err, data){
	if(err){
		throw err;
	}
	//console.log(data);
	var ext = '.' + process.argv[3];
//	console.log(ext);
	for(var x in data){
		if(data[x].indexOf(ext) !== -1){
			console.log(data[x]);			
		}
	}	
});*/
