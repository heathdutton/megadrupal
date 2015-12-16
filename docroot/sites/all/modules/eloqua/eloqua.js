/**
 * @file
 * Initialize Eloqua tracking.
 */

var _elqQ = _elqQ || [];

(function ($) {

  Drupal.behaviors.eloquaTracking = {
    attach: function (context, settings) {

      _elqQ.push(['elqSetSiteId', Drupal.settings.eloqua.siteId]);
      _elqQ.push(['elqTrackPageView']);

      function async_load() {
        var s = document.createElement('script');
        s.type = 'text/javascript';
        s.async = true;
        s.src = '//img.en25.com/i/elqCfg.min.js';
        var x = document.getElementsByTagName('script')[0];
        x.parentNode.insertBefore(s, x);
      }
      if (window.addEventListener) {
        window.addEventListener('DOMContentLoaded', async_load, false);
      } else if (window.attachEvent) {
        window.attachEvent('onload', async_load);
      }
    }
  };
})(jQuery);
