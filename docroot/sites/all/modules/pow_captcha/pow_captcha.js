/* global Drupal, jQuery */
(function($) {
  var badBrowser = false;

  if (navigator.appName.indexOf("Internet Explorer") != -1) {
      badBrowser = (
          navigator.appVersion.indexOf("MSIE 1") == -1  //v10, 11, 12, etc. is fine too
      );
  }

  Drupal.behaviors.pow_captcha = {
    attach: function(c) {
      $("form input[name='pow_captcha']", c).once('pow_captcha', attach_pow_captcha);
    }
  };

  function attach_pow_captcha() {
    var $form = $(this).parents('form').eq(0);

    if (badBrowser) {
        $(".pow-captcha-ie-warning", $form).show();
        return;
    }

    $form.addClass('pow-captcha-attached');

    var $button = $form.find(".form-actions input[type=submit], .form-actions button.form-submit").eq(0);

    $button.hashcash({
        autoId: false,
        hashcashInputName: "form_build_id",
        key: Drupal.settings.pow_captcha.key,
        complexity: Drupal.settings.pow_captcha.complexity,
        lang: {
            screenreader_notice: Drupal.t('Click this to unlock submit button'),
            screenreader_notice_done: Drupal.t('Form unlocked. Please submit this form.'),
            screenreader_computing: Drupal.t('Please wait while computing.'),
            screenreader_computed: Drupal.t('Form is ready. Please submit this form.'),
            screenreader_done: Drupal.t('__done__% done.'),
            popup_info: Drupal.t('Please unlock it first.')
        }
    });
  }

})(jQuery);


