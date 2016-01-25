
Drupal.behaviors.ecCheckout = function() {
  $('#ec-checkout-form input, #ec-checkout-form select')
    .not('.ignore-update')
    .change(function() {
      if (!$('#edit-order').attr('disabled')) {
        $('#edit-order').attr('disabled', true).parent().before('<div class="warning" style="display: block;"><span class="warning">*</span> '+ Drupal.t('Checkout has been changed. Please update the order to ensure the totals are correct.') +'</div>');
      }
    });
};
