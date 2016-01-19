/**
 * @file
 * Scale pixels on Chrome.
 */

(function ($, Drupal) {

"use strict";

Drupal.behaviors.cointools = {
  attach: function (context) {
    // If we are not on Chrome then do nothing.
    if(!window.chrome) {
      return;
    }
    // Scale each QR code without blurring.
    $(".qrcode").each(function() {
      var canvas = document.createElement('canvas');
      canvas.width = this.width;
      canvas.height = this.height;
      var context = canvas.getContext('2d');
      context.imageSmoothingEnabled = false;
      context.drawImage(this, 0, 0, this.width, this.height);
      $(this).after(canvas);
      $(this).remove();
    });
  }
};

})(jQuery, Drupal);
