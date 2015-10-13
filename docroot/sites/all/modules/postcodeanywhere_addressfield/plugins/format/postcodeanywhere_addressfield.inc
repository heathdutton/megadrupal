<?php

/**
 * @file
 * A specific handler for GB.
 */

$plugin = array(
  'title' => t('Address form (Postcode Anywhere, for UK only)'),
  'format callback' => 'postcodeanywhere_addressfield_format_address_generate',
  'type' => 'address',
  'weight' => 10,
);

function postcodeanywhere_addressfield_format_address_generate(&$format, &$address, $context = array()) {
  $postcodeanywhere_web_services = postcodeanywhere_addressfield_web_services_check();
  if ($address['country'] == 'GB' && $context['mode'] == 'form' && $postcodeanywhere_web_services) {
    $required = $context['delta'] == 0 && $context['instance']['required'];
    $wrapper_id = $format['#wrapper_id'];
    $widget_type = variable_get('postcodeanywhere_addressfield_widget_type', 'button');
    // Rebuild the element_key, not always present in the address array,
    // to be used to create unique identifiers for postcodeanywhere buttons.
    // Useful to avoid ajax issues on forms with multiple addressfields instances.
    // @see https://drupal.org/node/2191109
    $instance = $context['instance'];
    $element_key = implode('_', array($instance['entity_type'], $instance['bundle'], $instance['field_name'], $context['langcode'], $context['delta']));

    // Initialize postcodeanywhere.
    if (empty($address['postcodeanywhere'])) {
      // Check if there is already an address.
      if (!empty($address['postal_code']) && !empty($address['locality'])) {
        // Check if is an valid UK postcode.
        $is_uk_postcode = postcodeanywhere_addressfield_get_addresses_by_postcode($address['postal_code']);
        if (!empty($is_uk_postcode)) {
          // Address defined, use the prefilled addressfield form.
          if (!empty($address['thoroughfare'])) {
            $address['postcodeanywhere'] = 'address_selected';
          }
          // Use the addresses selection for the given postcode.
          else {
            $address['postcodeanywhere'] = 'address_selection';
          }
        }
        // Search for postcode if the address is not an UK one.
        else {
          $address['postcodeanywhere'] = 'search';
        }
      }
      // Search for postcode if there is no address defined.
      else {
        $address['postcodeanywhere'] = 'search';
      }
    }

    if ($widget_type == 'button') {
      if($address['postcodeanywhere'] == 'search') {
        $format['postcodeanywhere_house_number'] = array(
          '#type' => 'textfield',
          '#title' => t('House number or name'),
          '#default_value' => !empty($address['postcodeanywhere_house_number']) ? $address['postcodeanywhere_house_number'] : '',
          '#element_validate' => array('postcodeanywhere_addressfield_house_number_validate'),
          '#weight' => $format['country']['#weight'] + 1,
          '#size' => 30,
        );
        $format['postcodeanywhere_postcode'] = array(
          '#type' => 'textfield',
          '#title' => t('Postcode'),
          '#default_value' => !empty($address['postcodeanywhere_postcode']) ? $address['postcodeanywhere_postcode'] : '',
          '#element_validate' => array('postcodeanywhere_addressfield_postcode_validate'),
          '#weight' => $format['country']['#weight'] + 2,
          '#size' => 15,
          '#required' => TRUE,
        );
        $format['postcodeanywhere_find_address'] = array(
          '#type' => 'submit',
          // Unique identifier for the button.
          '#name' => $element_key . '-postcodeanywhere_find_address',
          '#id' => $element_key . '-postcodeanywhere_find_address',
          '#value' => t('Find address'),
          '#element_validate' => array('postcodeanywhere_addressfield_find_address_validate'),
          '#executes_submit_callback' => FALSE,
          '#ajax' => array(
            'callback' => 'addressfield_standard_widget_refresh',
            'wrapper' => $wrapper_id,
          ),
          '#weight' => $format['country']['#weight'] + 4,
          '#attributes' => array('class' => array('search find-address')),
        );
        $format['postcodeanywhere_cancel'] = array(
          '#type' => 'submit',
          // Unique identifier for the button.
          '#name' => $element_key . '-postcodeanywhere_cancel',
          '#id' => $element_key . '-postcodeanywhere_cancel',
          '#value' => t('Do not know the postcode'),
          '#executes_submit_callback' => FALSE,
          '#element_validate' => array('postcodeanywhere_addressfield_cancel_search_validate'),
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
        if (variable_get('postcodeanywhere_addressfield_hide_extra_fields', TRUE)) {
          $format['name_block']['#access'] = FALSE;
          $format['organisation_block']['#access'] = FALSE;
        }
      }
      // Address selected after a search for House number.
      elseif($address['postcodeanywhere'] == 'address_selected') {
        $search_by_postcode_button = t('Search for another address');
      }
      // After a search for postcode only.
      elseif($address['postcodeanywhere'] == 'addresses_selection') {
        $search_by_postcode_button = t('Search by another postcode');
      }
      // If the search by postcode was canceled.
      elseif($address['postcodeanywhere'] == 'cancel') {
        $search_by_postcode_button = t('Search by postcode');
      }
      if (!empty($search_by_postcode_button)) {
        _postcodeanywhere_addressfield_search_by_postcode_button($format, $element_key, $search_by_postcode_button);
      }
    }
    // Use autocomplete widget for searching addresses by postcode.
    elseif ($widget_type == 'autocomplete') {
      $format['postcodeanywhere_postcode'] = array(
        '#type' => 'textfield',
        '#title' => t('Search address by Postcode'),
        '#default_value' => !empty($address['postcodeanywhere_postcode']) ? $address['postcodeanywhere_postcode'] : '',
        '#element_validate' => array('postcodeanywhere_addressfield_autocomplete_postcode_validate'),
        '#ajax' => array(
          'callback' => 'addressfield_standard_widget_refresh',
          'wrapper' => $wrapper_id,
        ),
        '#autocomplete_path' => 'postcodeanywhere-autocomplete',
        '#limit_validation_errors' => array(),
        '#weight' => $format['country']['#weight'] + 1,
      );
    }
    // Address selector if there are defined addresses for a postcode.
    if (!empty($address['postcodeanywhere_addresses']) && $address['postcodeanywhere'] == 'addresses_selection') {
      $postcodeanywhere_addresses_options = array('' => t('- Please select -')) + $address['postcodeanywhere_addresses'];
      $postcodeanywhere_addresses_select_default = !empty($address['postcodeanywhere_addresses_select']) ? $address['postcodeanywhere_addresses_select'] : NULL;
      $format['postcodeanywhere_addresses_select'] = array(
        '#type' => 'select',
        '#title' => t('Select address for this postcode'),
        '#options' => $postcodeanywhere_addresses_options,
        '#default_value' => $postcodeanywhere_addresses_select_default,
        '#element_validate' => array('postcodeanywhere_addressfield_addresses_select_validate'),
        '#ajax' => array(
          'callback' => 'addressfield_standard_widget_refresh',
          'wrapper' => $wrapper_id,
        ),
        '#render_option_value' => TRUE,
        '#limit_validation_errors' => array(),
        '#weight' => $format['country']['#weight'] + 2,
      );

      $format['postcodeanywhere_addresses'] = array(
        '#type' => 'value',
        '#value' => $address['postcodeanywhere_addresses'],
        '#weight' => $format['country']['#weight'] + 3,
      );
    }
  }
  // Alter UK addressfield format according with the mapping.
  if ($address['country'] == 'GB') {
    _postcodeanywhere_addressfield_gb_extra_address_elements($format);
  }
}

/**
 * Validate callback for the "Find address" button.
 */
function postcodeanywhere_addressfield_find_address_validate($element, &$form_state, &$form) {
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Validate callback for the postcode input element.
 */
function postcodeanywhere_addressfield_postcode_validate($element, &$form_state, &$form) {
  if (!empty($element['#value'])) {
    $addresses = postcodeanywhere_addressfield_get_addresses_by_postcode($element['#value']);
    if (empty($addresses)) {
      form_set_error(implode('][', $element['#array_parents']), t('There are no addresses for this postcode. Try another'));
    }
    else {
      $array_parents = array_slice($element['#parents'], 0, -1);
      $address = drupal_array_get_nested_value($form_state['values'], $array_parents);
      // Set the postcode as form element value.
      form_set_value($element, $element['#value'], $form_state);
      // Set the postcodeanywhere addresses options.
      $address['postcodeanywhere_addresses'] = $addresses;

      // Search for postcode only, clean the addressfield data,
      // use addresses selection for get the address.
      // prefill the locality, postcode, administrative_area for the new postcode.
      if (empty($address['postcodeanywhere']) || $address['postcodeanywhere'] != 'address_selected') {
        _postcodeanywhere_addressfield_empty_address($address);
        $address['postcodeanywhere'] = 'addresses_selection';
        $first_address = postcodeanywhere_addressfield_get_address_details(key($addresses));
        $address['locality'] = isset($first_address['post_town']) ? $first_address['post_town'] : '';
        $address['postal_code'] = isset($first_address['postcode']) ? $first_address['postcode'] : '';
        $address['administrative_area'] = isset($first_address['county']) ? $first_address['county'] : '';
        // Form state for addressfield format.
        $form_state['addressfield'][$address['element_key']] = array_diff_key($address, array('element_key' => ''));
        foreach ($address as $key => $value) {
          // Form state for addressfield in custom forms,
          // ex. Drupal commerce checkout form.
          drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array($key)), $value, TRUE);
          drupal_array_set_nested_value($form_state['input'], array_merge($array_parents, array($key)), $value, TRUE);
        }
      }
      // Search for House number also.
      else {
        // The changes needed are covered by House number validation.
        // @see postcodeanywhere_addressfield_house_number_validate().
        // @see _postcodeanywhere_addressfield_populate_address().
      }
    }
  }
}

/**
 * Validate callback for the house number input element.
 */
function postcodeanywhere_addressfield_house_number_validate($element, &$form_state, &$form) {
  if (!empty($element['#value'])) {
    $array_parents = array_slice($element['#parents'], 0, -1);
    $address = drupal_array_get_nested_value($form_state['values'], $array_parents);
    $addresses = postcodeanywhere_addressfield_get_addresses_by_postcode($address['postcodeanywhere_postcode']);
    if (!empty($addresses)) {
      $validate = FALSE;
      // Search in addresses for the house number data
      // and return the first matched address.
      foreach ($addresses as $address_id => $address_description) {
        if (stristr($address_description, $element['#value'])) {
          $validate = TRUE;
          _postcodeanywhere_addressfield_populate_address($address_id, $element, $form_state);
          return TRUE;
        }
      }
      if (!$validate) {
        form_set_error(implode('][', $element['#array_parents']), t('The house number or name is no correct for this postcode. Try another'));
      }
    }
  }
}

/**
 * Validate callback for the "cancel" button.
 */
function postcodeanywhere_addressfield_cancel_search_validate($element, &$form_state, &$form) {
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    $array_parents = array_slice($element['#parents'], 0, -1);
    $address = drupal_array_get_nested_value($form_state['values'], $array_parents);
    $address['postcodeanywhere'] = 'cancel';
    $form_state['addressfield'][$address['element_key']] = array_diff_key($address, array('element_key' => ''));
    $form_state['addressfield'][$address['element_key']]['postcodeanywhere'] = 'cancel';
    drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array('postcodeanywhere')), 'cancel', TRUE);
    drupal_array_set_nested_value($form_state['input'], array_merge($array_parents, array('postcodeanywhere')), 'cancel', TRUE);
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Validate callback for the "Search by postcode" button, back to search form.
 */
function postcodeanywhere_addressfield_search_by_postcode_validate($element, &$form_state, &$form) {
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    $array_parents = array_slice($element['#parents'], 0, -1);
    $address = drupal_array_get_nested_value($form_state['values'], $array_parents);
    $address['postcodeanywhere'] = 'search';
    $form_state['addressfield'][$address['element_key']] = array_diff_key($address, array('element_key' => ''));
    drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array('postcodeanywhere_addresses')), NULL, TRUE);
    $form_state['addressfield'][$address['element_key']]['postcodeanywhere_addresses'] = NULL;
    $form_state['addressfield'][$address['element_key']]['postcodeanywhere'] = 'search';
    drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array('postcodeanywhere')), 'search', TRUE);
    drupal_array_set_nested_value($form_state['input'], array_merge($array_parents, array('postcodeanywhere')), 'search', TRUE);
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Validation after autocomplete selection of a postcodeanywhere address.
 * 
 * Rebuild the form with populating the addressfield.
 */
function postcodeanywhere_addressfield_autocomplete_postcode_validate($element, &$form_state, &$form) {
  if (!empty($element['#value'])) {
    // Validation after autocomplete selection.
    list($address_id, $postcode) = array_merge(explode('-', $element['#value']), array(FALSE));
    $array_parents = array_slice($element['#parents'], 0, -1);

    if ($postcode) {
      $addresses = postcodeanywhere_addressfield_get_addresses_by_postcode($postcode);
      if (!empty($addresses)) {
        // Set the postcode as form element value.
        form_set_value($element, $postcode, $form_state);
        // Set the postcodeanywhere addresses options.
        drupal_array_set_nested_value($form_state['values'], array_merge($array_parents, array('postcodeanywhere_addresses')), $addresses, TRUE);
        // Populate addressfield with the selected postcode address data.
        _postcodeanywhere_addressfield_populate_address($address_id, $element, $form_state);
      }
    }
  }
}

/**
 * Validate callback.
 *
 * Rebuild the form with populating the addressfield.
 */
function postcodeanywhere_addressfield_addresses_select_validate($element, &$form_state, &$form) {
  if ($element['#id'] == $form_state['triggering_element']['#id']) {
    if (!empty($element['#value'])) {
      $address_id = $element['#value'];
      // Populate addressfield with the selected postcode address data.
      _postcodeanywhere_addressfield_populate_address($address_id, $element, $form_state);
    }
  }
}

/**
 * Helper function.
 *
 * Add the other addressfield elements required by PostocodeAnywhere-Addressfield mapping.
 */
function _postcodeanywhere_addressfield_gb_extra_address_elements(&$format) {
  // New UK address elements.
  // Add Sub Premise element if it is not defined.
  if (empty($format['street_block']['sub_premise'])) {
    $format['street_block']['sub_premise'] = array(
      '#title' => t('Flat No.'),
      '#tag' => 'div',
      '#attributes' => array('class' => array('sub-premise')),
      '#size' => 30,
      '#weight' => -10,
    );
    // Re-order the street elements considering "Sub Premise" weight.
    $format['street_block']['premise']['#weight'] = -9;
  }
  if (empty($format['locality_block']['dependent_locality'])) {
    $format['locality_block']['dependent_locality'] = array(
      '#title' => t('Address 2'),
      '#tag' => 'div',
      '#attributes' => array('class' => array('dependent-locality')),
      '#size' => 30,
    );
    // Rename the elements considering "Dependant Locality" label.
    $format['street_block']['premise']['#title'] = t('House Name/Building');
  }
}

/**
 * Helper function.
 *
 * Build the "Search by postcode" button for back to postcode search.
 */
function _postcodeanywhere_addressfield_search_by_postcode_button(&$format, $element_key, $button_label = 'Search by postcode') {
  $format['postcodeanywhere_search_by_postcode'] = array(
    '#type' => 'button',
    // Unique identifier for the button.
    '#name' => $element_key . '-postcodeanywhere_search_by_postcode',
    '#id' => $element_key . '-postcodeanywhere_search_by_postcode',
    '#value' => $button_label,
    '#executes_submit_callback' => FALSE,
    '#element_validate' => array('postcodeanywhere_addressfield_search_by_postcode_validate'),
    '#ajax' => array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
    ),
    '#limit_validation_errors' => array(),
    '#weight' => $format['country']['#weight'] + 1,
    '#attributes' => array('class' => array('search-by-postcode')),
  );
}
