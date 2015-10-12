/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function($) {
  "use strict";

  Drupal.yashare = {};
  Drupal.yashare.initialize = function (callback, context, settings) {
    if (typeof Ya != 'undefined' && typeof Ya.share != 'undefined') {
      if (callback) {
        callback(context, settings);
      }
    }
    else {
      $.ajax({
        url: '//yandex.st/share/share.js',
        dataType: 'script',
        cache: true, //otherwise will get fresh copy every page load, this is why not $.getScript().
        success: function (data, textStatus, jqXHR) {
          if (callback) {
            callback(context, settings);
          }
        }
      });
    }
  };

})(jQuery);
