/**
 * @file
 * Saves answers to cookies and deletes them upon submit
 */
 (function($) {
  Drupal.behaviors.elearningOpenQuestionCheck = {
    attach: function(context, settings) {
      $('.elearning-question-elearning-open-question', context).once('open-question-feedback').each(function(index, question){
        var question_id = $(question).attr('id');
          $(question).find('input[type="submit"]').on('mousedown',function(){
          var answer = 'correct';
          $(this).parents('.entity-elearning-question').find('input[type="text"]').each(function(textIndex, textInput){
            var inputValue = $(textInput).val();
            // parse the value
            var questionSettings = settings['elearning_open_question'][question_id]['settings'];
            var correctAnswer = settings['elearning_open_question'][question_id]['values'][textIndex]['value'];

            if (questionSettings['case_sensitive'] === 0) {
              inputValue = inputValue.toLowerCase();
              correctAnswer = correctAnswer.toLowerCase();
            }

            if (questionSettings['space_sensitive'] === 0) {
             inputValue = inputValue.replace(' ', '');
              correctAnswer = correctAnswer.replace(' ', '');
            }

               if (questionSettings['punctuation_sensitive'] === 0) {
             inputValue = inputValue.replace(/[^\w\s]|_/g, "").replace(/\s+/g, " ");
              correctAnswer = correctAnswer.replace(/[^\w\s]|_/g, "").replace(/\s+/g, " ");
            }
                     if (questionSettings['apostroph_sensitive'] === 0) {
                inputValue = inputValue.replace('´', "'");
              correctAnswer = correctAnswer.replace('´', "'");
            }
            if(inputValue != correctAnswer){
              answer = 'incorrect';
            }
            if(inputValue === ''){
              answer = 'not-answered';
            }
          });
          Drupal.theme('elearningCheckAnswerImmediately',answer, question);
        });
      });
    }
  };

})(jQuery);
