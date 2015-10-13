(function ($) {
  function replacePasswordMeter(passwordInput) {
    var innerWrapper = $(passwordInput).parent();
    var outerWrapper = $(passwordInput).parent().parent();
    var confirmInput = $('input.password-confirm', outerWrapper);

    function ucfirst(string) {
      return string.charAt(0).toUpperCase() + string.substr(1);
    }

    function passwordCheck() {
      if (window.zxcvbn) {
        // Collect personal data
        var data = [];
        $('input:not(.password-field)').each(function () {
          data.push($(this).val());
        });

        var res = zxcvbn(passwordInput.val(), data);
        var text = res.crack_time_display;
        var match = text.match(/^[\d]+/);
        if (match) {
          text = Drupal.t(text.replace(/^[\d]+/, '@t'), {'@t': match});
        }
        else {
          text = Drupal.t(ucfirst(text));
        }


        $(innerWrapper).find('.indicator')
          .css('width', (res.score * 25) + '%');
        $(innerWrapper).find('.password-strength-text')
          .html(text);
      }
      confirmInput.trigger('blur');
    }

    passwordInput
      .unbind('keyup')
      .unbind('focus')
      .unbind('blur')
      .keyup(passwordCheck)
      .focus(passwordCheck)
      .blur(passwordCheck);

    $(innerWrapper).find('.password-strength-title')
      .html(Drupal.t("Time to crack:"));
  }

  Drupal.behaviors.zxcvbn = {
    attach: function (context) {
      $('input.password-field', context).once('zxcvbn', function () {
        var passwordInput = $(this);
        function check() {
          if (!passwordInput.hasClass('password-processed')) {
            setTimeout(check, 500);
          }
          else {
            replacePasswordMeter(passwordInput);
          }
        }

        check();
      });
    }
  };
})(jQuery);
