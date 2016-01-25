/**
 * @file
 */
function maildigest_frequency_callback(select) {
  jQuery('.maildigest-frequency-field').hide();
  if (jQuery(select).val() == 'weekly') {
    jQuery('#maildigest-weekly-wrapper').show();
  }
  else if (jQuery(select).val() == 'monthly') {
    jQuery('#maildigest-monthly-wrapper').show();
  }
}
