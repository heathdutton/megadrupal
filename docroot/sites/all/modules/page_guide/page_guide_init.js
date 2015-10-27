/**
 * @file
 * Activate the page guide.
 */

(function($) {
  Drupal.behaviors.myBehavior = {
    attach: function (context, settings) {
      tl.pg.init();
    }
  };
})(jQuery);
