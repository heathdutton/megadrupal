/**
 * @file
 * Adds a behavior for initializing the Slick slideshows.
 */
(function($) {
  // Store slideshow settings.
  var slickSlideshowSettings = Drupal.settings.slickSlideshowSettings;
  var target = '';

  // Initialize the slick sliders.
  Drupal.behaviors.slickSlideshowBehavior = {
    attach: function (context, settings) {
      $.each(slickSlideshowSettings, function(fieldSelector, settings) {
        // Prepare the Slick element.
        var field = $(fieldSelector, context);
        var fieldPrepare = slickSlideshowPrepare(field, context, 'slickSlideshowBehavior', fieldSelector, settings);
    
        if (fieldPrepare === undefined)
          return;
    
        var fieldSettings = fieldPrepare['fieldSettings'];
        var target = fieldPrepare['target'];

        // Initialize Slick.
        try {
          $(target).slick(fieldSettings);
        }
        catch(err) {
          throw new Error('The Slick slideshow library is not installed. Please download the library from http://kenwheeler.github.io/slick/ and install it in your libraries directory.');
        }
      });
    }
  };
}(jQuery));
