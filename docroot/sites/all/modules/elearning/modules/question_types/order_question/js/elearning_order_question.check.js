/**
 * @file
 * Saves answers to cookies and deletes them upon submit
 */
 (function($) {
  Drupal.behaviors.elearningOrderQuestionCheck = {
    attach: function(context, settings) {
      $('.elearning-question-elearning-order-question', context).once('order-question-feedback').each(function(index, question){
        var question_id = $(question).attr('id');
          $(question).find('input[type="submit"]').on('mousedown',function(){
          var answer = 'correct';
          $(this).parents('.entity-elearning-question').find('input[type="text"]').each(function(textIndex, textInput){
            var inputValue = $(textInput).val();
            var questionSettings = settings['elearning_order_question'][question_id]['settings'];
            var correctAnswer = settings['elearning_order_question'][question_id]['values'][textIndex]['value'];

            var decordedImputValue = window.atob(inputValue) ;
            if(decordedImputValue != textIndex){
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
