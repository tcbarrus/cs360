Template.loginDialog.events({
	'submit #login-form' : function(e, t){
		e.preventDefault();

		var username = e.target.loginUsername.value;
		var password = e.target.loginPassword.value;

		Meteor.loginWithPassword(username, password, function(err){
			if(err){
				alert("Invalid login credentials.");
			}
		});
		return false;
	},
	'submit #register-form' : function(e, t){
		e.preventDefault();

		var username = e.target.registerUsername.value;
		var password = e.target.registerPassword.value;
		var verify = e.target.registerVerify.value;

		//Validate matching passwords
		if(password != verify)
			return false;

		Accounts.createUser({ username: username, password: password }, function(err){
			if(err)
				alert("There was an error in creating your account.");
			//TODO: login user if meteor doesn't do it automatically
		});
		return false;
	}
});