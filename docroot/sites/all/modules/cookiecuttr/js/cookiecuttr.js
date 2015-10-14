(function ($) {
  Drupal.behaviors.cookiePolicy = {
    attach: function (context, settings) {
      $.cookieCuttr(Drupal.settings.cookieCuttr);
    }
  };
})(jQuery);