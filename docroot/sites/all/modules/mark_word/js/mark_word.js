var question = '';
var answer = '';

function copy_text(){
  var q_text = question.val();
  var new_text = q_text.replace(/\s/g, '</span> <span>');
  answer.html('<span>' + new_text + '</span>');
  // Question page - matched answer
  var user_answered = jQuery('#edit-field-mark-word-answer-und-0-value').val();
  if (user_answered) {
    var answer_array = user_answered.split(',');
    jQuery("#mw-answer-select span").each(function(index) {
      var span_text = jQuery(this).text();
      var is_present_array = jQuery.inArray(span_text, answer_array);
      if (is_present_array != -1) {
        jQuery(this).addClass('selected');
      }
    });
  }
}

jQuery(document).ready(function(){
  question = jQuery('#edit-body-und-0-value');
  answer = jQuery('#mw-answer-select');
  var category = jQuery('#edit-field-mark-word-type-und');
  //Copy text on page load
  copy_text();

  //Keyup event handler
  category.change(function(){
    copy_text();
  });

  //Text highlight
  answer.find('span').live('click', function(e) {
    var selecteds = '';
    jQuery(this).toggleClass('selected');
    answer.find('.selected').each(function(i,elem){
        selecteds += jQuery(this).html().replace(/\W/g, '') + ',';
    });
    jQuery('#edit-field-mark-word-answer-und-0-value').val(selecteds);
  });
});
