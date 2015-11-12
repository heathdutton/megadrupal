(function($) {
  Drupal.behaviors.language_cookie_js_redirect = {
    attach: function(context) {
      var cookieName = Drupal.settings.language_selection_page.cookieName;
      var language = readCookie(cookieName);

      if (language) {
        var targetUrl = $('a.language_selection_page_link_' + language).get(0).href;
        if (targetUrl) {
          if (targetUrl.substr(0,4) != 'http') {
            targetUrl = location.protocol + "//" + location.host + targetUrl;
          }
          if (navigator.appName == "Microsoft Internet Explorer" && (parseFloat(navigator.appVersion.split("MSIE")[1])) < 7) {
            window.location.href = targetUrl;
          } else {
            window.location = targetUrl;
          }
        }
      }
    }
  }

  function readCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) {
      var c = ca[i];
      while (c.charAt(0)==' ') c = c.substring(1,c.length);
      if (c.indexOf(nameEQ) == 0) {
        return c.substring(nameEQ.length,c.length);
      }
    }
    return null;
  }

})(jQuery);
