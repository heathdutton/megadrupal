/**
 * @file
 *
 * Drupal integration with the jQuery Raty library.
 *
 */

(function ($) {

  Drupal.behaviors.raty_input = {
    attach: function (context, settings) {
      $('.raty-star-widget', context).once('raty-star-widget', function(){
        var $target = '.' + $(this).attr('id');
        var $score = $(this).parent('.raty-input-wrapper').find('input').val();

        $(this).raty({
          path      : '/' + Drupal.settings.raty_input.raty_path + '/img/',
          cancel    : true,
          cancelHint: '0',
          target    : $target,
          targetKeep: true,
          targetType: 'number',
          number    : function() {
            return $(this).attr('data-number');
          },
          score: function() {
            if ($(this).attr('data-score')) {
              return $(this).attr('data-score');
            }
            if (Drupal.settings.raty_input.score) {
              return Drupal.settings.raty_input.score;
            }
            // Failsafe fallback.
            return $score;
          }
        });

      });
    }
  };

})(jQuery);
