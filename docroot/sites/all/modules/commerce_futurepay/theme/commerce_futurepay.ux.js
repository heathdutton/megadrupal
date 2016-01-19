/**
 * Hide the "continue to next step" button when choosing the FuturePay payment
 * gateway.
 * @namespace commerce_futurepay
 */
(function ($) {
  $(document).ready(function () {
    if (($container = $('#edit-commerce-payment-payment-method')).length == 1) {
      if ($container.find('input[value="futurepay|commerce_payment_futurepay"]:checked').length == 1) {
        $('#edit-continue').addClass('element-invisible');
        $('span.button-operator').addClass('element-invisible');
      }

      $container.bind('click', function (e) {
        $input = $(this).find('input[value="futurepay|commerce_payment_futurepay"]');
        if ($input.is(':checked')) {
          $('#edit-continue').addClass('element-invisible');
          $('span.button-operator').addClass('element-invisible');
        }
        else {
          $('#edit-continue').removeClass('element-invisible');
          $('span.button-operator').removeClass('element-invisible');
        }
      });
    }
  });
})(jQuery);