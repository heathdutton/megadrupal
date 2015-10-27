/**
 * @file
 * Handle automatic redirect after agreement.
 */

(function($) {
  $(
    function() {
      $(".agree-button").click(
        function() {
          var redirectUrl = document.URL.replace("?cookie-not-accepted=1", "");
          redirectUrl = redirectUrl.replace("cookie-not-accepted=1", "");
          window.location.href = redirectUrl;
        }
      );
    }
  );
})(jQuery);
