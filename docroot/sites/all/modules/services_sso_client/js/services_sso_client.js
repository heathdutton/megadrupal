(function ($) {
  Drupal.behaviors.services_sso_client = {
    attach: function(context, settings) {
      // Attach colorbox to edit links. This falls back gracefully without JS.
      jQuery('a.load-modal', context).each(function(i) {
        jQuery(this).colorbox({
          speed: 500,
          iframe: true,
          width: 1000,
          height: '80%',

          onClosed: function() {
            // Force a page refresh after colorbox has been closed.
            window.location.reload(false);
          }
        });
      });

      // Reproduce the password repeating function on the user registration form for emails as well.
      // Trying to follow user.js style in the core user module.
      $('.confirm-email-wrapper .form-item-mail input', context).once('email', function () {
        var translate = settings.services_sso_client.email_translate;

        var emailInput = $(this);
        var innerWrapper = $(this).parent();
        var outerWrapper = $(this).parent().parent();

        innerWrapper.addClass('email-parent');

        var confirmInput = $('.form-item-mail-confirm input', outerWrapper);
        confirmInput.parent().prepend('<div class="email-confirm">' + translate['confirmTitle'] + '<span></span></div>').addClass('confirm-parent');
        var confirmResult = $('div.email-confirm', outerWrapper);
        var confirmChild = $('span', confirmResult);

        // Check that email and confirmation inputs match.
        var emailCheckMatch = function () {

          if (confirmInput.val()) {
            var success = emailInput.val() === confirmInput.val();

            // Show the confirm result.
            confirmResult.css({ visibility: 'visible' });

            // Remove the previous styling if any exists.
            if (this.confirmClass) {
              confirmChild.removeClass(this.confirmClass);
            }

            // Fill in the success message and set the class accordingly.
            var confirmClass = success ? 'ok' : 'error';
            confirmChild.html(translate['confirm' + (success ? 'Success' : 'Failure')]).addClass(confirmClass);
            this.confirmClass = confirmClass;
          }
          else {
            confirmResult.css({ visibility: 'hidden' });
          }


        };

        confirmInput.keyup(emailCheckMatch).blur(emailCheckMatch);
        emailInput.keyup(emailCheckMatch).blur(emailCheckMatch);
      });
    }
  };
})(jQuery);
