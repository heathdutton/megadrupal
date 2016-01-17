(function ($) {
/* Show a modal message when user clicks on "Login" (there may be a wait). */

Drupal.behaviors.openid_sso_relying = {};
Drupal.behaviors.openid_sso_relying.attach = function(context) {
  $('input.login-submit').click( function () {
    $.blockUI({ css: {border: 'none', background: 'transparent', color: '#CCCCCC'}, message:  Drupal.settings.openid_sso_relying_wait_message});
  });
}

})(jQuery);
