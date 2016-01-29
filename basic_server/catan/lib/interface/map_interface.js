Meteor.methods({
	createMap: function(gameId){
		return maps.insert({
			gameId: gameId
		});
	},
	deleteMap: function(mapId){
		maps.remove(mapId);
	}
});