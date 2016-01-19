(function($) {
  Drupal.behaviors.pinterest_hover = {
    attach: function (context, settings) {
      var excluded = Drupal.settings.pinterest_hover.excluded;
      
      // Disable the Pin It button on all items specified by the selectors
      for (var i = 0; i < excluded.length; i++) {
        $(excluded[i]).attr('data-pin-no-hover', 'true');
      }
    }
  };
})(jQuery);