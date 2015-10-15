/**
 * @file
 * Saves answers to cookies and deletes them upon submit.
 */
 (function($) {
  Drupal.behaviors.elearningMultipleChoiceCheck = {
    attach: function(context, settings) {
      $('.elearning-question-elearning-multiplechoice', context).once('mc-feedback').each(function(index, question){
        var question_id = $(question).attr('id');
        if(settings['elearning_multiplechoice'][question_id]['settings']['multiple_answers'] === 0){
          $(question).find('input[type="radio"]').change(function(){

            var optionIndex = $(this).parents('.form-item').index();
            if(settings['elearning_multiplechoice'][question_id]['values'][optionIndex]['value'] == 1){
              answer = 'correct';
            } else {
              answer = 'incorrect';
            }
            Drupal.theme('elearningCheckAnswerImmediately',answer, question);
          });
        }
        if(settings['elearning_multiplechoice'][question_id]['settings']['multiple_answers'] === 1){
          $(question).find('input[type="submit"]').on('mousedown', function(){
            var all_correct = settings['elearning_multiplechoice'][question_id]['settings']['all_correct'];
            var correct = 'not-answered';
            var optionIndex = $(this).parents('.form-item').index();
            answer = 'correct';
            if(all_correct === 1){
              $(this).parents(question).find('input[type="checkbox"]').each(function(checkboxIndex, checkboxElement){
                console.log(settings['elearning_multiplechoice'][question_id]['values'][checkboxIndex]['value']);
                console.log($(checkboxElement).val());
                if($(checkboxElement).is(':checked') != settings['elearning_multiplechoice'][question_id]['values'][checkboxIndex]['value']){
                  answer = 'incorrect';
                }
              });
            } else {
              answer = 'incorrect';
              $(this).parents(question).find('input[type="checkbox"]').each(function(checkboxIndex, checkboxElement){
                if($(checkboxElement).is(':checked') == 1 && $(checkboxElement).val() == settings['elearning_multiplechoice'][question_id]['values'][checkboxIndex]['value']){
                  answer = 'correct';
                }
              });
                $(this).parents(question).find('input[type="checkbox"]').each(function(checkboxIndex, checkboxElement){
                if($(checkboxElement).is(':checked') == 1 && $(checkboxElement).val() != settings['elearning_multiplechoice'][question_id]['values'][checkboxIndex]['value']){
                  answer = 'incorrect';
                }
              });
            }
            Drupal.theme('elearningCheckAnswerImmediately',answer, question);
          });
        }
      });
    }
  };
})(jQuery);
