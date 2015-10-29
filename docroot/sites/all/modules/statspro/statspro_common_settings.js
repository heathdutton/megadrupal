// $Id: statspro_common_settings.js,v 1.1.2.1 2010/06/10 17:53:48 rsevero Exp $

/**
 * Javascript auxiliar functions for statspro settings form.
 */

// Our namespace:
var StatsProSettings = StatsProSettings || {};

/**
 * Set the 'disabled' atttribute of the 'Custom number of days' text field
 * according to the type of period choosen.
 *
 */
StatsProSettings.controlsCustomNumberDaysAvailability = function() {
  if ($('#edit-statspro-period').val() == 'custom_days') {
    label_text = Drupal.t('Custom number of days:') + ' <span title="' +
      Drupal.t('This field is required.') + '" class="form-required">*</span>';
    $('label', '#edit-statspro-custom-number-days-wrapper').html(label_text);
    $('#edit-statspro-custom-number-days').removeAttr('disabled');
  }
  else {
    $('label', '#edit-statspro-custom-number-days-wrapper').html(Drupal.t('Custom number of days:'));
    $('#edit-statspro-custom-number-days').attr('disabled', 'disabled');
  }
}

/**
 * Configures the starting state of the form and sets the chenge event handler 
 * for the '#edit-statspro-period' text field.
 * 
 */
Drupal.behaviors.statspro_settings_init = function (context) {
  $('#edit-statspro-period', context).change(StatsProSettings.controlsCustomNumberDaysAvailability);
  StatsProSettings.controlsCustomNumberDaysAvailability();
}
