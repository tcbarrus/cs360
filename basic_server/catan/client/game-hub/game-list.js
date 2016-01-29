Template.gameList.events({
	'click button' : function(){
		Session.set('game', this._id);
		//TODO: Query db for player info if they already have game activity for selected game
	}
});