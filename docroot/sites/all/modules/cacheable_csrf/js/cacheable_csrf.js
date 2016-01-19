(function ($) {
  "use strict";
  var cacheableCsrfInterval;
  Drupal.behaviors.cacheableCsrfReplaceToken = {
    attach: function (context) {
      var cacheable_csrf_token = $.cookie('cacheable_csrf_token');
      if (cacheable_csrf_token === null) {
        $.post(Drupal.settings.basePath + 'cacheable-csrf', function() {
          Drupal.behaviors.cacheableCsrfReplaceToken.apply(context);
        });
      }
      else {
        Drupal.behaviors.cacheableCsrfReplaceToken.apply(context);
      }
      $('body').once(function() {
        cacheableCsrfInterval = setInterval(function() {
          Drupal.behaviors.cacheableCsrfReplaceToken.checkStale(cacheable_csrf_token);
        }, 1000);
      });
    },
    apply: function (context, current_token) {
      // Ensure the cookie value is up to date whenever this runs.
      var new_token = $.cookie('cacheable_csrf_token');
      $("input[type='hidden'][name='cacheable_csrf_token']", context).val(new_token);

      // Don't use context here, for AJAX links sometimes the context is the
      // link itself, and the a selector doesn't work if that's the only HTML.
      $("a[href*='cacheable_csrf_token']").each(function() {
        var href = $(this).attr('href')
          .replace('cacheable_csrf_token_placeholder', new_token)
          .replace(current_token, new_token);
        $(this).attr('href', href);
      });
    },
    checkStale: function (current_token) {
      // Get the current cookie value.
      var new_token = $.cookie('cacheable_csrf_token');
      if (new_token !== current_token) {
        clearInterval(cacheableCsrfInterval);
        cacheableCsrfInterval = setInterval(function() {
          Drupal.behaviors.cacheableCsrfReplaceToken.checkStale(new_token);
        }, 1000);
        // Don't send context through, so that the entire page is updated.
        Drupal.behaviors.cacheableCsrfReplaceToken.apply(null, current_token);
      }
    },
    weight: -1
  };
})(jQuery);
