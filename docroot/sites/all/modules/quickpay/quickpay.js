(function ($) {

Drupal.behaviors.quickpayCardSelection = {
  attach: function (context, settings) {
    // Avoid slide animation if cards is hidden on load.
    if ($('#quickpay-accepted-methods input:radio:checked').val() != 'selected') {
      $('#quickpay-accepted-cards').hide();
    }
    // Toggle the display as necessary when the radio is clicked.
    $('#quickpay-accepted-methods input:radio').change(function () {
    if ($(this).val() == 'selected') {
      $('#quickpay-accepted-cards').slideDown();
    }
    else {
      $('#quickpay-accepted-cards').slideUp();
    }
    });
  }
};
})(jQuery);
