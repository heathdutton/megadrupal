/*=======================================
 Load
=========================================*/
jQuery(document).ready(function() {
/*=======================================
 Full Screen Header
=========================================*/
function fullscreen(){
	jQuery('#header').css({
		height: jQuery(window).height()
	});
}
fullscreen();
jQuery(window).resize(function() {
	fullscreen();         
});
/*=======================================
 Slider
=========================================*/
jQuery("#js-rotating").Morphist({
	animateIn: 'bounceInDown',
	animateOut: 'zoomOutDown',
	speed: 5000,
});
var windowsheight = jQuery(window).height();
jQuery("#js-rotating").css('margin-top',windowsheight/4+'px');
jQuery(".arrow-down").click(function() {
	jQuery('html, body').animate({
		scrollTop: jQuery("#home-content").offset().top
	}, 2000);
});
/*=======================================
 Set minimum height for content
=========================================*/
var getheight = jQuery( window ).height();
var setheight = getheight - 400;

jQuery("#node-container").css('min-height', setheight);
/*=======================================
Search Form
=========================================*/
jQuery(".form-item-search-block-form .form-text").attr("placeholder", "Search here...");

});