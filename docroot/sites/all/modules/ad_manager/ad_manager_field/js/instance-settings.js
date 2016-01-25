(function ($) {
'use strict';
var select
  , providers
  , options
  , ads;

function checkboxChanged () {
  var checked = [];
  // Determine which proviers are selected.
  providers.filter(':checked').each(function () {
    checked.push($(this).val());
  });

  options.each(function () {
    // If no providers are checked or if the ad unit belongs to one of the
    // the checked providers.
    if (!checked.length ||
        $.inArray(ads[$(this).val()].provider, checked) !== -1) {
      $(this).removeAttr('disabled');
    }
    else {
      $(this).attr('disabled', 'disabled');
    }
  });

  // Set an error state on the selected value if it is from a non-checked
  // provider.
  if (checked.length &&
      $.inArray(ads[select.val()].provider, checked) === -1) {
    select.addClass('error');
  }
  else {
    select.removeClass('error');
  }
}

Drupal.behaviors.adManagerFieldInstanceSettings = {
  attach: function (context, settings) {
    var checkboxes = $('#edit-instance-settings-ad-providers')
      .once('adManagerFieldInstanceSettings')
      .find('input[type="checkbox"]')
        .bind('change', checkboxChanged);

    // Store our jQuery collection for checkboxes and our select field.
    if (typeof providers === 'undefined') {
      providers = checkboxes;
      select = $('#edit-field-ad-cell-unit-und-0-ad')
        .bind('change', checkboxChanged);
      options = select.children();
      ads = settings.adManagerField.ads;
      // Initialize to show/hide the units.
      checkboxChanged();
    }
  }
};
})(jQuery);
