$(document).ready(function(){
	$(document).on('input', '#cityInput', function(){
		var url = '../getcity?q='+$('#cityInput').val();
		$.getJSON(url,function(data){
			$("ul").empty();
			$.each(data, function(){
				var $city = $("<li></li>").text(this.city);
				$("#suggestion").append($city);
			});
		});
	});

	$(document).on('click', '#citySubmit', function(){
		var value = $('#cityInput').val();
		$('#cityField').val(value);
		var url = 'https://api.wunderground.com/api/8f637350ea6ec6af/geolookup/conditions/q/UT/' + value + '.json'; 
		$.ajax({
			url: url,
			dataType: 'jsonp',
			success: function(data){
				console.log(data['location']['city']);
				var $city = $('<li></li>').text('Location: ' + data['location']['city']);
				var $temp = $('<li></li>').text('Temperature: ' + data['current_observation']['temperature_string']);
				var $weather = $('<li></li>').text('Weather: ' + data['current_observation']['weather']);
				$('#forecast').append($city);
				$('#forecast').append($temp);
				$('#forecast').append($weather);
			}
		});
	});

	$(document).on('click', '#catSubmit', function(){
		$.ajax({
			url: 'http://thecatapi.com/api/images/get?format=xml&results_per_page=20',
			type: 'GET',
			dataType: 'xml',
			success: function(data){
				$('#movieInfo').empty();
				var imgUrl = $(data).find("url");
				var $img = $('<img/>', {src: imgUrl[0].innerHTML});
				var $title = $('<h2></h2>').text('Cat Pic of the Day');
				$('#movieInfo').append($title);
				$('#movieInfo').append($img);
			},
			error: function(err){
				console.log(err);
			}
		})
	});
});
