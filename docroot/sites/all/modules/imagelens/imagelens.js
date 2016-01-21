(function($) {
  Drupal.behaviors.imageLens = {
    attach: function(context, settings) {
      $('img.imagelens', context).imageLens();
    }
  }
}) (jQuery);