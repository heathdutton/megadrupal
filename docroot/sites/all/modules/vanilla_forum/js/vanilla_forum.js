/**
 * @file
 * vanilla_forum.js file.
 *
 * Drupal side JS additions/tweaks to Vanilla Forum functionality.
 */

(function ($) {
  Drupal.behaviors.vanillaGeneral = {
    attach: function (context, settings) {
      // Remove the target attribute from the Vanilla Forum menu link
      // to prevent 'target="_blank"' behavior.
      $('li a[href*="forum."]').removeAttr("target");
    }
  };
})(jQuery);
