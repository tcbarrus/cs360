$(document).ready(function(){
    $("#serialize").click(function(){
        var myobj = {Name:$("#Name").val(),Comment:$("#Comment").val()};
        jobj = JSON.stringify(myobj);
        $("#json").text(jobj);

	var url = "comment";
	$.ajax({
 		url:url,
 		type: "POST",
 		data: jobj,
 		contentType: "application/json; charset=utf-8",
 		success: function(data,textStatus) {
      			$("#done").html(textStatus);
  		}
	});
    });

    $("#getThem").click(function(){
	$.getJSON('comment', function(data){
		console.log(data);
		var everything = "<ul>";
		for(var comment in data){
			var red = Math.floor(Math.random() * 256);
			var green = Math.floor(Math.random() * 256);
			var blue = Math.floor(Math.random() * 256);
			com = data[comment];
			everything += "<li style='background-color: rgba("+red+","+green+","+blue+", 0.5)'>Name: "+com.Name+" -- Comment: "+com.Comment+"</li>";
		}
		everything += "</ul>";
		$("#comments").html(everything);
	});
    });
});
