/**
 * @file
 * It Generates qrcode for faircoin addresses when the checkout process shows it.
 *
 * It uses jquery.qrcode library.
 */

(function ($) {

  "use strict";

  Drupal.behaviors.commerce_faircoin = {
    attach: function (context, settings) {
      var text_qrcode = Drupal.settings.commerce_faircoin.text_qrcode;
      $('div.faircoin-address-qrcode-image').qrcode({
        width: 120,
        height: 120,
        text: text_qrcode
      });
    }
  };
})(jQuery);
