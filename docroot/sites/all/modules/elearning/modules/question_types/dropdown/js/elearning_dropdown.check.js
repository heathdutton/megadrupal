/**
 * @file
 * Saves answers to cookies and deletes them upon submit
 */
 (function($) {
  Drupal.behaviors.elearningDropdownCheck = {
    attach: function(context, settings) {
     $('.elearning-question-elearning-dropdown', context).once('dropdown-feedback').each(function(index, question){
        var question_id = $(question).attr('id');
        $(question).find('select').change(function(){
          var answer = 'correct';
          $(this).parents('.entity-elearning-question').find('select').each(function(selectIndex, selectList){
            selectedOption = $(selectList).find('option:selected').text();
           if(selectedOption !== settings.elearning_dropdown[question_id][selectIndex].value && selectedOption !==''){
              answer = 'incorrect';
            }
            if(selectedOption === ''){
              answer = 'not-answered';
            }
          });
          if(answer === 'incorrect'){
            $(question).find('select').val(0);
          }
          Drupal.theme('elearningCheckAnswerImmediately',answer, question);
        });
      });
    }
  };

})(jQuery);
