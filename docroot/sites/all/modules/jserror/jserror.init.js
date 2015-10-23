/**
 * Uncompressed code for the JavaScript used in jserror.module
 * For debug mode only. Do not use in production environment.
 */

(function (window, document) {
  // Create jserror object to store data on window.onerror
  jserror = {
      data: [],
      onError: function () {
        jserror.data.push(arguments);
        return true;
      }
  };

  // Load the script for sending data to server.
  var scriptLoader = function () {
    var s = document.createElement("script"),
    scriptLocation = document.getElementsByTagName("script")[0];
    s.src = Drupal.settings.jserrorPath + "/jserror.js";
    s.async = !0;
    scriptLocation.parentNode.insertBefore(s, scriptLocation);
  };
  window.addEventListener ? window.addEventListener("load", scriptLoader, !1) : window.attachEvent("onload", scriptLoader);
  window.onerror = jserror.onError
})(window, document)
