/**
 * @file
 * Appointment Calendar Node javascript functions.
 */

Drupal.behaviors.appointment_calendar = {
    attach: function (context) {
      var pathname = window.location.pathname;
      if (pathname == '/appointcal') {
        jQuery('div[class*="menu"]').hide();
      }
      if (jQuery("body select[name='appointment_slot[und]']").val() == '_none') {
        jQuery('#edit-check').hide();
        jQuery('.error-calendar').html('No Slot selected/available.');
      }
      else {
        jQuery('#edit-check').show();
        jQuery('.error-calendar').hide();
      }
      jQuery("body select[name='appointment_slot[und]']").change(function () {
            var result = jQuery("select[name='appointment_slot[und]']").val()
            if (result != '_none') {
                result = result.split('-');
            }
      });
      jQuery("#edit-check").hover(function () {
            var result = jQuery("select[name='appointment_slot[und]']").val();
            if (result != '_none') {
                result = result.split('-');
                jQuery("#edit-appointment-date-und-0-value-timeEntry-popup-1").val(result[0]);
            }
      });
      jQuery("#edit-submit").hover(function () {
            var result = jQuery("select[name='appointment_slot[und]']").val()
            if (result != '_none') {
                result = result.split('-');
                jQuery("#edit-appointment-date-und-0-value-timeEntry-popup-1").val(result[0]);
            }
      });
      jQuery('.form-item-appointment-date-und-0-value-time').hide();
    }
};
