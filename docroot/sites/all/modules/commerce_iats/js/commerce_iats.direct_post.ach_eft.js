/**
 * @file
 * Commerce iATS Direct Post ACH/EFT JavaScript functionality.
 */

(function ($) {

  Drupal.behaviors.commerce_iats_direct_post_ach_eft = {
    attach: function(context, settings) {
      // Attach click handler to populate Direct Post form elements on submit.
      $('input.checkout-continue', context).unbind('click').bind('click', function() {

        // General fields.

        var account_number = $("input[name=internal_iats_dpm_number]").val();
        var type = $("select[name=internal_iats_dpm_type]").val();

        // USD currency fields.

        var routing_number = $("input[name=internal_iats_dpm_routing_number]").val();

        if (routing_number) {
          $("input[name=IATS_DPM_AccountNumber]").val(String(routing_number) + String(account_number));
        }

        // CAD currency fields.

        var bank_number = $("input[name=internal_iats_dpm_bank_number]").val();
        var transit_number = $("input[name=internal_iats_dpm_transit_number]").val();

        if (bank_number) {
          $("input[name=IATS_DPM_AccountNumber]").val(String(bank_number) + String(transit_number) + String(account_number));
        }

        // GBP currency fields.

        var sort_code = $("input[name=internal_iats_dpm_sort_code]").val();

        if (sort_code) {
          $("input[name=IATS_DPM_AccountNumber]").val(String(sort_code) + String(account_number));
        }

        if (type) {
          $("input[name=IATS_DPM_AccountType]").val(type);
        }

        return true;
      });
    }
  }

})(jQuery);
