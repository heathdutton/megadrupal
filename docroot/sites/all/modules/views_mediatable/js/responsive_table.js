/**
 * @file
 * Enables the MediaTable plugin on tables with the class ".responsive-table".
 */

 (function($) {
  Drupal.behaviors.views_mediatable = {
    attach: function (context, settings) {
      $('table.responsive-table').mediaTable();
    }
  };
})(jQuery);