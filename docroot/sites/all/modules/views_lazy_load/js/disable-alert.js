/**
 * @file
 * Disable Alert.
 */
(function($) {

  /**
   * This is an agressive approach to disable cores alert boxes that often occur
   * on AJAX errors which are easily trigged by navigating away from a page
   * while an AJAX requests is loading.
   */
  Drupal.behaviors.views_lazy_load_disable_alert = {
    attach: function(context, settings) {
      // Backup the alert function for people who really want it.
      window.real_alert = window.alert;
      window.alert = function(input) {
        if (window.console.log) {
          console.log(input);
        }
      };
    }
  }
})(jQuery);
