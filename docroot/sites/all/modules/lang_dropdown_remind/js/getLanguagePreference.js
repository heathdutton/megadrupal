/**
 * Either returns or acts on user browser language preference, depending on what
 * parameters are passed.
 *
 * @param mixed async
 *   If a callable is passed, the callable will be called when a browser's
 *   language preference is determined, with the language preference as its only
 *   argument. Nothing will be returned. If a boolean false is passed, the check
 *   for language preference will be performed synchronously and the result
 *   returned.
 * @return
 *   If called with async=false, the language preference will be returned.
 *   Otherwise,
 */
Drupal.getLanguagePreference = function (async) {
  var preference = '';
  var from_cookie = (function() {
    var parts = document.cookie.split('langPref=');
    return parts.length == 2 ? parts.pop().split(';').shift() : '';
  })();

  // Only perform the HTTP request if the value isn't already known.
  if (!from_cookie) {
    // Perform a request for parsed accept-language header data.
    var ajaxAsync = (typeof async === 'boolean') ? async : true;
    jQuery.ajax({
      url: Drupal.settings.getLanguagePreferencePath,
      async: ajaxAsync,
      success: function (data) {
        preference = data[1] || 'und';

        // Store the language preference in a cookie to minimize requests.
        document.cookie = 'langPref=' + preference + '; path=/';

        // Call the callback with the language preference.
        if (typeof async === 'function') {
          async(preference);
        }
      }
    });
  }
  else {
    // If we already have the value, just call the callback.
    if (typeof async === 'function') {
      async(from_cookie);
    }
  }

  // Only return a value if this method was intended to be called synchronously.
  if (typeof async === 'boolean' && !async) {
    return preference || from_cookie;
  }
};
