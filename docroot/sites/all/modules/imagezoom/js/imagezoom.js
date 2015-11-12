(function($) {

  /**
   * Initialize image zoom functionality.
   */
  Drupal.behaviors.imagezoom = {
    attach: function(context, settings) {
      $('.imagezoom-image', context).elevateZoom(settings.imagezoom);
    }
  }

})(jQuery);
