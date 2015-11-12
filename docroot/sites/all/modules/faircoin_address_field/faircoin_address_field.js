/**
 * @file
 * It Generates qrcode for faircoin addresses when the field is displayed.
 *
 * It uses jquery.qrcode library.
 */

(function ($) {

  "use strict";

  Drupal.behaviors.faircoin_address_field = {
    attach: function (context, settings) {
      $('code.faircoin-address-qrcode-text').each(function (index) {
        var text = $(this).text();
        $(this).siblings('div.faircoin-address-qrcode-wrapper').children('div.faircoin-address-qrcode-image').css('z-index', '-999').qrcode({
          width: 100,
          height: 100,
          text: 'faircoin:' + text
        });
      });
      $('img.icon-qrcode-petit').click(function () {
        var div_qrcode = $(this).siblings('div.faircoin-address-qrcode-wrapper').children('div.faircoin-address-qrcode-image');
        if (div_qrcode.hasClass('div-qrcode-visible')) {
          div_qrcode.removeClass('div-qrcode-visible').css('z-index', '-999');
        }
        else {
          div_qrcode.addClass('div-qrcode-visible').css('z-index', '999');
          $('div.faircoin-address-qrcode-image').not(div_qrcode).removeClass('div-qrcode-visible').css('z-index', '-999');
        }
      });
      $(document).click(function (event) {
        if (!$(event.target).closest('img.icon-qrcode-petit').length) {
          $('div.faircoin-address-qrcode-image').removeClass('div-qrcode-visible').css('z-index', '-999');
        }
      });
    }
  };
})(jQuery);
