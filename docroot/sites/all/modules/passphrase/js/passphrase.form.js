/**
 * @file
 * Handles passphrase form alterations.
 */
(function ($) {
  Drupal.behaviors.passPhrase = {
    attach: function(context, settings) {
      $(".form-type-passphrase-confirm").each(function () {
        var button = $('<input type="submit" class="form-submit" value="' + Drupal.t('Show text') + '">');
            pass1 = $(this).find(".passphrase-field"),
            pass2 = $(this).find(".passphrase-confirm"),
            text1 = $('<input type="text" class="form-text">'),
            text2 = $('<input type="text" class="form-text">');

        // Check that the fields we need are there
        if (pass1.length && pass2.length) {
          // Setup a textfield that can be used as alternative to the password
          // field.
          text1.attr('size', pass1.attr('size')).hide();
          // Keep the fields in sync whenever they are typed into.
          text1.keyup(function () {
            pass1.val($(this).val());
          });
          pass1.keyup(function () {
            text1.val($(this).val());
          });
          pass1.after(text1);

          // Now repeat for the confirm field.
          text2.attr('size', pass2.attr('size')).hide();
          // Keep the fields in sync whenever they are typed into.
          text2.keyup(function () {
            pass2.val($(this).val());
          });
          pass2.keyup(function () {
            text2.val($(this).val());
          });
          pass2.after(text2);

          // Add a button to switch between seen and hidden password.
          button.click(function (event) {
            if ($(this).hasClass('shown')) {
              $(this).removeClass('shown');
              $(this).parent().find('input[type=password]').show().end()
                              .find('input[type=text]').hide().end();
              $(this).val(Drupal.t('Show text'));
            }
            else {
              $(this).addClass('shown');
              $(this).parent().find('input[type=text]').show().end()
                              .find('input[type=password]').hide().end();
              $(this).val(Drupal.t('Hide text'));
            }
            event.preventDefault();
          });
          $(this).prepend(button);
        }
      });
    }
  }
})(jQuery);


