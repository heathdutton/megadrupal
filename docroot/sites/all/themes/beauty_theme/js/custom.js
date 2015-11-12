jQuery(document).ready(function($){ // START

  // Superfish
  $('#main-menu ul.menu').superfish({
    delay: 500,
    animation: {opacity:'show',height:'show'},  // fade-in and slide-down animation 
    speed: 'fast',  // faster animation speed 
    autoArrows: true,  // disable drop shadows 
    dropShadows: false  // disable drop shadows 
  });


  // Mobile-view navigation
	// Create the dropdown base
	$("<select class=\"mobile-nav\" />").appendTo("#navigation");
	
	// Create default option "Go to..."
	$("<option />", {
	   "selected": "selected",
	   "value"   : "",
	   "text"    : "Navigate to..."
	}).appendTo("#navigation select");
	
	// Populate dropdown with menu items
	$("#navigation a").each(function() {
		var el = $(this);
		$("<option />", {
			"value"   : el.attr("href"),
			"text"    : el.text()
		}).appendTo("#navigation select");
	});
	
	$("#navigation select").change(function() {
		window.location = $(this).find("option:selected").val();
	});


}); // END