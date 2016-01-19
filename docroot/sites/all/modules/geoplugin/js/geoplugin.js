/**
 * @file geoplugin.js
 * geoPlugin main JavaScript file.
 *
 */

(function ($) {

  $(document).ready(function() {

    // Required to store JSON objects in cookies.
    // @TODO: Make sure it works on old browsers, and provide an alternative.
    $.cookie.json = true;

    // Request API data only if the cookie does not exist.
    if (!$.cookie('geoplugin')) {
      $.getJSON("http://www.geoplugin.net/json.gp?jsoncallback=?", function(data) {

        // Store the cookie.
        $.cookie('geoplugin', data);
      });
    }
  });

})(jQuery);
