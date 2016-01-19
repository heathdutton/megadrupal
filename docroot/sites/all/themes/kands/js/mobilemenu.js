// We can ignore active-sidebar because we don't have a callout on the right.

var showMenu = function() {
	jQuery('body').toggleClass("active-nav");
	jQuery('.menu-button').toggleClass("active-nav");
}

jQuery(document).ready(function($) {
		// Toggle for nav menu
		$('.menu-button').click(function(e) {
			e.preventDefault();
			showMenu();							
		});	
});