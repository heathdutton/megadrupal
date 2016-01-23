/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.imageeditor_close = {};
  Drupal.behaviors.imageeditor_close.attach = function(context, settings) {
    try {
      // Check for a popup.
      if (window.opener && window.opener.location.hostname === window.location.hostname) {
        window.self.close();
      }
    }
    catch (e) {
    }

    if (window.parent) {
      window.parent.Drupal.imageeditor.overlay.hide();
    }
  };

})(jQuery);
