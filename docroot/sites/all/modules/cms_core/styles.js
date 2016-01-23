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
Drupal.behaviors.cmsCore = {
  attach: function(context, settings) {
  	$('.node-blog.node-teaser .content:has(".field-blog-image")')
  		.wrapInner("<div class='row'></div>")
  		.find('.field-blog-image')
  		.addClass('col-sm-6').find('+.field-body')
  		.addClass('col-sm-6');
		$('.field-portfolio-images')
			.wrapInner("<div class='row'></div>")
			.find('img')
			.wrap('<div class="col-md-8">')
			.parent()
			.find('+blockquote')
			.wrap('<div class="col-md-4">');
  }
};
})(jQuery, Drupal, this, this.document);
