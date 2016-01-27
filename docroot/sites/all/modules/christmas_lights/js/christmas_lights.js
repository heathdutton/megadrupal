(function ($) {
  Drupal.behaviors.christmasLights = {
    attach: function(context, settings) {
      var value = 0;
      setInterval(function() {
        $('#christmas-lights', context).css('backgroundPosition', '0 ' + value + 'px');
      value = value + 36;
      }, 200);
    }
  }
})(jQuery);
