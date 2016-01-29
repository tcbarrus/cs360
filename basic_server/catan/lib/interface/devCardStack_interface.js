Meteor.methods({
	createDevCardStack: function(yearOfPlenty, monopoly, soldier, roadBuilding, monument){
		return devCards.insert({
			yearOfPlenty: yearOfPlenty,
			monopoly: monopoly,
			soldier: soldier,
			roadBuilding: roadBuilding,
			monument: monument
		});
	},
	deleteDevCardStack: function(stackId){
		devCards.remove(stackId);
	}
});