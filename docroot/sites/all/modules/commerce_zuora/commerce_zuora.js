(function (Drupal, $, Z, window) {
  "use strict";

  Drupal.behaviors.zPaymentFrame = {
    attach: function (context, settings) {
      var params = settings.zPayments.params || {};
      var fields = settings.zPayments.fields || {};
      Z.render(
        params,
        fields,
        Drupal.behaviors.zPaymentFrame.frameCallback
      );
    },
    frameCallback: function (response) {
      var redirect = "";
      if (response.success) {
        redirect = Drupal.settings.zPayments.commerce.nextPage + "?refid=" + response.refId;
      } else {
        redirect = Drupal.settings.zPayments.commerce.prevPage + "?zuoraEc=" + response.errorCode + "&zuoraEm=" + response.errorMessage;
      }

      window.location.replace(redirect);
    }
  };
})(Drupal, jQuery, Z, window);
