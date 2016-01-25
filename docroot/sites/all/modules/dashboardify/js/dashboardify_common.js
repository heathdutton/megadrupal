/**
 * @file
 * Handles contextual links fixes while js enabled.
 */

(function ($) {

/**
 * Implements Drupal.behaviors for the Dashboardify module.
 */
Drupal.behaviors.dashboardify_common = {
  attach: function (context, settings) {
    $("div.contextual-links-js-disabled")
      .removeClass('contextual-links-js-disabled');
    $("a.contextual-links-trigger-js-disabled")
      .remove();
  },
};

})(jQuery);
