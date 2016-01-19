/**
 * @file
 * RoyalSlider Field.
 */
(function($, Drupal) {
  Drupal.behaviors.royalsliderfield = {
    attach: function(context, settings) {
      if (settings.royalslider) {
        if (settings.royalslider.instances) {
          for (var id in settings.royalslider.instances) {
            var $slider = $('#' + id, context).data('royalSlider');
            if ($slider) {
              $slider.ev.on('rsSlideClick', function(event, originalEvent) {
                $rsContent = $(event.target.currSlide.content[0]);
                if ($rsContent.data('rslink')) {
                  location.href = $rsContent.data('rslink');
                }
              });
            }
          }
        }
      }
    }
  };
})(jQuery, Drupal);
