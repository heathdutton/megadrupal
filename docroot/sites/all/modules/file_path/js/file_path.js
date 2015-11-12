/**
 * @file
 * file_path.js
 */
(function ($) {

  /**
   * When the user search for sub folders we need to replace the "/" character
   * with another character so the routing system won't with break the word for
   * us.
   */
  Drupal.behaviors.filePath = {
    attach: function(context, settings) {
      $.ajaxSetup({
        beforeSend: function(jqXHR, ajaxSettings) {
          // Get the word the user searched for.
          var userSearch = ajaxSettings.url.replace(settings.filePath.url + "/", '');

          // Replace the "/" in the word with another character and create a new
          // XMLHTTPRequest request.
          jqXHR.open("GET", settings.filePath.url + "/" + userSearch.split("/").join("*"), true);
        }
      });
    }
  }

})(jQuery);


