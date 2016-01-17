/**
 * @file
 *
 * JavaScript should be made compatible with libraries other than jQuery by
 * wrapping it with an "anonymous closure". See:
 * @see  http://drupal.org/node/1446420
 * @see  http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
 */

(function ($, Drupal, window, document, undefined) {

  // Keep all JavaScript in this closure.

  // Begin jQuery.
  $(document).ready(function () {

  });
  // End jQuery.
})(jQuery, Drupal, this, this.document);
