<?php

/**
 * @file
 * Admin functionality for the Address Field lookup module.
 */

/**
 * Form builder: Overview form for address field lookup services.
 */
function addressfield_lookup_services_overview_form() {
  // Get the list of available services.
  $addressfield_lookup_services = addressfield_lookup_services();

  // Initialize the form.
  $form = array();

  // Build list of all services.
  $options = array();

  // Check we have some services.
  if (count($addressfield_lookup_services)) {
    foreach ($addressfield_lookup_services as $machine_name => $addressfield_lookup_service) {
      // Add the service name.
      $form['services'][$machine_name] = array(
        '#markup' => t('@service_name <small> (Machine name: @service_machine_name)</small>', array('@service_name' => $addressfield_lookup_service['name'], '@service_machine_name' => $machine_name)),
      );

      // Add the config path (if present).
      if (isset($addressfield_lookup_service['config path'])) {
        $form['services'][$machine_name]['config'] = array(
          '#markup' => l(t('configure'), $addressfield_lookup_service['config path']),
        );
      }

      // Add to the options list.
      $options[$machine_name] = '';
    }

    $form['addressfield_lookup_default_service'] = array(
      '#type' => 'radios',
      '#title' => t('Default address field lookup service'),
      '#title_display' => 'invisible',
      '#options' => $options,
      '#default_value' => variable_get('addressfield_lookup_default_service', key($options)),
    );

    $form['actions'] = array(
      '#type' => 'actions',
    );

    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );

    $form['actions']['test'] = array(
      '#type' => 'submit',
      '#value' => t('Test default service'),
      '#submit' => array('addressfield_lookup_services_test_default_service'),
    );
  }
  else {
    // There are no services.
    drupal_set_message(t('There are currently no address field lookup services available.'), 'error');
  }

  // Add our custom theme for the form.
  $form['#theme'] = 'addressfield_lookup_services_overview_form';

  return $form;
}

/**
 * Returns HTML for the address field lookup services overview form.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 */
function theme_addressfield_lookup_services_overview_form(array $variables) {
  $form = $variables['form'];

  $output = '';

  // Loop through the available services.
  if (!empty($form['services'])) {
    foreach ($form['services'] as $key => $element) {
      // Ignore form control structures.
      if (is_array($element) && element_child($key)) {
        // Add invisible labels for radio buttons in the table for
        // accessibility. These changes are only required and valid when the
        // form is themed as a table, so it would be wrong to perform them in
        // the form constructor.
        $service_name = drupal_render($form['services'][$key]);
        $form['addressfield_lookup_default_service'][$key]['#title'] = t('Set !title as default', array('!title' => $service_name));
        $form['addressfield_lookup_default_service'][$key]['#title_display'] = 'invisible';

        // Add the row.
        $rows[] = array(
          'service_name' => $service_name,
          'default' => drupal_render($form['addressfield_lookup_default_service'][$key]),
          'operations' => isset($form['services'][$key]['config']) ? drupal_render($form['services'][$key]['config']) : '',
        );
      }
    }

    // Table headers.
    $headers = array(
      'service_name' => t('Service Name'),
      'default' => t('Default Service'),
      'operations' => t('Operations'),
    );

    $output = theme('table', array(
      'header' => $headers,
      'rows' => $rows,
    ));

    $output .= drupal_render_children($form);
  }

  return $output;
}

/**
 * Submit handler: Submit the address field lookup services overview form.
 */
function addressfield_lookup_services_overview_form_submit($form, &$form_state) {
  // Set the default service.
  variable_set('addressfield_lookup_default_service', $form_state['values']['addressfield_lookup_default_service']);

  // Show a message.
  drupal_set_message(t('Configuration saved.'));
}

/**
 * Submit handler: Test the default address field lookup services.
 */
function addressfield_lookup_services_test_default_service($form, &$form_state) {
  // Get the default service.
  $addressfield_lookup_default_service = addressfield_lookup_get_default_service();

  // Check there is some test data.
  if (!isset($addressfield_lookup_default_service['test data'])) {
    drupal_set_message(t('Could not test the default service (%service_name) as it does not define any test data.', array('%service_name' => $addressfield_lookup_default_service['name'])), 'warning');
    return FALSE;
  }

  // Peform an address search with the default service test data.
  if ($test_addresses = addressfield_lookup_get_addresses($addressfield_lookup_default_service['test data'], TRUE, TRUE)) {
    // Run secondary test to get the full details of the 1st result.
    if ($test_address_details = addressfield_lookup_get_address_details($test_addresses[0]['id'], TRUE, TRUE)) {
      // Tidy up.
      unset($test_addresses);
      unset($test_address_details);

      // The test passed.
      drupal_set_message(t('The default service (%service_name) test was successful.', array('%service_name' => $addressfield_lookup_default_service['name'])));
      return TRUE;
    }
    else {
      // The test failed.
      drupal_set_message(t('The default service (%service_name) test failed. The full address details lookup failed.', array('%service_name' => $addressfield_lookup_default_service['name'])), 'error');
      return FALSE;
    }
  }
  else {
    // The test failed.
    drupal_set_message(t('The default service (%service_name) test failed. The address lookup failed.', array('%service_name' => $addressfield_lookup_default_service['name'])), 'error');
    return FALSE;
  }
}

/**
 * Form builder: Settings form.
 */
function addressfield_lookup_settings_form($form, &$form_state) {
  // Extra fields.
  $form['addressfield_lookup_addressfield_hide_extra_fields'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide additional fields'),
    '#description' => t('Do not show fields such as names or company on the address field lookup form.'),
    '#default_value' => variable_get('addressfield_lookup_addressfield_hide_extra_fields', TRUE),
  );

  // Address details cache length.
  $form['addressfield_lookup_cache_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache length'),
    '#default_value' => variable_get('addressfield_lookup_cache_length', ADDRESSFIELD_LOOKUP_CACHE_LENGTH),
    '#description' => t('Length (in seconds) to keep address details in the cache.'),
  );

  return system_settings_form($form);
}
