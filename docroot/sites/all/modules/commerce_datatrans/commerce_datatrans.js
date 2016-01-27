(function ($) {

  Drupal.behaviors.datatransLightbox = {
    attach: function (context, settings) {

      // Register a trigger on the payment button, use once() to avoid
      // multiple triggers.
      $('#paymentButton', context).once('datatrans', function() {
        $(this).click(function () {
          $('form').submit(function (e) {
            e.preventDefault();
          });

          Datatrans.startPayment({
            // Use the class selector here and not the id selector because of
            // cloned payment button.
            'form': '.checkout-continue[data-merchant-id]',
            'closed': function() {
              // Commerce checkout js will clone and hide original pay button
              // with disabled one (disable multiple clicks) on payment button
              // click, so we need to remove disabled button and show original
              // pay button again.
              // @see commerce_checkout.js
              $(this.form + '[disabled]').remove();
              $(this.form).show();
            }
          });
        });

        // Hides continue button if Datatrans payment method selected.
        if ($('#paymentButton').length) {
          $('#edit-buttons').hide();
        }
      });

      // Shows continue button for any other payment method.
      if (!$('#paymentButton').length) {
        $('#edit-buttons').show();
      }
    }
  };

})(jQuery);
