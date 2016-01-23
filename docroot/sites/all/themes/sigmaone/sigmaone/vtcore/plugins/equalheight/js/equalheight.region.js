(function($) {
  Drupal.behaviors.regionEqualheight = {
      attach: function(context, settings) {
        var elements = $('.equalheight-region');
        
        elements.regionEqualHeight();
        
        // Equalheight on window resize
        $(window).resize(function() {
          // Reset to avoid too tall bug
          elements.find('.region').css('min-height', 'auto');
          
          // Recalculate the height
          elements.regionEqualHeight();
        });
      }
  };
})(jQuery);
