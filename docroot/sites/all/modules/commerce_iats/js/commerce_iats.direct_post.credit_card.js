/**
 * @file
 * Commerce iATS Direct Post credit card JavaScript functionality.
 */

(function ($) {

  Drupal.behaviors.commerce_iats_direct_post_credit_card = {
    attach: function(context, settings) {
      // Attach click handler to populate Direct Post form elements on submit.
      $('input.checkout-continue', context).unbind('click').bind('click', function() {
        var expiry_month = $("select[name=internal_iats_dpm_exp_month]").val();
        var expiry_year = $("select[name=internal_iats_dpm_exp_year]").val();

        var expiry_string = expiry_month + '/' + expiry_year[2] + expiry_year[3];

        $("input[name=IATS_DPM_ExpiryDate]").val(expiry_string);

        return true;
      });
    }
  }

})(jQuery);
