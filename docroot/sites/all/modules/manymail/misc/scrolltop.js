(function ($) {

/**
 * Scrolls the window to the top to show errors.
 */
Drupal.behaviors.ManyMailScrollTop = {
  attach: function (context, settings) {
    if (settings.ManyMail.sendError) {
      window.scrollTo(0, 0);
    }
  }
};

})(jQuery);
