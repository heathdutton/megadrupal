(function ($) {
  
  /**
   * @file Javascript behaviors for jqzoom module
   */

 
  Drupal.jqzoom = Drupal.jqzoom || {};
  
  Drupal.behaviors.jqzoom = {
    attach: function (context, settings) {
      $('a.jqzoom-image-link', context).jqzoom(settings.jqzoom);     
    }
  };

})(jQuery);
