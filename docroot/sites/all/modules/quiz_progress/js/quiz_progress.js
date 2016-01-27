(function ($) {
  Drupal.behaviors.quizProgress = {
    attach: function (context, settings) {
      $('.quiz-progress-block-wrapper a').tooltip({ showURL: false });
      $('#block-quiz-progress-quiz-progress-block a.quiz-progress').click(function() {
        var element_id = $(this).attr('id');
        var quiz_jump_id = element_id.replace('quiz-progress-id-', '');
        if (quiz_jump_id > 0) {
          $("#quiz-jumper").val(quiz_jump_id);
          $("#quiz-jumper").change();
        }
      });
    }
  };
})(jQuery);

