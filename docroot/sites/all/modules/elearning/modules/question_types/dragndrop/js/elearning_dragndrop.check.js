/**
 * @file
 * Adds javascript behaviour to the question type.
 */

 (function($) {
  Drupal.behaviors.elearningDragnDropCheck = {
    attach: function(context, settings) {
      $('.elearning-question-elearning-dragndrop', context).once('dragndrop-feedback').each(function(index, question){
        var question_id = $(question).attr('id');
        $(question).find('input[type="submit"]').once('dragndrop-submit').on('mousedown', function(){
         var answer = 'correct';

         $(this).parents('.entity-elearning-question').find('input[type="text"]').each(function(textIndex, textInput){
          var inputString = $(textInput).val();
          var inputValue = parseInt(inputString, 10 );
          var correctValues = settings['elearning_dragndrop'][question_id];
          $.each(correctValues, function( correctValueIndex, correctValue ) {
            if (parseInt(correctValue['value'],10) === textIndex + 1) {
              answerValue = correctValueIndex;
            }
          });
          if (answerValue != inputValue) {
            answer = 'incorrect';
          }
        });
         Drupal.theme('elearningCheckAnswerImmediately',answer, question);
       });
      });
    }
  };
})(jQuery);
