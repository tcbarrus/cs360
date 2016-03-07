var fs = require('fs');
fs.readFile(process.argv[2],function read(err, data){
	if(err){
		throw err;
	}
	var string = data.toString();
	var array = string.split('\n');
	console.log(array.length - 1);
});
