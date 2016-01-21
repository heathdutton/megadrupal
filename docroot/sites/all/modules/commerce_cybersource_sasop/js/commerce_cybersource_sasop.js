/**
 * @file
 * Javascript to enhance the Commerce CyberSource SASOP payment form.
 */

(function ($) {
  Drupal.behaviors.commerceCybersourceSasop = {
    expiryYear: null,
    expiryMonth: null,
    expiryDate: null,
    attach: function (context, settings) {
      this.expiryDate = $('#edit-card-expiry-date').hide();
      $('div.form-item-card-expiry-date .description span', context).text(Drupal.t('As month and year'));

      if (this.expiryMonth == null) {
        this.expiryMonth = $('<select id="commerce-cybersource-sasop-expiry-month"></select>', context)
          .append($('<option value="01">' + Drupal.t('1 - January') + '</option>', context))
          .append($('<option value="02">' + Drupal.t('2 - February') + '</option>', context))
          .append($('<option value="03">' + Drupal.t('3 - March') + '</option>', context))
          .append($('<option value="04">' + Drupal.t('4 - April') + '</option>', context))
          .append($('<option value="05">' + Drupal.t('5 - May') + '</option>', context))
          .append($('<option value="06">' + Drupal.t('6 - June') + '</option>', context))
          .append($('<option value="07">' + Drupal.t('7 - July') + '</option>', context))
          .append($('<option value="08">' + Drupal.t('8 - August') + '</option>', context))
          .append($('<option value="09">' + Drupal.t('9 - September') + '</option>', context))
          .append($('<option value="10">' + Drupal.t('10 - October') + '</option>', context))
          .append($('<option value="11">' + Drupal.t('11 - November') + '</option>', context))
          .append($('<option value="12">' + Drupal.t('12 - December') + '</option>', context));
        this.expiryMonth.once('change').change(this.updateExpiryDate);
        this.expiryDate.after(this.expiryMonth);
        var breakBefore = $('<div class="clearfix"></div>', context);
        this.expiryMonth.before(breakBefore);
      }
      if (this.expiryYear == null) {
        this.expiryYear = $('<select id="commerce-cybersource-sasop-expiry-year"></select>', context);
        var currentYear = new Date().getFullYear();
        for (var i = 0; i < 20; i++) {
          var iYear = i + currentYear;
          this.expiryYear.append($('<option value="' + iYear + '">' + iYear + '</option>', context));
        }
        this.expiryYear.once('change').change(this.updateExpiryDate);
        this.expiryMonth.before(this.expiryYear);
      }

      $("#edit-bill-to-address-country").once('change').change(this.setAddressLabels);
    },

    updateExpiryDate: function() {
      expiryDate = $('#edit-card-expiry-date');
      expiryMonth = $('#commerce-cybersource-sasop-expiry-month');
      expiryYear = $('#commerce-cybersource-sasop-expiry-year');
      expiryDate.val(expiryMonth.val() + '-' + expiryYear.val());
    },

    setAddressLabels: function() {
      var region_label = "";
      var postal_code_label = "";
      var country_code = $("#edit-bill-to-address-country option:selected").val();

      if(country_code == "US") {
        region_label = Drupal.t('State');
        postal_code_label = Drupal.t('ZIP Code');
        $(".form-item-bill-to-address-state .description").show();
      } else if(country_code == "CA") {
        region_label = Drupal.t('Province');
        postal_code_label = Drupal.t('Postal Code');
        $(".form-item-bill-to-address-state .description").show();
      } else if(country_code == "MX") {
        region_label = Drupal.t('State');
        postal_code_label = Drupal.t('Postal Code');
        $(".form-item-bill-to-address-state .description").show();
      } else {
        region_label = Drupal.t('Province');
        postal_code_label = Drupal.t('Postal Code');
        $(".form-item-bill-to-address-state .description").hide();
      }

      $(".form-item-bill-to-address-state label span.label-text").html(region_label);
      $(".form-item-bill-to-address-postal-code label span.label-text").html(postal_code_label);
    }
  };
})(jQuery);
