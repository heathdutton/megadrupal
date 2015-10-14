jQuery(document).ready(function() {

	jQuery('#main-menu ul.menu').mobileMenu();
	

/* Navigation */
	jQuery('#main-menu ul.menu').superfish({ 
		delay:       500,								// 0.1 second delay on mouseout 
		animation:   {opacity:'show',height:'show'},	// fade-in and slide-down animation 
		dropShadows: true								// disable drop shadows 
	});	

});