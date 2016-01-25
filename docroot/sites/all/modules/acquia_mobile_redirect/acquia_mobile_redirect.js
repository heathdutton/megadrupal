(function($){
  Drupal.behaviors.acquia_mobile_redirect = {
    attach: function (context) {
      // Cookie domain and lifetime.
      var domain = Drupal.settings.acquia_mobile_redirect[0].domain;
      var lifetime = Drupal.settings.acquia_mobile_redirect[0].lifetime;

      // Calculate a timeout for the device pinning cookie.
      var now = new Date();
      var time = now.getTime();
      time += lifetime;
      now.setTime(time);
      expiry = now.toUTCString();

      // Switch to mobile.
      $('a#acquia-mobile-redirect-mobile').click(function(event) {
        document.cookie = "X-UA-Device-force=mobile; domain=" + domain + "; expires=" + expiry + "; path=/";
        window.location.href = $(this).attr("href");
      });

      // Switch to tablet.
      $('a#acquia-mobile-redirect-tablet').click(function(event) {
        document.cookie = "X-UA-Device-force=tablet; domain=" + domain + "; expires=" + expiry + "; path=/";
        window.location.href = $(this).attr("href");
      });

      // Switch to desktop.
      $('a#acquia-mobile-redirect-desktop').click(function(event) {
        document.cookie = "X-UA-Device-force=desktop; domain=" + domain + "; expires=" + expiry + "; path=/";
        window.location.href = $(this).attr("href");
      });
    }
  };
})(jQuery);
