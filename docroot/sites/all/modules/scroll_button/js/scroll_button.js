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
        options = settings.scroll_button,
        top = $('#scroll_top'),
        bot = $('#scroll_bot');

      // Add hover event for changing background color.
      current.setButtonBgColor(options, context);

      if (options.position == 'float') {
        // Add click event to the bottom button.
        current.addClickEvent(options, $(document).height(), 'bot', bot, current.scrollTo);
        // Prevent the overlaying when buttons display in the same time.
        current.preventOverlaying(options, top, bot);
        // Handle the visibility of buttons.
        current.visibilityButtonsHandler(top, bot, options, current.getFadeSpeed(options));
      }

      // Add click event to the top button.
      current.addClickEvent(options, 0, 'top', top, current.scrollTo);
    },
    /**
     * Adds background colors on hover event for button.
     */
    setButtonBgColor: function(options, context) {
      // Add hover event for changing background color.
      $('.block-scroll-button', context).hover(
        function() {
          $('.scroll-button', $(this)).css("background-color", options.color['hv']);
        }, function() {
          $('.scroll-button', $(this)).css("background-color", options.color['bg']);
        }
      );
    },
    /**
     * Adds click event to button.
     *
     * @param options
     *   The object of scroll buttons params.
     * @param dest
     *   The destination of scrolling.
     * @param type
     *   Type of button.
     * @param elem
     *   The button object.
     * @param func
     *   The scroll function responsible for animation.
     */
    addClickEvent: function(options, dest, type, elem, func) {
      var parent = elem.parent();
      elem.unbind('click.' + type).bind('click.' + type, {
        dest: dest,
        settings: options,
        parent: parent
      }, func);
    },
    /**
     * Hides and shows the buttons depends on the page scrolling position.
     *
     * @param top
     *   The top button object.
     * @param bot
     *   The bottom button object.
     * @param options
     *   The object of scroll buttons params.
     * @param speed
     *   Speed of fade effect.
     */
    visibilityButtonsHandler: function(top, bot, options, speed) {
      if (!top.length || !bot.length) {
        return;
      }

      $(document).scroll(function() {
        if ($(this).scrollTop() >= options.distance) {
          top.fadeIn();
          bot.fadeOut(speed);
        }
        else {
          top.fadeOut(speed);
          bot.fadeIn();
        }
      });
    },
    /**
     * Animate scrolling on the top or to the bottom of the page.
     */
    scrollTo: function(event) {
      var speed = event.data.settings.speed,
        parent  = event.data.parent;

      // Prevent multiple clicks during animation.
      if (parent.attr('animated')) {
        return;
      }

      // Add the attribute "animated" indicates animating is working.
      parent.attr('animated', 'animated');

      speed = (speed == 'extra' ? parseInt(event.data.settings.duration) : speed);
      $('html, body').animate({scrollTop: event.data.dest}, {
        duration: speed,
        complete: function() {
          // Remove the attribute "animated".
          parent.removeAttr('animated');
        }
      });
    },
    /**
     * Get speed for Fade effect in dependence on amount of enabled buttons.
     */
    getFadeSpeed: function(options) {
      var size = 0,
        obj = options.buttons;

      for (var key in obj) {
        if (obj.hasOwnProperty(key) && obj[key] == 1) {
          ++size;
        }
      }
      return size > 1 ? 100 : 400;
    },
    /**
     * Prevents the overlaying of buttons.
     */
    preventOverlaying: function(options, top, bot) {
      if (options.always_show && top.length && bot.length) {
        $(document).scrollTop() >= options.distance ? bot.hide() : top.hide();
      }
    }
  };
})(jQuery);
