/**
 * @file
 * Provide slick example behaviors.
 */

(function ($, Drupal, window) {

  "use strict";

  Drupal.behaviors.slickExample = {
    attach: function(context, settings) {

      $('.slick__slider', context).on('afterChange.example', function(e, slick, currentSlide) {
        if (e.handled !== true) {
          // console.log('slick_example afterChange: ' + currentSlide);
          e.handled = true;
        }
      });
    }
  };

})(jQuery, Drupal, this);
