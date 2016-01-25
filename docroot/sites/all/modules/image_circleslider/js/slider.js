/**
 * @file
 * Contains the style plugin.
 */

(function($, Drupal, window, document, undefined) {
  $(document).ready(function() {
    $("#circleslider3").tinycircleslider({
      dotsSnap: true,
      radius: 182,
      dotsHide: false,
      interval: true
    });
  });
})(jQuery, Drupal, this, this.document);
