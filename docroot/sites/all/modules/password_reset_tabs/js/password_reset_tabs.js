/**
 * @file
 * Disable Tab Click.
 */

(function ($) {
  "use strict";
  Drupal.behaviors.TabsClick = {
    attach: function (context) {
      $('.page-password-reset .tabs ul.primary li a').click(false);
      $('.page-password-reset .tabs ul.primary li a').click(function() {return false; });
    }
  };

})(jQuery);
