/**
 * @file
 * JavaScript for the jQuery Nicescroll module.
 */

(function ($) {

Drupal.behaviors.jqueryNicescroll = {
  attach: function (context, settings) {
    $(settings.jqueryNicescroll.element).niceScroll(settings.jqueryNicescroll.params);
  }
};

})(jQuery);
