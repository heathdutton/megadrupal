(function($) {
  Drupal.behaviors.setupSystemTheme = {
    attach: function(context) {
      // Create and pause jQuery Cycle object.
      $('#edit-theme').addClass('jquery-cycle').cycle({
        after: function(currSlideElement, nextSlideElement, options, forwardFlag) {
          $('#edit-theme input[type="radio"]').each(function() {
            if ($(this).parent().css('display') == 'block') {
              $(this).trigger('click');
            }
          });
        },
        fx: 'scrollHorz',
        speed: 250
      }).cycle('pause');

      // Attach navigaton controls.
      $('<div id="cycle-nav"><a href="#" id="cycle-prev"></a><a href="#" id="cycle-next"></a></div>').insertAfter('#edit-theme');

      // Click behaviour for Previous navigation control.
      $('#cycle-prev').click(function() {
        $('#edit-theme').cycle('prev');
        return false;
      });

      // Click behaviour for Next navigation control.
      $('#cycle-next').click(function() {
        $('#edit-theme').cycle('next');
        return false;
      });
    }
  }
})(jQuery);
