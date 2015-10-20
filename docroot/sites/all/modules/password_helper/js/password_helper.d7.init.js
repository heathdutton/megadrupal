;
(function ($) {
  Drupal.password_helper = Drupal.password_helper || {};

  Drupal.behaviors.password_helper = {
    attach:function (context, settings) {
      $('input.password-field', context).once('password-helper', function () {
        var passwordInput = $(this);
        var outerWrapper = $(this).parent().parent();
        var passwordConfirm = $('input.password-confirm', outerWrapper);

        var passwordHelperWrapperEl = $('<div id="password-helper-wrapper"></div>');
        var passwordHelperEl = $('<div id="password-helper"></div>').appendTo(passwordHelperWrapperEl);
        var passwordHelperGenBtnEl = $('<a href="#" id="password-helper-generator" class="button"></a>').html(Drupal.t("Click for new password")).appendTo(passwordHelperWrapperEl);
        var passwordHelperUseBtnEl = $('<a href="#" id="password-helper-use" class="button"></a>').html(Drupal.t("Use this password")).appendTo(passwordHelperWrapperEl);

        outerWrapper.append(passwordHelperWrapperEl);

        // bind custom event
        passwordHelperEl.bind('password_helper_generatePassword', function () {
          $(this).easypassgen(settings.password_helper);
        });

        // bind generatePassword click handler
        passwordHelperGenBtnEl.bind('click', function(){
          passwordHelperEl.trigger('password_helper_generatePassword');
          return false;
        });

        // take password over
        passwordHelperUseBtnEl.bind('click', function(){
          passwordInput.val(passwordHelperEl.text());
          passwordConfirm.val(passwordHelperEl.text());
          return false;
        });

        // initial trigger
        passwordHelperEl.trigger('password_helper_generatePassword');
      });
    }
  }
})(jQuery);