/**
 * @file
 * Show / hide the vat number field according to the selected billing country
 */
(function ($) {
  UCVatNumber = {
    european_countries: [], // this will be populated inline

    toggle_field: function() {
      // get billing country
      var el = $('select[id^=edit-panes-billing-billing-country]');
      // this may not be available, if the billing and delivery address is the same
      if (!el.length) {
        el = $('select[id^=edit-panes-delivery-delivery-country]');
      }

      // sanity check
      if (!el.length) {
        return;
      }

      for (var i = 0; i < UCVatNumber.european_countries.length; i++) {
        if(UCVatNumber.european_countries[i] == el.val()) {
          $('#vat_number_pane-pane').fadeIn();
          return;
        }
      }

      $('#vat_number_pane-pane').fadeOut();
    }
  };

  Drupal.behaviors.uc_vat_number = {
    attach: function(context, settings) {
      $('select[id^=edit-panes-billing-billing-country]').change(function() {
        UCVatNumber.toggle_field();
      });

      // force refresh after page loads
      UCVatNumber.toggle_field();
    }
  }
})(jQuery);
