<?php

/**
 * @file
 * Provides callbacks for the IP Authentication module.
 */

/**
 * Authenticate call.
 *
 * @see services_ipauth_services_authentication_info()
 */
function _services_ipauth_authenticate_call($settings) {
  // Get remote IP address.
  $current_ip = ip_address();

  // Prepare list of user defined addresses and ranges.
  $ip_ranges = array_filter($settings['services_ipauth_addresses'], 'services_ipauth_is_ip_range');
  $ip_addresses = array_diff($settings['services_ipauth_addresses'], $ip_ranges);

  // Check whether filtering method is Deny.
  $filtering_method_deny = $settings['services_ipauth_type'] == SERVICES_IPAUTH_TYPE_DENY;

  // Check whether the current ip is in the list.
  $ip_address_in_list = in_array($current_ip, $ip_addresses);

  $ip_address_in_range = FALSE;
  // Don't need to check range if the ip is already in the listed ips.
  if (!$ip_address_in_list) {
    foreach ($ip_ranges as $ip_range) {
      if (services_ipauth_ip_is_in_range($current_ip, $ip_range)) {
        $ip_address_in_range = TRUE;
        break;
      }
    }
  }

  $ip_match = $ip_address_in_list || $ip_address_in_range;
  // Deny access if the ip address is in the list and we use Deny method,
  // or if the ip address is not in the list and we use Allow method.
  if (($ip_match && $filtering_method_deny) || (!$filtering_method_deny && !$ip_match)) {

    return t('IP address access denied.');
  }
}

/**
 * Security settings form.
 *
 * @see services_ipauth_services_authentication_info()
 */
function _services_ipauth_security_settings($settings) {
  $form = array();
  if (!is_array($settings)) {
    $settings = array();
  }
  $form['services_ipauth_type'] = array(
    '#title' => t('Type of IP filtering'),
    '#type' => 'radios',
    '#options' => array(
      SERVICES_IPAUTH_TYPE_DENY => t('Deny'),
      SERVICES_IPAUTH_TYPE_ALLOW => t('Allow'),
    ),
    '#description' => t('Choose the method of IP address filtering. "Allow" means you need to list only addresses that can have access. All others will be denied. "Deny" means access will be granted to every address except listed below'),
    '#default_value' => isset($settings['services_ipauth_type']) ? $settings['services_ipauth_type'] : SERVICES_IPAUTH_TYPE_DENY,
  );

  $form['services_ipauth_addresses'] = array(
    '#title' => t('IP addresses'),
    '#type' => 'textarea',
    '#description' => t('One IP address per line.'),
    '#default_value' => isset($settings['services_ipauth_addresses']) ? implode("\n", $settings['services_ipauth_addresses']) : '',
    '#element_validate' => array('_services_ipauth_ips_validate'),
  );
  return $form;
}

/**
 * #element_validate callback to validate IP addresses (one per line).
 *
 * @see _services_ipauth_security_settings()
 */
function _services_ipauth_ips_validate($element, &$form_state) {
  // Take user input.
  $ip_addresses_input = $element['#value'];

  // Convert user input to an associative array with non-empty unique values.
  $ip_addresses = drupal_map_assoc(array_filter(array_map('trim', array_unique(explode("\n", $ip_addresses_input)))));

  // Definition array of filter_var_array() can not have numeric keys. Change
  // the key of the numeric values so that they are interpreted as a string.
  foreach ($ip_addresses as $key => $address) {
    if (is_numeric($key)) {
      unset($ip_addresses[$key]);
      $ip_addresses[$key . 'x'] = $address;
    }
  }

  // Create the definition array for filter_var_array().
  $definition = array_combine(array_keys($ip_addresses), array_fill(0, count($ip_addresses), array('filter' => FILTER_VALIDATE_IP)));

  // filter_var_array() returns FALSE on failure, or an array with a result for
  // each key of the filtered array. If the result does not contain FALSE, all
  // elements of the array passed validation. array_filter() filters out values
  // that, when converted to boolean, equal FALSE. So if the filtered result
  // equals the normal result, all elements passed validation.
  if (!($result = filter_var_array($ip_addresses, $definition)) || array_filter($result) != $result) {
    $invalids = array_diff_key($ip_addresses, array_filter($result));
    foreach ($invalids as $key => $invalid) {
      if (services_ipauth_is_valid_ip_range($invalid)) {
        unset($invalids[$key]);
      }
    }
    if (!empty($invalids)) {
      form_error($element, t('Incorrect IP address format for following entries: @list', array('@list' => theme('item_list', $invalids))));
    }
  }

  // Set the value to an array, this way it is easier to work with the value
  // that will be stored in the $settings array.
  form_set_value($element, array_values($ip_addresses), $form_state);
}
