(function ($) {
var captcha_status = new Array(); 
Drupal.behaviors.boost_captcha = {
  attach: function (context) {
    $('div.boost-captcha-process-form:not(boost-captcha-processed)').each(function() {
      var form_id = $(this).find('input[name="form_id"]').attr('value');
      var form_wrapper = $(this);
      // Don't do anything if there is no captcha fieldset
      if (form_wrapper.find('fieldset.captcha').length == 0) {
        return;
      }
      captcha_status[form_id] = 'default';
      form_wrapper.click(function(e) {
        if (captcha_status[form_id] == 'default') {
          // Disable form submission till AJAX returns
          form_wrapper.find('form').submit(function() {
            if (captcha_status[form_id] != 'loaded') {
              e.preventDefault();
              return false;
            }
            return true;
          });
          captcha_status[form_id] = 'processing';
          // Fetch captcha using ajax
          $.ajax({
            url: Drupal.settings.basePath + 'ajax/boost-captcha/get-captcha/' + form_id,
            dataType: "html",
            success: function(data) {
              form_wrapper.find('fieldset.captcha').replaceWith(data);
              Drupal.attachBehaviors(form_wrapper.find('fieldset.captcha'));
              captcha_status[form_id] = 'loaded';
              // If captcha is recaptcha re-process the recaptcha and refresh it
              if (form_wrapper.find('input[name="captcha_response"]').attr('value') == 'reCAPTCHA') {
                var recaptcha_id = 'recaptcha_ajax_api_container_' + form_id;
                if (form_wrapper.find('div.' + recaptcha_id).length == 0) {
                  form_wrapper.find('fieldset.captcha div.form-item').before($('<div id="' + recaptcha_id + '"></div>'));
                }
                // load Recaptcha if not already loaded
                if (typeof(Recaptcha) != "object") {
                  $.ajax({
                    url: 'http://www.google.com/recaptcha/api/js/recaptcha.js',
                    dataType: "script",
                    success: function() {
                      if (Recaptcha && Recaptcha.create) {
                        Recaptcha.create(Drupal.settings.boost_captcha.recaptcha_public_key, recaptcha_id, RecaptchaOptions.theme);
                      }
                    }
                  });
                }
                else {
                  if (Recaptcha && Recaptcha.create) {
                    Recaptcha.create(Drupal.settings.boost_captcha.recaptcha_public_key, recaptcha_id, RecaptchaOptions.theme);
                  }
                }
              }
            }
          });
        }
      });
    }).addClass('boost-captcha-processed');
  }
};  
})(jQuery);
