/**
 * @file
 * Small helper JS for the wish list settings form address.
 */

/**
 * Apply the selected address to the appropriate fields in the cart form.
 */
function apply_address(type, address_str) {
  if (address_str == '0') {
    return;
  }

  eval('var address = ' + address_str + ';');

  jQuery('#edit-' + type + '-first-name').val(address.first_name).trigger('change');
  jQuery('#edit-' + type + '-last-name').val(address.last_name).trigger('change');
  jQuery('#edit-' + type + '-phone').val(address.phone).trigger('change');
  jQuery('#edit-' + type + '-company').val(address.company).trigger('change');
  jQuery('#edit-' + type + '-street1').val(address.street1).trigger('change');
  jQuery('#edit-' + type + '-street2').val(address.street2).trigger('change');
  jQuery('#edit-' + type + '-city').val(address.city).trigger('change');
  jQuery('#edit-' + type + '-postal-code').val(address.postal_code).trigger('change');

  if (jQuery('#edit-' + type + '-country').val() != address.country) {
    jQuery('#edit-' + type + '-country').val(address.country);
    try {
      uc_update_zone_select('#edit-' + type + '-country', address.zone);
    }
    catch (err) {}
  }

  jQuery('#edit-' + type + '-zone').val(address.zone).trigger('change');
}
