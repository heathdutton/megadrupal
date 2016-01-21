/**
 * @file
 * Behaviors for time class form.
 *
 * This code initialize datepicker popup for time class settings form.
 */

(function ($) {
  "use strict";

  Drupal.behaviors.time_class = {
    attach: function (context, settings) {
      $('.datepicker', context).datepicker({
        dateFormat: 'dd.mm.yy'
      });
    }
  };
})(jQuery);
