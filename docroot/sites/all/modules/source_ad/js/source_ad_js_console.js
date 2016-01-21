/**
 * @file
 * Adds ad to a javascript console.
 */

(function ($) {
  Drupal.behaviors.source_ad_js_console = {
    attach: function(context, settings) {
      if (typeof console == 'object' && typeof console.log != "undefined") {
        console.log(Drupal.settings.source_ad.javascript_console_message);
      }
    }
  };
})(jQuery);
