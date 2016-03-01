$(document).ready(function(){
	$(document).on('input', '#cityInput', function(){
		var url = "https://students.cs.byu.edu/~clement/CS360/ajax/getcity.cgi?q="+$("#cityInput").val();
		$.getJSON(url,function(data){
			$("ul").empty();
			$.each(data, function(){
				var $city = $("<li></li>").text(this.city);
				$("#suggestion").append($city);
			});
		});
	});

	$(document).on('click', 'button', function(){
		var value = $('#cityInput').val();
		$('#cityField').val(value);
		var url = 'https://api.wunderground.com/api/8f637350ea6ec6af/geolookup/conditions/q/UT/' + value + '.json'; 
		console.log(url);
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

	$(document).on('change', 'select', function(){
		var $id = $('select').val();
		var $url = 'http://imdb.wemakesites.net/api/' + $id;
		$.ajax({
			url: $url,
			dataType: 'jsonp',
			success: function(data){
				console.log(data);
				$('#movieInfo').empty();
				var $title = $('<h2></h2>').text($('select option:selected').text());
				var $rating = $('<p></p>').text(data['data']['review']['rating']);
				var $description = $('<p></p>').text(data['data']['description']);
				var $img = $('<img/>', {src: data['data']['image']});
				$('#movieInfo').append($title);
				$('#movieInfo').append($rating);
				$('#movieInfo').append($description);
				$('#movieInfo').append($img);
			}
		});
	});
});