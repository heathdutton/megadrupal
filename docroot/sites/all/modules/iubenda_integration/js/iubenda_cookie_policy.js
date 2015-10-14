/**
 * Iubenda Cookie Policy script.
 */
var _iub = _iub || [];
_iub.csConfiguration = Drupal.settings.iubenda_integration.cookie_policy;
(function (w, d) {
  var loader = function () {
    var s = d.createElement("script"), tag = d.getElementsByTagName("script")[0];
    s.src = "//cdn.iubenda.com/cookie_solution/iubenda_cs.js";
    tag.parentNode.insertBefore(s, tag);
  };
  if (w.addEventListener) {
    w.addEventListener("load", loader, false);
  } else if (w.attachEvent) {
    w.attachEvent("onload", loader);
  } else {
    w.onload = loader;
  }
})(window, document);