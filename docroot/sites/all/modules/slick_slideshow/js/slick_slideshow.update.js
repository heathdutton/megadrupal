/**
 * @file
 * Adds a behavior for updating Slick slideshows.
 */
(function($) {
  // Store slideshow settings.
  var slickSlideshowSettingsUpdate = Drupal.settings.slickSlideshowSettingsUpdate;
  var target = '';

  // Initialize the slick sliders.
  Drupal.behaviors.slickSlideshowUpdateBehavior = {
    attach: function (context, settings) {
      $.each(slickSlideshowSettingsUpdate, function(fieldSelector, settings) {
        // Prepare the Slick element.
        var field = $(fieldSelector, context);
        var fieldPrepare = slickSlideshowPrepare(field, context, 'slickSlideshowUpdateBehavior', fieldSelector, settings);
        var fieldSettings = fieldPrepare['fieldSettings'];
        var target = fieldPrepare['target'];

        // Run through the fieldSettings and update them on the target.
        for (var key in fieldSettings) {
          // Update option.
          try {
            if (key === 'responsive') {
              $(target).slickSetOption(key, fieldSettings[key], true);
            } else {
              $(target).slickSetOption(key, fieldSettings[key], false);
            }
          }
          catch(err) {
            throw new Error('The Slick slideshow library is not installed or is out of date. Please download the latest library from http://kenwheeler.github.io/slick/ and install it in your libraries directory.');
          }
        }
      });
    }
  };
}(jQuery));
