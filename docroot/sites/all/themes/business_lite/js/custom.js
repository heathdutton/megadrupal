jQuery(document).ready(function() {

	jQuery('#nav ul.menu').mobileMenu();


/* Navigation */
	jQuery('#nav ul.menu').superfish({ 
		delay:       500,								// 0.1 second delay on mouseout 
		animation:   {opacity:'show',height:'show'},	// fade-in and slide-down animation 
		dropShadows: true								// disable drop shadows 
	});	

});
