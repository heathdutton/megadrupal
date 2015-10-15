/**
 * @file
 * Initialize the Optify system.
 *
 * The following javascript enables Optify page tracking.
 */

(function ($) {

  Drupal.behaviors.optifyTracking = {
    attach: function (context, settings) {
      var _opt = _opt || [];

      _opt.push([ "view", ]);
      var scr = document.createElement("script");
      scr.type = "text/javascript";
      scr.async = true;
      scr.src = "//service.optify.net/opt-v2.js";
      var other = document.getElementsByTagName("script")[0];
      other.parentNode.insertBefore(scr, other);
    }
  };

})(jQuery);
