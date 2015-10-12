
(function ($) {

/**
 * Provides the YoxView Drupal behavior.
 */
Drupal.behaviors.yoxview = {
  attach: function (context, settings) {
    // Apply the YoxView image viewer to the desired elements.
    $('body').once('yoxview').yoxview({
      'thumbnailsOptions': {
        'setTitles': true
      }
    });
  }
};

})(jQuery);
