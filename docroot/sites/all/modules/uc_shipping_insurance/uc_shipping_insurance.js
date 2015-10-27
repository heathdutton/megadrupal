(function($) {
  Drupal.behaviors.ucShippingInsuranceAdmin = {
    attach: function(context) {
      $('#uc-cart-checkout-settings-form').once('uc-shipping-insurance-type-toggle', function() {
        var $form = $(this);
        var $amount = $form.find(':input[name="uc_shipping_insurance_checkout_pane_amount"]');
        var $prefix = $amount.prev('.field-prefix');
        var $suffix = $amount.next('.field-suffix');
        var $calcMode = $(':input[name="uc_shipping_insurance_checkout_pane_calculation_mode"]', $form);
        var changeMode = function() {
          Drupal.settings.uc_shipping_insurance.amount_sign;
          Drupal.settings.uc_shipping_insurance.sign_after;
          if ($(this).val() == 'fixed') {
            if (Drupal.settings.uc_shipping_insurance.sign_after) {
              $prefix.html('');
              $suffix.html(Drupal.settings.uc_shipping_insurance.amount_sign);
            }
            else {
              $prefix.html(Drupal.settings.uc_shipping_insurance.amount_sign);
              $suffix.html('');
            }
          }
          else {
            $prefix.html('');
            $suffix.html('%');
          }          
        }
        $calcMode.change(changeMode);
        $(':input[name="uc_shipping_insurance_checkout_pane_calculation_mode"]:checked', $form).triggerHandler('change');
      });
    }
  }
})(jQuery);
