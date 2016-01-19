(function($) {
  Drupal.behaviors.slidedeck = {
    attach: function(context) {
        jQuery(".slidedeck").slidedeck();
      }
    }
  })(jQuery);
