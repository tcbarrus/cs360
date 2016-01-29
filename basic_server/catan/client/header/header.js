Template.header.events({
	'click #logout' : function(){
		Meteor.logout(function(){
			Session.keys = {};
		});
		return false;
	}
});