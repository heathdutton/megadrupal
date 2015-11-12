(function ($) {

/**
 * Attach handlers to evaluate the strength of any password fields and to check
 * that its confirmation is correct.
 */
Drupal.behaviors.fixcore_reminder = {
  attach: function (context) {
    $('input.password-field', context).once('fixcore_reminder', function () {
      var passwordInput = $(this);
      var outerWrapper = $(this).parent().parent();

      var confirmInput = $('input.password-confirm', outerWrapper);
      var reminder = $('span.fixcore-reminder', outerWrapper);

      var updateReminder = function () {
        if (confirmInput.val() && passwordInput.val() === confirmInput.val()) {
          reminder.css('text-decoration', 'blink');
          reminder.addClass('warning');
        }
        else {
          reminder.css('text-decoration', 'none');
          reminder.removeClass('warning');
        }
      }

      // Monitor keyup and blur events.
      // Blur must be used because a mouse paste does not trigger keyup.
      passwordInput.keyup(updateReminder).focus(updateReminder).blur(updateReminder);
      confirmInput.keyup(updateReminder).blur(updateReminder);
    });
  }
};

})(jQuery);
