(function ($) {
  Drupal.behaviors.fillUserInfoFromCookie = {
    attach: function (context, settings) {
      $('form.user-info-from-cookie').once('user-info-from-cookie', function () {
        var formContext = this;
        $.each(['name', 'mail', 'homepage'], function () {
          var $element = $('[name="field_anonymous_author[und][0][' + this + ']"]', formContext);
          var cookie = $.cookie('Drupal.visitor.' + this);
          if ($element.length && cookie) {
            $element.val(cookie);
          }
        });
      });
    }
  };
})(jQuery);