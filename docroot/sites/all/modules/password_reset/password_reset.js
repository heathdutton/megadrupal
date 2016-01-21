(function ($) {

Drupal.behaviors.password_reset = {
  attach: function() {
    var f = $('.password-reset-question-set');
    $('#edit-password-reset-question', f).attr('disabled', 'disabled');
    var a = $('.form-item-password-reset-answer', f)
      .hide()
      .after('<div id="password-reset-change">' + Drupal.t('Your answer for the chosen question has <strong>already</strong> been saved. <a href="#">Click here</a> if you would like to change the question or your answer.') + '</div>')
      .next('#password-reset-change')
      .children('a')
      .click(function(e) {
        $('#edit-password-reset-question', f).removeAttr('disabled');
        $('.form-item-password-reset-answer', f).show('slow');
        $(this).parent().hide();
        e.preventDefault();
      });
    
    // If there's an answer validation error, then unhide the answer field.
    if (Drupal.settings.password_reset) {
      $(a).click();
    }
    
  }
};

}(jQuery));
