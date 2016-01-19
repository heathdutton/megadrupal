/**
 * @file
 * The main javascript file for the slim_time module
 *
 * @ingroup slim_time
 * @{
 */
(function ($) {
  Drupal.behaviors.slim_time = {
    attach: function (context) {
      var options;
      for (var id in Drupal.settings.slimTime) {
        options = Drupal.settings.slimTime[id];

        // Rewrite the fail and pass to account for drupal's error classes.
        options.fail = function(entered, parsed, $element, slimTime) {
          $element.addClass('error');
        }
        options.pass = function(entered, parsed, $element, slimTime) {
          $element
          .val(slimTime.join(parsed))
          .removeClass('error');
        }

        // Instantiate this form element.
        $('#'+ id).slimTime(options);
      }
    }
  }
})(jQuery);
