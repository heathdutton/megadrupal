(function ($) {
  Drupal.behaviors.crm_core_profile_commerce_items_amount_buttons = {
    attach: function(context) {
      $(".amounts .amount").click(function(){
        $('[name="commerce_amount_single"]').val($(this).attr('amount'));
      });
    }
  }
})(jQuery);
