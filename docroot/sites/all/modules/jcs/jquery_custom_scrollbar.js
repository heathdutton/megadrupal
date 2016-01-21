/**
 * @file
 * JQuery Custom Scrollbar initialisation.
 */

(function ($) {
  Drupal.behaviors.jQueryCustomScrollbar = {
    attach: function (context, settings) {
      $(settings.jqueryCustomScrollbar.elements).each(function() {
        // Apply css skins.
        $(this).addClass(settings.jqueryCustomScrollbar.skin);
        // Apply scrollbars.
        $(this).customScrollbar();
      });
    }
  };
}(jQuery));
