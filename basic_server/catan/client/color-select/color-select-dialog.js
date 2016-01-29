Template.colorSelect.helpers({
	'showColorSelect' : function(){
		return !(Session.get('game') === undefined);
	}
});

Template.colorSelect.events({
	'click .cancel' : function(){
		Session.set('game', undefined);
	},
	'click .color' : function(e){
		console.log(e.currentTarget.classList[0]);
		//TODO: create player document
	}
});