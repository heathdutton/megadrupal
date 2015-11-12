;(function($) {
  /**
   * Automatically submit the hidden form that points to the iframe.
   */
  Drupal.behaviors.commercePayPointIFrame = {
    attach: function (context, settings) {
      if (top.location != location) {
        $('html').hide();
        top.location.href = document.location.href;
      }
    }
  }
})(jQuery);
