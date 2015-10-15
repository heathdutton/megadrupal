/**
 * @file
 * jQuery functions for the donation form
 */
Drupal.behaviors.authorizenet_payment_profile = function(context) {
  $('.pay-cc-form').hide();
  $('.authorizenet-payment-profile').parents().find('form').change(function() {
    if ($('.authorizenet-payment-profile:checked').val() == 0) {
      $('.pay-cc-form').slideDown('slow');
    }
    else {
      $('.pay-cc-form').slideUp('slow')  ;
    }
  }).trigger('change');
}