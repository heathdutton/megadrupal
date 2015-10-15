/**
 * @file
 * CheckoutIntegration Api javascript functions.
 */

(function ($) {
  'use strict';

  $(function () {

    var head = document.getElementsByTagName("head")[0];
    var s = document.createElement('script');
    s.type = 'text/javascript';
    s.async = true;
    if (Drupal.settings.commerce_checkoutpayment.mode === 'live') {
      s.src = "https://www.checkout.com/cdn/js/checkout.js";
    }
    else {
      s.src = "//sandbox.checkout.com/js/v1/checkout.js";
    }
    head.appendChild(s);
    $('#commerce-checkoutpayment-redirect-form #edit-submit').click(function (event) {
        event.preventDefault();
        if (typeof CheckoutIntegration != 'undefined') {
          if (!CheckoutIntegration.isMobile()) {
            CheckoutIntegration.open();
            $('#commerce-checkoutpayment-redirect-form #edit-submit').attr("disabled", "disabled");
          }
          else {
            $('#cko-cc-redirectUrl').val(CheckoutIntegration.getRedirectionUrl());
            $('#commerce-checkoutpayment-redirect-form').trigger('submit');
          }
        }
      });
  });

  Drupal.behaviors.commerce_checkoutpayment = {
    attach: function (context, settings) {

      var reload = false;
      window.CKOConfig = {
        debugMode: false,
        renderMode: 2, // Displaying widget:- 0 All, 1 Pay Button Only, 2 Icons Only.
        publicKey: Drupal.settings.commerce_checkoutpayment.publicKey,
        customerEmail: Drupal.settings.commerce_checkoutpayment.email,
        namespace: 'CheckoutIntegration',
        customerName: Drupal.settings.commerce_checkoutpayment.name,
        value: Drupal.settings.commerce_checkoutpayment.amount,
        currency: Drupal.settings.commerce_checkoutpayment.currency,
        paymentMode: Drupal.settings.commerce_checkoutpayment.localpayment,
        logoUrl: Drupal.settings.commerce_checkoutpayment.logourl,
        themeColor: Drupal.settings.commerce_checkoutpayment.themecolor,
        buttonColor: Drupal.settings.commerce_checkoutpayment.buttoncolor,
        iconColor: Drupal.settings.commerce_checkoutpayment.iconcolor,
        useCurrencyCode: Drupal.settings.commerce_checkoutpayment.currencycode,
        paymentToken: Drupal.settings.commerce_checkoutpayment.paymentToken,
        forceMobileRedirect: true,
        widgetContainerSelector: '.widget-container', // The .class of the element hosting the Checkout.js widget card icons.
        cardCharged: function (event) {
          $('#cko-cc-paymenToken').val(event.data.paymentToken);
          $('#commerce-checkoutpayment-redirect-form').trigger('submit');
          $('#commerce-checkoutpayment-redirect-form #edit-submit').attr("disabled", "disabled");
        },
        lightboxDeactivated: function () {
          $('#commerce-checkoutpayment-redirect-form #edit-submit').removeAttr("disabled");
          if (reload) {
            window.location.reload();
          }
        },
        paymentTokenExpired: function () {
          reload = true;
        },
        invalidLightboxConfig: function () {
          reload = true;
        }
      };

      $('#edit-commerce-payment-payment-method-commerce-checkoutpaymentcommerce-payment-commerce-checkoutpayment').once('checkoutapi').change(function () {
        var interVal2 = setInterval(function () {
          if ($('.widget-container').length) {
            if (typeof CheckoutIntegration != 'undefined') {
              CheckoutIntegration.render(window.CKOConfig);
              clearInterval(interVal2);
            }
          }
        }, 500);
      });
    }
  };
})(jQuery);
