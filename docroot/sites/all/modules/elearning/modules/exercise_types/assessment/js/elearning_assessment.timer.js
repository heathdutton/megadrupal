/**
 * @file
 * Behaviour for the timer.
 */

(function($) {
  Drupal.behaviors.elearningAssessmentTimer = {
    attach: function(context, settings) {
     var seconds_used = Drupal.settings.elearning_assessment.seconds_used;
      var seconds = Drupal.settings.elearning_assessment.seconds;
      var seconds_resting = seconds - seconds_used;
      $(window).load(function() {
        if (seconds !== 0) {
          setTimeout(function() {
            //disable input for questions.
           $('#elearning-exercise-formatter-form .entity-elearning-question input').attr("disabled", true);
            $('body').addClass('time-up');
          }, seconds_resting * 1000);
        }
      });

      // Since the questions won't be submitted if disabled we need to
      // undo that upon click.
      $('#elearning-exercise-formatter-form .form-submit').click(function() {
        $('#elearning-exercise-formatter-form .entity-elearning-question input').attr("disabled", false);
      });
    }
  };
})(jQuery);
