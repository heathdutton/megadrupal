/**
 * @file
 * Contains popup logic.
 */

(function ($) {
  Drupal.behaviors.deezer = {
    attach: function (context, settings) {
      jQuery('#deezer-login', context).click(function() {
        // @TODO Need to refactor this heavily.
        var api_key = Drupal.settings.deezer.app_id,
          src = "http://connect.deezer.com/oauth/auth.php?app_id=" + api_key + "&redirect_uri=http://" + location.hostname + Drupal.settings.basePath + "/deezer/login/&perms=basic_access,email";

        var screenX  = typeof window.screenX !== 'undefined' ? window.screenX : window.screenLeft,
          screenY     = typeof window.screenY !== 'undefined' ? window.screenY : window.screenTop,
          outerWidth  = typeof window.outerWidth !== 'undefined' ? window.outerWidth : document.body.clientWidth,
          outerHeight = typeof window.outerHeight !== 'undefined' ? window.outerHeight : (document.body.clientHeight - 22),
          width    = 500,
          height   = 270,
          left     = parseInt(screenX + ((outerWidth - width) / 2), 10),
          top      = parseInt(screenY + ((outerHeight - height) / 2.5), 10),
          features = (
            'width=' + width +
              ',height=' + height +
              ',left=' + left +
              ',top=' + top
            );

        var login_popup = window.open(src,'Deezer_login', features);

        if (window.focus) {
          login_popup.focus();
        }

        return false;
      });
    }
  };
})(jQuery);
