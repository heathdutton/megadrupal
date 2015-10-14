/**
 * @file
 * Contains js functions for Scroll Button module.
 */

(function ($) {
  /**
   * Scroll Button js functions.
   */
  Drupal.behaviors.ScrollButton = {
    /**
     * Add js events to scroll buttons.
     */
    attach: function(context, settings) {
      var current = this,
        position = settings.scroll_button.position,
        top = $('#scroll_top');

      // Add hover event for changing background color.
      $('.block-scroll-button', context).hover(
        function() {
          $('.scroll-button', $(this)).css("background-color", settings.scroll_button.color['hv']);
        }, function() {
          $('.scroll-button', $(this)).css("background-color", settings.scroll_button.color['bg']);
        }
      );

      if (position == 'float') {
        var bot = $('#scroll_bot'),
          buttons  = settings.scroll_button.buttons,
          distance = settings.scroll_button.distance,
          speed = current.getSpeed(buttons);

        // Add click event to bottom button.
        bot.unbind('click.bot').bind('click.bot', {
          dest: $(document).height(),
          settings: settings.scroll_button
        }, current.scrollTo);

        $(document).scroll(function() {
          if ($(this).scrollTop() >= distance) {
            top.fadeIn();
            bot.fadeOut(speed);
          }
          else {
            top.fadeOut(speed);
            bot.fadeIn();
          }
        });
      }

      // Add click event to top button.
      top.unbind('click.top').bind('click.top', {
        dest: 0,
        settings: settings.scroll_button
      }, current.scrollTo);
    },

    /**
     * Animate scrolling on the top or to the bottom of the page.
     */
    scrollTo: function(event) {
      var speed = event.data.settings.speed;
      speed = (speed == 'extra' ? parseInt(event.data.settings.duration) : speed);
      $('html, body').animate({scrollTop: event.data.dest}, speed);
    },

    /**
     * Get speed for Fadeout() in dependence on amount of enabled buttons.
     */
    getSpeed: function(obj) {
      var size = 0;
      for (var key in obj) {
        if (obj.hasOwnProperty(key) && obj[key] == 1) {
          ++size;
        }
      }
      return size > 1 ? 100 : 400;
    }
  };
})(jQuery);
