
/**
 * @file
 * Attaches the behaviors for the Nivo Lightbox module.
 */

(function ($) {
  Drupal.behaviors.nivoLightbox = {
    attach: function (context, settings) {
      // Initialize the lightbox
      for (var instance in Drupal.settings.nivo_lightbox) {
        var settings = Drupal.settings.nivo_lightbox[instance];

        $('a#' + settings.id).nivoLightbox({
          'effect': settings.effect, // The effect to use when showing the lightbox
          'theme': settings.theme, // The lightbox theme to use
          'keyboardNav': settings.keyboardNav, // Enable/Disable keyboard navigation (left/right/escape)
          'errorMessage': settings.errorMessage
        });
      }
    }
  };
}(jQuery));
