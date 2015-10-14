(function($) {

/**
 * Fill in Battle.net login fields from the visitor cookie.
 */
Drupal.behaviors.fillLoginFromCookie = {
  attach: function(context, settings) {
    $('form.blizzardapi-login-from-cookie').once('blizzardapi-login-from-cookie', function() {
      var formContext = this;
      $.each(['battlenet_region'], function () {
        var $element = $('[name=' + this + ']', formContext);
        var cookie = $.cookie('Drupal.visitor.' + this);
        if ($element.length && cookie) {
          $element.val(cookie);
        }
      });
    });
  }
}
  
})(jQuery);
