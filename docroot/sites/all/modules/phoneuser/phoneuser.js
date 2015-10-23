(function ($) {
  Drupal.behaviors.PhoneUser = {
    attach: function(context) {
      if(jQuery("#edit-phoneuser-register").is(":checked")) {
        jQuery("#phoneuser-radios-options-1").css("display","block");
      }
    }
  };
})(jQuery);
