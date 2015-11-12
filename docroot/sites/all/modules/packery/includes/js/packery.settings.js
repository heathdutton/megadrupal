/**
 * @file
 * packery.settings.js
 */

(function($) {

  Drupal.behaviors.packery = {
    attach: function(context, settings) {

      for (var packery in settings.packery) {
        var $container = $('#' + packery);
        $container.packery(settings.packery[packery].settings);

        // Provide support for imagesLoaded.
        if (settings.packery[packery].settings.imagesloaded) {
          $container.imagesLoaded(function() {
            $container.packery();
          });
        }
      }
    }
  };

}(jQuery));
