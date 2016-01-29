Meteor.methods({
	createGame: function(title, deckId, bankId){
		return games.insert({
			title: title,
			deck: deckId,
			bank: bankId
		});
	},
	deleteGame: function(gameId){
		games.remove(gameId);
	}
});