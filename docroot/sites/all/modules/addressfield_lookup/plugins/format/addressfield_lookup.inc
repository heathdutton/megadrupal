<?php

/**
 * @file
 * Address Field handler based on the 'Address Field Lookup' module API.
 */

$plugin = array(
  'title' => t('Address Field Lookup'),
  'format callback' => 'addressfield_lookup_addressfield_format_generate',
  'type' => 'address',
  'weight' => 10,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_lookup_addressfield_format_generate(&$format, &$address, $context = array()) {
  // Only run in when the address field form is shown.
  if ($context['mode'] == 'form') {
    // Check there is a default 'address field lookup service'.
    if (addressfield_lookup_get_default_service()) {
      // Get the address wrapper ID.
      $wrapper_id = $format['#wrapper_id'];

      // Rebuild the element_key, not always present in the address array,
      // to be used to create unique identifiers for address field lookup
      // buttons. Useful to avoid ajax issues on forms with multiple
      // addressfields instances.
      //
      // @see https://drupal.org/node/2191109
      $instance = $context['instance'];
      $element_key = implode('_', array(
        $instance['entity_type'],
        $instance['bundle'],
        $instance['field_name'],
        $context['langcode'],
        $context['delta'],
      ));

      // Initialize address field lookup.
      if (empty($address['addressfield_lookup_mode'])) {
        // Check if there is already an address.
        if (!empty($address['postal_code']) && !empty($address['locality'])) {
          // Check if we already have some address results.
          $addresses = addressfield_lookup_get_addresses($address['postal_code']);

          // We have a valid postal code and some addresses.
          if (!empty($addresses)) {
            // Address defined, use the prefilled addressfield form.
            if (!empty($address['thoroughfare'])) {
              $address['addressfield_lookup_mode'] = 'address_selected';
            }
            else {
              // Use the addresses selection for the given postal code.
              $address['addressfield_lookup_mode'] = 'address_selection';
            }
          }
          else {
            // We have no addresses, so we need to do a search.
            $address['addressfield_lookup_mode'] = 'search';
          }
        }
        elseif (!empty($address['postal_code']) && !empty($address['thoroughfare'])) {
          // If we have a postcode and thoroughfare we're most likely editing
          // an existing address.
          $address['addressfield_lookup_mode'] = 'address_selected';
        }
        else {
          // We have no postal code, so we need to do a search.
          $address['addressfield_lookup_mode'] = 'search';
        }
      }

      // Set up for address field lookup form elements based on the mode
      // determined above.
      switch ($address['addressfield_lookup_mode']) {
        case 'search':
          // House number.
          $format['addressfield_lookup_house_number'] = array(
            '#type' => 'textfield',
            '#title' => t('House number or name'),
            '#default_value' => !empty($address['addressfield_lookup_house_number']) ? $address['addressfield_lookup_house_number'] : '',
            '#element_validate' => array('addressfield_lookup_addressfield_house_number_validate'),
            '#weight' => $format['country']['#weight'] + 1,
            '#size' => 30,
          );

          // Postal code.
          $format['addressfield_lookup_postal_code'] = array(
            '#type' => 'textfield',
            '#title' => t('Postcode'),
            '#default_value' => !empty($address['addressfield_lookup_postal_code']) ? $address['addressfield_lookup_postal_code'] : '',
            '#element_validate' => array('addressfield_lookup_addressfield_postal_code_validate'),
            '#weight' => $format['country']['#weight'] + 2,
            '#size' => 15,
            '#required' => TRUE,
          );

          // Find address button.
          $format['addressfield_lookup_find_address'] = array(
            '#type' => 'submit',
            '#name' => $element_key . '-addressfield_lookup_find_address',
            '#id' => $element_key . '-addressfield_lookup_find_address',
            '#value' => t('Find address'),
            '#element_validate' => array('addressfield_lookup_addressfield_find_address_validate'),
            '#executes_submit_callback' => FALSE,
            '#ajax' => array(
              'callback' => 'addressfield_standard_widget_refresh',
              'wrapper' => $wrapper_id,
            ),
            '#weight' => $format['country']['#weight'] + 4,
            '#attributes' => array('class' => array('search find-address')),
          );

          // Cancel link.
          $format['addressfield_lookup_cancel'] = array(
            '#type' => 'submit',
            '#name' => $element_key . '-addressfield_lookup_cancel',
            '#id' => $element_key . '-addressfield_lookup_cancel',
            '#value' => t('Do not know the postcode'),
            '#executes_submit_callback' => FALSE,
            '#element_validate' => array('addressfield_lookup_addressfield_cancel_search_validate'),
            '#ajax' => array(
              'callback' => 'addressfield_standard_widget_refresh',
              'wrapper' => $wrapper_id,
            ),
            '#limit_validation_errors' => array(),
            '#weight' => $format['country']['#weight'] + 3,
            '#attributes' => array('class' => array('cancel cancel-search')),
          );

          // Hide the other addressfield form elements.
          $format['street_block']['#access'] = FALSE;
          $format['locality_block']['#access'] = FALSE;

          // Hide additional fields if the settings require.
          if (variable_get('addressfield_lookup_addressfield_hide_extra_fields', TRUE)) {
            $format['name_block']['#access'] = FALSE;
            $format['organisation_block']['#access'] = FALSE;
          }

          break;

        case 'address_selected':
          // Address selected after a search for House number.
          _addressfield_lookup_search_by_postal_code_button($format, $element_key, t('Search for another address'));

          break;

        case 'address_selection':
          // After a search for postal code only.
          _addressfield_lookup_search_by_postal_code_button($format, $element_key, t('Search by another postal code'));

          break;

        case 'cancel':
          // If the search by postal code was canceled.
          _addressfield_lookup_search_by_postal_code_button($format, $element_key, t('Search by postal code'));

          break;
      }

      // Address selector if there are defined addresses for a postcode.
      if (!empty($address['addressfield_lookup_addresses']) && $address['addressfield_lookup_mode'] == 'address_selection') {
        // Build out the options list for the form element.
        $addressfield_lookup_address_options = array('' => t('- Please select -'));

        // Add the address from the service call.
        $addressfield_lookup_address_options += $address['addressfield_lookup_addresses'];

        // Build the drop down.
        $format['addressfield_lookup_addresses_select'] = array(
          '#type' => 'select',
          '#title' => t('Select address for this postal code'),
          '#options' => $addressfield_lookup_address_options,
          '#default_value' => !empty($address['addressfield_lookup_addresses_select']) ? $address['addressfield_lookup_addresses_select'] : NULL,
          '#element_validate' => array('addressfield_lookup_addresses_select_validate'),
          '#ajax' => array(
            'callback' => 'addressfield_standard_widget_refresh',
            'wrapper' => $wrapper_id,
          ),
          '#render_option_value' => TRUE,
          '#limit_validation_errors' => array(),
          '#weight' => $format['country']['#weight'] + 2,
        );

        // Hidden variable for the address options.
        $format['addressfield_lookup_addresses'] = array(
          '#type' => 'value',
          '#value' => $address['addressfield_lookup_addresses'],
          '#weight' => $format['country']['#weight'] + 3,
        );
      }
    }
    else {
      watchdog('addressfield_lookup', 'Address field lookup handler could not initialise as there are no available lookup services.', array(), WATCHDOG_WARNING);
    }
  }

  // Get any format updates from the default address lookup service.
  addressfield_lookup_get_format_updates($format, $address);
}

/**
 * Validate callback for the house number input element.
 */
function addressfield_lookup_addressfield_house_number_validate($element, &$form_state, &$form) {
  // Check there is a value to validate.
  if (!empty($element['#value'])) {
    // Get the address element from the form.
    $array_parents = array_slice($element['#parents'], 0, -1);
    $address = drupal_array_get_nested_value($form_state['values'], $array_parents);

    // Get the matching addresses.
    $addresses = addressfield_lookup_get_addresses($address['addressfield_lookup_postal_code']);

    // Only validate if there are addresses.
    if (!empty($addresses)) {
      $validate = FALSE;

      // Prepare list options from the lookup results.
      $address_options = _addressfield_lookup_prepare_options($addresses);

      // Search in addresses for the house number data and return the first
      // matched address.
      foreach ($address_options as $address_id => $address_description) {
        // The house number is valid.
        if (stristr($address_description, $element['#value'])) {
          // Set the validate flag to true and populate the address.
          $validate = TRUE;
          _addressfield_lookup_populate_addressfield($address_id, $element, $form_state);

          break;
        }
      }

      // Show an error if the validation failed.
      if (!$validate) {
        form_set_error(implode('][', $element['#array_parents']), t('The house number or name is not correct for this postal code. Please try another.'));
      }
    }
  }
}

/**
 * Validate callback for the postal code input element.
 */
function addressfield_lookup_addressfield_postal_code_validate($element, &$form_state, &$form) {
  // Check there is a value to validate.
  if (!empty($element['#value'])) {
    // Perform an address lookup for this postcode.
    $addresses = addressfield_lookup_get_addresses($element['#value']);

    // Show an error if we have no results.
    if (empty($addresses)) {
      form_set_error(implode('][', $element['#array_parents']), t('Unfortunately there are no matching addresses for this postal code. Please try another.'));
    }
    else {
      // Get the address element from the form.
      $array_parents = array_slice($element['#parents'], 0, -1);
      $address = drupal_array_get_nested_value($form_state['values'], $array_parents);

      // Set the postal code as form element value.
      form_set_value($element, $element['#value'], $form_state);

      // Prepare list options from the lookup results.
      $address['addressfield_lookup_addresses'] = _addressfield_lookup_prepare_options($addresses);

      // Prefill the common details for each of the addresses (locality,
      // postcode, administrative_area) on the form.
      if (empty($address['addressfield_lookup_mode']) || $address['addressfield_lookup_mode'] != 'address_selected') {
        // Reset the address to the default values.
        _addressfield_lookup_default_values($address);

        // Set the mode.
        $address['addressfield_lookup_mode'] = 'address_selection';

        // Get the full details of the 1st address in the results.
        $first_address_details = addressfield_lookup_get_address_details($addresses[0]['id']);

        // Set the common details.
        $address['locality'] = isset($first_address_details['locality']) ? $first_address_details['locality'] : '';
        $address['postal_code'] = isset($first_address_details['postal_code']) ? $first_address_details['postal_code'] : '';
        $address['administrative_area'] = isset($first_address_details['administrative_area']) ? $first_address_details['administrative_area'] : '';

        // Update the form state, where possible.
        if (isset($address['element_key']) && isset($form_state['addressfield'][$address['element_key']])) {
          $form_state['addressfield'][$address['element_key']] = array_diff_key($address, array('element_key' => ''));
        }

        // Set the address values on the form.
        foreach ($address as $key => $value) {
          // Update the form state for addressfield.
          // Note: form_state['input'] must be updated so that
          // addressfield_lookup_addressfield_format_generate() has correct
          // information during the rebuild.
          drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array($key)), $value, TRUE);
          drupal_array_set_nested_value($form_state['input'], array_merge($array_parents, array($key)), $value, TRUE);
        }
      }
    }
  }
}

/**
 * Validate callback for the "Find address" button.
 */
function addressfield_lookup_addressfield_find_address_validate($element, &$form_state, &$form) {
  // Check if this was the triggering element.
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Validate callback for the "cancel" button.
 */
function addressfield_lookup_addressfield_cancel_search_validate($element, &$form_state, &$form) {
  // Check if this was the triggering element.
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    // Get the address element from the form.
    $array_parents = array_slice($element['#parents'], 0, -1);
    $address = drupal_array_get_nested_value($form_state['values'], $array_parents);

    // Set the mode.
    $address['addressfield_lookup_mode'] = 'cancel';

    // Update the form state, where possible.
    if (isset($address['element_key']) && isset($form_state['addressfield'][$address['element_key']])) {
      $form_state['addressfield'][$address['element_key']] = array_diff_key($address, array('element_key' => ''));
      $form_state['addressfield'][$address['element_key']]['addressfield_lookup_mode'] = 'cancel';
    }

    // Update the form state for addressfield.
    // Note: form_state['input'] must be updated so that
    // addressfield_lookup_addressfield_format_generate() has correct
    // information during the rebuild.
    drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array('addressfield_lookup_mode')), 'cancel', TRUE);
    drupal_array_set_nested_value($form_state['input'], array_merge($array_parents, array('addressfield_lookup_mode')), 'cancel', TRUE);

    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Validate callback for "Search by postal code" button, back to search form.
 */
function addressfield_lookup_addressfield_search_by_postal_code_validate($element, &$form_state, &$form) {
  // Check if this was the triggering element.
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    // Get the address element from the form.
    $array_parents = array_slice($element['#parents'], 0, -1);
    $address = drupal_array_get_nested_value($form_state['values'], $array_parents);

    // Set the mode.
    $address['addressfield_lookup_mode'] = 'search';

    $form_state['addressfield'][$address['element_key']] = array_diff_key($address, array('element_key' => ''));
    drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array('addressfield_lookup_addresses')), NULL, TRUE);

    $form_state['addressfield'][$address['element_key']]['addressfield_lookup_addresses'] = NULL;
    $form_state['addressfield'][$address['element_key']]['addressfield_lookup_mode'] = 'search';

    // Update the form state for addressfield.
    // Note: form_state['input'] must be updated so that
    // addressfield_lookup_addressfield_format_generate() has correct
    // information during the rebuild.
    drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array('addressfield_lookup_mode')), 'search', TRUE);
    drupal_array_set_nested_value($form_state['input'], array_merge($array_parents, array('addressfield_lookup_mode')), 'search', TRUE);

    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Validate callback: rebuild the form and populate the addressfield.
 */
function addressfield_lookup_addresses_select_validate($element, &$form_state, &$form) {
  // Check if this was the triggering element.
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    if (!empty($element['#value'])) {
      // Get the address ID.
      $address_id = $element['#value'];

      // Populate addressfield with the selected postal code address data.
      _addressfield_lookup_populate_addressfield($address_id, $element, $form_state);
    }
  }
}

/**
 * Build the "Search by postal code" button for back to postcode search.
 */
function _addressfield_lookup_search_by_postal_code_button(&$format, $element_key, $button_label = 'Search by postal code') {
  $format['addressfield_lookup_search_by_postal_code'] = array(
    '#type' => 'button',
    '#name' => $element_key . '-addressfield_lookup_search_by_postal_code',
    '#id' => $element_key . '-addressfield_lookup_search_by_postal_code',
    '#value' => $button_label,
    '#executes_submit_callback' => FALSE,
    '#element_validate' => array('addressfield_lookup_addressfield_search_by_postal_code_validate'),
    '#ajax' => array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
    ),
    '#limit_validation_errors' => array(),
    '#weight' => $format['country']['#weight'] + 1,
    '#attributes' => array('class' => array('search-by-postal-code')),
  );
}
