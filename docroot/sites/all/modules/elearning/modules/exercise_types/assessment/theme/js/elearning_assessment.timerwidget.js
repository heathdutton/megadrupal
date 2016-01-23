/**
 * @file
 *   Timer widget behavior.
 *
 * @todo put this in a theme function.
 */

(function($) {
  Drupal.behaviors.elearningAssessmentTimerWidget = {
    attach: function(context, settings) {
      var seconds_used = Drupal.settings.elearning_assessment.seconds_used;
      var seconds = Drupal.settings.elearning_assessment.seconds;
      window.setInterval(function() {
        seconds_used += 1;
        if (seconds_used < seconds) {
          var proportion = seconds_used / seconds;
          $('.assessment-timer').width(proportion * 100 + '%');
        } else {
          $('.assessment-timer').width('100%');
        }
      }, 1000);
    }
  };

})(jQuery);
