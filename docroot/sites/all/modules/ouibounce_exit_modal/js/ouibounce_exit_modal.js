var Drupal = Drupal || {};
var ouibounceExitModal = ouibounceExitModal || {};

(function ($, Drupal) {
  "use strict";

  Drupal.behaviors.ouibounceExitModal = {
    attach: function (context) {
      $(context).find('body').once('ouibounce-exit-modal', function () {
        var settings = Drupal.settings.ouibounceExitModal;

        ouibounceExitModal = ouibounce(document.getElementById('ouibounce-exit-modal'), {
          sensitivity: settings.Sensitivity,
          aggressive: settings.AggressiveMode,
          timer: settings.Timer,
          delay: settings.Delay,
          cookieExpire: settings.CookieExpiration,
          cookieDomain: settings.CookieDomain,
          cookieName: settings.CookieName,
          sitewide: settings.SitewideCookie,
          callback: function () {
          }
        });

        $('body').on('click', function () {
          $('#ouibounce-exit-modal').hide();
        });

        $('#ouibounce-exit-modal .modal-footer').on('click', function () {
          $('#ouibounce-exit-modal').hide();
        });

        $('#ouibounce-exit-modal .modal').on('click', function (e) {
          e.stopPropagation();
        });
      });
    }
  };

})(jQuery, Drupal);
