/**
 * @file
 * JS for Radix Starter.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - http://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

function parallax(){
  var scrolled = $(window).scrollTop();
  $('.background').css('top', -(scrolled * .6) + 'px');
}

$(window).scroll(function(e){
  parallax();
});

})(jQuery, Drupal, this, this.document);
