(function ($) {
	// Get vars from d3map_views module settings
	var activePaths = $.parseJSON(Drupal.settings.d3map_views.activePaths);
	var mapPath = Drupal.settings.d3map_views.mapPath;
	var paramName = Drupal.settings.d3map_views.paramName;
	var showTerritories = Drupal.settings.d3map_views.showTerritories;
	var width = Drupal.settings.d3map_views.width,
		height = Drupal.settings.d3map_views.height;
	var stateLabelBoxHeight = 25;
	var stateLabelBoxWidth = 30;

	// Areas that should show up as separate boxes and labels
	var stateLabels = [
		{"state":"Rhode Island", "postal":"RI"},
		{"state":"New Hampshire", "postal":"NH"},
		{"state":"New Jersey", "postal":"NJ"},
		{"state":"Maryland", "postal":"MD"},
		{"state":"Vermont", "postal":"VT"},
		{"state":"Massachusetts", "postal":"MA"},
		{"state":"Connecticut", "postal":"CT"},
		{"state":"Delaware", "postal":"DE"},
		{"state":"District of Columbia", "postal":"DC"}
	];

	// Territories that should show up as separate boxes and labels
	var territories = [
		{"state":"American Samoa", "postal":"AS"},
		{"state":"Micronesia", "postal":"FM"},
		{"state":"Guam", "postal":"GU"},
		{"state":"Marshall Islands", "postal":"MH"},
		{"state":"Northern Mariana Islands", "postal":"MP"},
		{"state":"Palau", "postal":"PW"},
		{"state":"Puerto Rico", "postal":"PR"},
		{"state":"Virgin Islands", "postal":"VI"},
		{"state":"All Federal Government", "postal":"FG"}
	];

	var path = d3.geo.path();

	var svgMap = d3.select("#d3-map").append("svg")
		.attr("width", width + 'px')
		.attr("height", height + 'px');

	// Add state shapes to svg
	d3.json(mapPath, function (data) {
		svgMap.selectAll("path")
			.data(data.features)
			.enter()
			.append('svg:path')
			// Add path classes and active class if data exists for the path
			.attr('class', function (d) { return "us-state " + d.properties.postal + 
				" " + checkActive(d.properties.postal); })
			// Add path id
	        .attr('id', function (d) { return "us-state-" + d.properties.postal; })
			.attr("d", path);

		// Add state shapes to svg
		svgMap.selectAll(".dynamicRects")
			.data(stateLabels)
			.enter()
			.append("rect")
			// Set the horizontal offset for the label box
			.attr("x", function (d, i) { return (width-120) - ((i%2) * 35); })
			// Set the vertical offset for the label box
	 		.attr("y", function (d, i) { return height / 2 - 70 + ((i-(i%2)) * 16); })
	 		// Add path classes and active class if data exists for the path
			.attr('class', function (d) { return "us-state us-state-isolabel " + d.postal + 
				" " + checkActive(d.postal); })
			.attr("width", stateLabelBoxWidth)
			.attr("height", stateLabelBoxHeight);

		if (showTerritories) {
			// Add state shapes to svg
			svgMap.selectAll(".dynamicRects")
				.data(territories)
				.enter()
				.append("rect")
				// Set the horizontal offset for the label box
				//.attr("x", function (d, i) { return (width-120) - ((i%2) * 35); })
				.attr("x", function (d, i) { return (width-300) - ((i-(i%2)) * 18); })
				// Set the vertical offset for the label box
		 		//.attr("y", function (d, i) { return height / 2 + 90 + ((i-(i%2)) * 16); })
		 		.attr("y", function (d, i) { return height / 2 + 190 + ((i%2) * 32); })
		 		// Add path classes and active class if data exists for the path
				.attr('class', function (d) { return "us-state us-state-isolabel us-territory " + d.postal + 
					" " + checkActive(d.postal); })
				.attr("width", stateLabelBoxWidth)
				.attr("height", stateLabelBoxHeight);
		}

		// Add text labels for states
		svgMap.selectAll(".dynamicText")
			.data(data.features)
			.enter()
			.append("text")
			.text(function (d) { return d.properties.postal;})
			.attr("x", function (d) {
				if (d.geometry != null) {
					return path.centroid(d)[0];
				}
				return;
			})
			.attr("y", function (d) {
				if (d.geometry != null) {
					return path.centroid(d)[1];
				}
				return;
			})
			.attr('class', function (d) { return "us-state-label " + checkActive(d.properties.postal); })
			.attr('id', function (d) { return "us-state-label-" + d.properties.postal; })
			.attr('pointer-events', 'none')
			.attr("text-anchor","middle");

		if (showTerritories) {
			// Add text labels for territories
			svgMap.selectAll(".dynamicText")
				.data(territories)
				.enter()
				.append("text")
				.text(function (d) { return d.postal;})
				.attr('class', function (d) { return "us-state-label " + checkActive(d.postal); })
				.attr('id', function (d) { return "us-state-label-" + d.postal; })
				.attr('pointer-events', 'none')
				.attr("text-anchor","middle");

			// Select all state labels that match the map side labels list
			var selector = territories.map(function (d) { return '#us-state-label-' + d.postal; }).join(',');

			// Shift text labels of states to side labels that use them
			var labelShift = svgMap.selectAll(selector)
				// Set the horizontal offset for the label box
				.attr("x", function (d, i) { 
					var labelX = parseInt(svgMap.select(".us-state-isolabel." + d.postal)
							.attr("x")) + stateLabelBoxWidth - 16;
					return labelX + 1;
				})
				// Set the vertical offset for the label box
				.attr("y", function (d, i) { 
					var labelY = parseInt(svgMap.select(".us-state-isolabel." + d.postal)
							.attr("y")) + stateLabelBoxHeight - 8;
					return labelY + 1;
				});
		}

		// Select all state labels that match the map side labels list
		var selector = stateLabels.map(function (d) { return '#us-state-label-' + d.postal; }).join(',');

		// Shift text labels of states to side labels that use them
		var labelShift = svgMap.selectAll(selector)
			// Set the horizontal offset for the label box
			.attr("x", function (d, i) { 
				var labelX = parseInt(svgMap.select(".us-state-isolabel." + d.properties.postal)
						.attr("x")) + stateLabelBoxWidth - 16;
				return labelX + 1;
			})
			// Set the vertical offset for the label box
			.attr("y", function (d, i) { 
				var labelY = parseInt(svgMap.select(".us-state-isolabel." + d.properties.postal)
						.attr("y")) + stateLabelBoxHeight - 8;
				return labelY + 1;
			});

		// listen for events
		svgMap.selectAll('.us-state')
			.on('mouseover', stateMouseover)
			.on('mouseout', stateMouseout)
			.on('click', stateClick);
	});

	// On path pointer hover
	function stateMouseover (d) {
		var state = '';
		try {
			state = d.properties.postal;
		}
		catch (error) {
			state = d.postal;
		}

		// Exit if not an active path
		if(typeof activePaths[state] == 'undefined') {
			return;
		}

		svgMap.selectAll('.us-state.' + state)
			// the '.hover' class is added to this path
			.classed('hover', true);
	}

	// When the cursor leaves the path
	function stateMouseout (d) {
		var state = '';
		try {
			state = d.properties.postal;
		}
		catch (error) {
			state = d.postal;
		}

		// Exit if not an active path
		if(typeof activePaths[state] == 'undefined') {
			return;
		}

		svgMap.selectAll('.us-state.' + state)
			// the '.hover' class is removed from this path
			.classed('hover', false);
	}

	// Pretend we clicked a link instead of an SVG path
	function stateClick (d) {
		var state = '';
		try {
			state = d.properties.postal;
		}
		catch (error) {
			state = d.postal;
		}

		// Exit if not an active path
		if(typeof activePaths[state] == 'undefined') {
			return;
		}

		// The value used by views to represent the exposed field
		var field_value = activePaths[state];

		// replace url var name with the new value and go to the URL
		location.href = updateQueryString(paramName, field_value, null);
	}

	// Check to see if the current path should be active
	function checkActive(path) {
		// Check if path is in activePaths set in the template file
		if(typeof activePaths[path] != 'undefined') {
			// If so this state has data from views and is active
			return 'active';
		}
		return '';
	}

	// Helper function to replace the value of a key in the url with a new one
	function updateQueryString(key, value, url) {
		if (!url) url = location.href;
		var re = new RegExp("([?&])" + key + "=.*?(&|#|$)(.*)", "gi"),
		hash;

		if (re.test(url)) {
			if (typeof value !== 'undefined' && value !== null)
				return url.replace(re, '$1' + key + "=" + value + '$2$3');
			else {
				hash = url.split('#');
				url = hash[0].replace(re, '$1$3').replace(/(&|\?)$/, '');
				if (typeof hash[1] !== 'undefined' && hash[1] !== null) 
					url += '#' + hash[1];
				return url;
			}
		}
		else {
		if (typeof value !== 'undefined' && value !== null) {
			var separator = url.indexOf('?') !== -1 ? '&' : '?';
			hash = url.split('#');
			url = hash[0] + separator + key + '=' + value;
			if (typeof hash[1] !== 'undefined' && hash[1] !== null) 
				url += '#' + hash[1];
			return url;
		}
		else
			return url;
		}
	}
})(jQuery);