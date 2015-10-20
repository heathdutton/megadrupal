/**
 * @file
 * AJAX functionality for Yandex.Captcha module.
 *
 * Provides some client-side functionality refresh captcha images.
*/

(function ($) {
  Drupal.behaviors.yandexCaptcha = {
    attach: function (context) {
      $('.reload-captcha', context).not('.processed').bind('click', function () {
        var element = $(this);
        var $form = element.parents('form');
        var url = element.attr('href');
        element.addClass('processed');
        element.after('<div class="ajax-progress ajax-progress-throbber yandex-captcha-throbber"><div class="throbber">&nbsp;</div></div>');
        $.get(url, {}, 
          function (response) {
            if(response.status == 1) {
              if (typeof response.data.url == 'string' || response.data.url instanceof String) {
                $('.captcha', $form).find('img').attr('src', response.data.url);                
              } else {
                $('.captcha', $form).find('img').attr('src', response.data.url[0]);
              }
              $('input[name=captcha_sid]', $form).val(response.data.sid);
              $('input[name=captcha_token]', $form).val(response.data.token);
            }
            else {
              alert(response.message);
            }
            element.siblings('.yandex-captcha-throbber').remove();
          }, 'json');
        return false;
      });
    }

  };
})(jQuery);
