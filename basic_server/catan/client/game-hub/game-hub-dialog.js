Template.gameHub.helpers({
	'games' : function(){
		return games.find();
	},
	'joined' : function(){
		var notInGame = (Session.get('game') === undefined);
		var loggedIn = (Meteor.user() !== null);
		return (notInGame && loggedIn);
	}
});

Template.gameHub.events({
	'click #createGame' : function(){
		Session.set('createGame', true);
	}
});