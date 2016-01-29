var chit = [8,4,5,3,11,10,9,2,11,12,9,5,6,4,6,3,10,8];
var terrain = ["desert","hill","hill","mountain","forest","forest","pasture","mountain","field","field","pasture","pasture","hill","forest","forest","field","mountain","pasture","field"];

Meteor.map = {
	initializeMap : function(num,hex,port){
		var numArray = (num) ? shuffleArray(chit) : chit;
		var hexArray = (hex) ? shuffleArray(terrain) : terrain;
		setTerrain(numArray, hexArray);
	}
}

/**
 * Randomize array element order in-place.
 * Using Fisher-Yates shuffle algorithm.
 */
function shuffleArray(array) {
    for (var i = array.length - 1; i > 0; i--) {
        var j = Math.floor(Math.random() * (i + 1));
        var temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    return array;
}

function setTerrain(numArray, hexArray){
	var numCount = 0;
	var hexCount = 0;
	console.log(hexArray);

	//TODO modify function so that it reads hex terrains from db rather than array

	$('.hex-row').each(function(i, obj){
		var size = $(this).children('.hex').length;
		$(this).children('.hex').each(function(j, hex){
			if(i == 4 || i == 6 || i == 8){
				$(this).addClass(hexArray[hexCount]);
				hexCount++;
			}
			else if(j > 0 && j != size-1){
				$(this).addClass(hexArray[hexCount]);
				hexCount++;
			}
			else{
				$(this).addClass("sea");
			}
		});
	});
}