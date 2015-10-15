/**
 * @file
 * A JavaScript file that styles the page with bootstrap classes.
 *
 * @see sass/styles.scss for more info
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

// To understand behaviors, see https://drupal.org/node/756722#behaviors
Drupal.behaviors.cmsCoreAdmin = {
  attach: function(context, settings) {
  	$( ".theme-grid" ).each(function( index ) {
  		var fullHeight = $(this).height();
  		$(this).attr('data-full-height', fullHeight);
		  $(this).height(310);
		});

	  $('.theme-grid-expand').click(function() {
  		var fullHeight = $(this).parent().parent().attr('data-full-height');
	  	$(this).parent().parent().animate({ height: fullHeight }, 600);
	  	return false;
		});
  }
};
})(jQuery, Drupal, this, this.document);

