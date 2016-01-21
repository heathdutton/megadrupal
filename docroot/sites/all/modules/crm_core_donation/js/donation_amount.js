/**
* @file
* Allows buttons to update the amount of donations.
* 
*/
(function ($) {
  Drupal.behaviors.crm_core_donation_amount_btns = {
    attach: function(context) {
      $(".donation-amounts .donation-amount").click(function(){
        $('[name="field_cmcd_amount[und][0][value]"]').val($(this).val());
        $('[name="commerce_amount_single"]').val($(this).val());
      });
    }
  }
})(jQuery);
