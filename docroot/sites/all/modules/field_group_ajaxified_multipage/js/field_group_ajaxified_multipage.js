(function($){

  /**
   * Scroll to top.
   */
  Drupal.behaviors.scrollTop = {
    attach: function (context, settings) {

      $('html, body').animate({
        scrollTop: $("body").offset().top
      }, 200);

    }
  };
})(jQuery);