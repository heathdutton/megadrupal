jQuery(document).ready(function(){
  var answer_limit = Drupal.settings['mark_word']['answer_limit'];
  var admin_answers = Drupal.settings['mark_word']['admin_answers'];
  var question = jQuery('#mw-question-select');
  var q_text = question.html();
  var new_text = q_text.replace(/\s/g, '</span> <span>');
  question.html('<span>' + new_text + '</span>');
  // Answering page - matched answer
  var user_answered = jQuery('#edit-tries-mark-answer').val();
  if (user_answered) {
    var answer_array = user_answered.split(',');
    jQuery("#mw-question-select span").each(function(index) {
      var span_text = jQuery(this).text();
      var is_present_array = jQuery.inArray(span_text, answer_array);
      if (is_present_array != -1) {
        jQuery(this).addClass('selected');
      }
    });
  }
  //Text highlight
  var current_length = 0;
  question.find('span').live('click', function(e) {
    var selecteds = '';

    //count current selected items
    current_length = question.children('span.selected').length + 1;
    //console.log(jQuery(this).html());
    if (current_length > answer_limit) {
      //jQuery(this).toggleClass('selected');
      alert (Drupal.t('You have already marked the allowed number of words'));
      return true;
    }
    // check correct answer and add class as correct
    var correct_ans = jQuery.inArray(jQuery(this).html(), admin_answers);
    if (correct_ans != -1) {
      console.log(current_length);
      jQuery(this).addClass('correct');
      //
      jQuery("#circle-"+current_length).addClass('circle-color-correct');
    }
    else {
      jQuery("#circle-"+current_length).addClass('circle-color-wrong');
    }

    jQuery(this).addClass('selected');
    question.find('.selected').each(function(i,elem){
        selecteds += jQuery(this).html().replace(/\W/g, '') + ',';
    });
    jQuery('#edit-tries-mark-answer').val(selecteds);
  });
});
