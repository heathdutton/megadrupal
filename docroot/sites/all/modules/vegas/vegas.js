/**
 * @file
 * Vegas jQuery Plugin Drupal Integration.
 */

(function ($) {

/**
 * Drupal Disqus behavior.
 */
Drupal.behaviors.vegas = {
  attach: function (context, settings) {
    $('body').once('vegas', function() {
      var vegas = settings['vegas'] || [];
      // Apply the whole slideshow.
      if (vegas['backgrounds'] || false) {
        $.vegas('slideshow', {
          backgrounds: vegas['backgrounds'],
          delay: vegas['delay']
        });

        // Apply the overlay if provided.
        if (vegas['overlay'] || false) {
          $.vegas('overlay', {
            src: vegas['overlay']
          });
        }
      }
    });
  }
};

})(jQuery);
