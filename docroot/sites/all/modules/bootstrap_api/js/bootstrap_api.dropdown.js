/**
 * @file
 * triggers bootstrap api dropdown
 */

(function ($) {
  /**
   * Attaches outline behavior for regions associated with contextual links.
   */
  Drupal.behaviors.bootstrap_api_dropdown = {
    attach: function (context) {

      $('.dropdown-toggle', context).once('bootstrap-api-dropdown', function () {
        $(this).dropdown();
      });
    }
  };
})(jQuery);
