/**
 * @file
 * Javascript for China address.
 */


(function ($) {
  Drupal.behaviors.china_address = {
    attach: function(context) {
      chinaAddressLevelTrigger();
      $("#edit-field-settings-china-address-level").change(function() {
        chinaAddressLevelTrigger();
      });
    }
  }

  function chinaAddressLevelTrigger() {
  if ($("#edit-field-settings-china-address-level").val() == 3) {
    $('.form-item-field-settings-china-address-province-default').show();
  }
  else {
    $('.form-item-field-settings-china-address-province-default').hide();
  }
}
})(jQuery);
