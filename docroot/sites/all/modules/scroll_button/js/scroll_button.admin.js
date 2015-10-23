/**
 * @file
 * Contains admin js functions for Scroll Button module.
 */

(function ($) {
  /**
   * Scroll Button admin js functions.
   */
  Drupal.behaviors.ScrollButton = {
    // Attach farbtastic to settings page.
    attach: function(context, settings) {
      var current = this;

      $("div.scroll-farbtastic").each(function () {
        var input = $('input.form-text', $(this).parent()),
          farb = $.farbtastic($(this)),
          type = input.attr('id').split('-');

        if (typeof type[2] != 'undefined') {
          var color = settings.scroll_button.color[type[2]];
          farb.setColor(color);
          input.css("background-color", color).val(color);
        }

        farb.linkTo(function(color) {
          input.css({
            'background-color': color,
            color: current.invertColor(color)
          }).val(color);
        });

        // If paste the color directly - set the same color to farb.
        input.unbind().bind('keyup change', function() {
          farb.setColor($(this).val());
        });
      });
    },
    // Invert input color.
    invertColor: function (inputcolor) {
      var color = inputcolor.substring(1);
      color = parseInt(color, 16);
      color = 0xFFFFFF ^ color;
      color = color.toString(16);
      color = ("000000" + color).slice(-6);
      return "#" + color;
    }
  };
})(jQuery);
