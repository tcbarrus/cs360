Meteor.methods({
	createPlayer: function(gameId, userId, color, resources, devCards, structures){
		return players.insert({
			gameId: gameId,
			userId: userId,
			color: color,
			resourceStack_id: resources,
			devCardStack_id: devCards,
			structures: structures
		});
	},
	deletePlayer: function(playerId){
		players.remove(playerId);
	}
});