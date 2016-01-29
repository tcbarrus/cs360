Template.createGame.helpers({
	'showCreateGame' : function(){
		var create = !Session.equals('createGame', undefined);
		return create;
	}
});

Template.createGame.events({
	'click .cancel' : function(){
		Session.set('createGame', undefined);
	},
	'submit #createGame-form' : function(e, t){
		e.preventDefault();

		var title = e.target.gameName.value;
		var num = e.target.randNum.checked;
		var hex = e.target.randHex.checked;
		var port = e.target.randPort.checked;

		//TODO: 
		// Meteor.call("createDevCardStack", 2,2,14,2,5, function(error, result){
		// 	deckId = result;
		// 	Meteor.call("createResourceCardStack", 19,19,19,19,19, function(error, result){
		// 		bankId = result;
		// 		Meteor.call("createGame", title, deckId, bankId, function(error, result){
		// 			gameId = result;
		// 			Meteor.call("createMap", gameId, function(error, result){
		// 				mapId = result;
		// 				// initialize map
		// 				Meteor.map.initializeMap(num,hex,port);
		// 			});
		// 		});
		// 	});
		// });
		Meteor.map.initializeMap(num,hex,port);
		// Create new player document and attach user to game
		// Show color select dialog

		
		//Session.set('createGame', undefined);
		return false;
	}
});