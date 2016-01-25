
(function ($) {

  Drupal.eCommerce = {};

  Drupal.eCommerce.setSelectState = function () {
    var country = $(this).val();

    if (Drupal.settings.eCommerce.regions[country]['states'] && (Drupal.settings.eCommerce.regions[country]['states'].length || Drupal.settings.eCommerce.regions[country]['states'].length == undefined)) {
      var country;
      var current_state = $(this)
        .parents('.address-form')
        .find('.state-text-wrapper input')
        .val();

      $(this)
        .parents('.address-form')
        .find('.state-text-wrapper')
        .hide();
      $(this)
        .parents('.address-form')
        .find('.state-select-wrapper')
        .each(function () {
          var options = '<option value="">' + Drupal.t('Select a ' + Drupal.settings.eCommerce.regions[country]['state_name'].toLowerCase()) + '</option>';
          for (state in Drupal.settings.eCommerce.regions[country]['states']) {
            options += '<option value="' + state + '"' + (state == current_state ? ' selected="selected"' : '') + '>' + Drupal.settings.eCommerce.regions[country]['states'][state] + '</option>';
          }
          $('select', this).html(options);
          Drupal.eCommerce.stateUpdateTextField.apply($('select', this));
        })
        .show()
    }
    else {
      $(this)
        .parents('.address-form')
        .find('.state-text-wrapper')
        .show();
      $(this)
        .parents('.address-form')
        .find('.state-select-wrapper')
        .hide();
    }

    $(this)
      .parents('.address-form')
      .find('.state-text-wrapper label, .state-select-wrapper label')
      .html(Drupal.settings.eCommerce.regions[country]['state_name']);
    $(this)
      .parents('.address-form')
      .find('.zip-wrapper label')
      .html(Drupal.settings.eCommerce.regions[country]['zip_name']);
  }

  Drupal.eCommerce.stateUpdateTextField = function () {
    $(this)
      .parents('.address-form')
      .find('.state-text-wrapper input')
      .val($(this).val());
  }

  Drupal.eCommerce.stateSelectAutoAttach = function () {
    Drupal.eCommerce.setSelectState.apply(this);

    $(this)
      .change(Drupal.eCommerce.setSelectState)
      .parents('.address-form')
      .find('.state-select-wrapper select')
      .change(Drupal.eCommerce.stateUpdateTextField);
  }

  Drupal.behaviors.addressCountryState = {
    attach: function (context) {
      $('select.address-country-id', context)
        .each(Drupal.eCommerce.stateSelectAutoAttach);
    }
  }

})(jQuery);