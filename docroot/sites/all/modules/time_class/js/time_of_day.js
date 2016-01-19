/**
 * @file
 * Behaviors for time class form.
 *
 * This code adds time_of_day class to the body tag.
 */

(function ($) {
  "use strict";

  Drupal.behaviors.time_of_day = {
    attach: function (context, settings) {
      var date = new Date();
      var hour = date.getHours();
      var bodyClass = '';

      if (hour < 12) {
        bodyClass = 'morning';
      }
      else if (hour >= 12 && hour < 18) {
        bodyClass = 'afternoon';
      }
      else if (hour >= 18 && hour < 24) {
        bodyClass = 'evening';
      }
      else if (hour >= 24) {
        bodyClass = 'night';
      }

      $('body').addClass(bodyClass);
    }
  };
})(jQuery);
