(function($) {
  
  Drupal.behaviors.post = {
    attach: function(context, settings) {
      var content = $('div.region-content');
      var sharebar = $('div.post-vertical');
      var container = $('div.post-container');
      var cont_off = container.offset();
      var lr = Drupal.settings.post_position;
  
      $(window).resize(function() {
        var pos = content.offset();
  
        if (pos.left < 90) {
          sharebar.css('position', 'relative')
            .css('float', lr)
            .css('margin-' + (lr == 'left' ? 'right' : 'left'), '10px')
            .css(lr, '0')
            .css('top', '0');
        }
        else {
          sharebar.css('position', 'fixed')
            .css('float', 'none')
            .css('margin-' + (lr == 'left' ? 'right' : 'left'), '0')
            .css('left', (lr == 'left' ? pos.left - 90 : pos.left + content.width() + 10) + 'px')
            .css('top', (cont_off.top - $(this).scrollTop()) + 'px');
        }
      }).scroll(function() {
        var pos = content.offset();
  
        if (pos.left >= 90) {
          if (cont_off.top < $(this).scrollTop() + 40) {
            sharebar.css('top', '40px');
          }
          else if (cont_off.top > $(this).scrollTop()) {
            sharebar.css('top', (cont_off.top - $(this).scrollTop()) + 'px');
          }
        }
      }).trigger('resize');
    }
  };
})(jQuery);
