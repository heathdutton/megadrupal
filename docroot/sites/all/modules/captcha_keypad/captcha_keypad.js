(function ($) {
  Drupal.behaviors.capcha_keypad = {
    attach: function (data, settings) {
      jQuery(document).ready(function () {

          jQuery('.captcha-keypad-keypad-used').val('');

          jQuery('.captcha-keypad-input').val('').keyup(function () {
          var wrapper = jQuery(this).parent().parent().parent().parent();
          var textfield = wrapper.find('.captcha-keypad-input');
          var message = wrapper.find('.message');
          textfield.val('');
          message.css('color', 'red');
          message.html(Drupal.t('Use keypad ->'));
        });

        jQuery('.form-item-captcha-keypad-input').append(
          '<span class="clear">' +
          Drupal.t('Clear') +
          '</span><br/>' +
          '<span class="message"></span>'
        );

        jQuery('.form-item-captcha-keypad-input .clear').click(function () {
          var wrapper = jQuery(this).parent().parent().parent().parent();
          var textfield = wrapper.find('.captcha-keypad-input');
          var message = wrapper.find('.message');
          textfield.val('');
          message.html('');
        });

        jQuery('.captcha-keypad .inner span').click(function () {
          var wrapper = jQuery(this).parent().parent().parent().parent();
          var textfield = wrapper.find('.captcha-keypad-input');
          var keypadused = wrapper.find('.captcha-keypad-keypad-used');
          var message = wrapper.find('.message');
          var value = textfield.val();
          textfield.val(value + jQuery(this).text());
          keypadused.val('Yes');
          message.html('');
        });
      })
    }
  };
})(jQuery);
