Meteor.methods({
	createResourceCardStack: function(brick, wood, sheep, wheat, ore){
		return resourceCards.insert({
			brick: brick,
			wood: wood,
			sheep: sheep,
			wheat: wheat,
			ore: ore
		});
	},
	deleteResourceCardStack: function(stackId){
		resourceCards.remove(stackId);
	}
});