/**
 * @file
 * JS behaviours for the uc_worldpay module.
 */

Drupal.behaviors.uc_worldpay = function(context) {
  uc_worldpay_toggle_test_mode_result();
  $('#edit-uc-worldpay-test').click(function() {
    uc_worldpay_toggle_test_mode_result();
  });
};

function uc_worldpay_toggle_test_mode_result() {
  if ($('#edit-uc-worldpay-test').attr('checked')) {
    $('#edit-uc-worldpay-test-result').removeAttr('disabled');
  }
  else {
    $('#edit-uc-worldpay-test-result').attr('disabled', 'disabled');
  }
}
