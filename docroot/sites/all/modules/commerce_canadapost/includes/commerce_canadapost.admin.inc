<?php

/**
 * @file
 * Settings forms the for Commerce Canada Post module.
 */


module_load_include('inc', 'commerce_canadapost', 'includes/commerce_canadapost.services');

/**
 * Form callback: builds the settings form.
 *
 * Used to collect API credentials and enable supported shipping services.
 */
function commerce_canadapost_settings_form($form, &$form_state) {
  $api_key = variable_get('commerce_canadapost_api_key', '');

  $form['sellonline'] = array(
    '#type' => 'fieldset',
    '#title' => t('Canada Post web services Settings'),
    '#collapsible' => TRUE,
  );

  $form['sellonline']['commerce_canadapost_customer_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer Number'),
    '#description' => t('Your Canada Post customer number.'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_canadapost_customer_number', ''),
  );

  $form['sellonline']['commerce_canadapost_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('Enter your <strong>Production</strong> API Key Number. See <a href="@url">this page</a> for instructions on how to get one (follow steps 1 to 3).', array(
      '@url' => 'https://www.canadapost.ca/cpo/mc/business/productsservices/developers/services/gettingstarted.jsf',
    )),
    '#required' => TRUE,
    '#default_value' => $api_key,
  );

  $form['sellonline']['commerce_canadapost_origin'] = array(
    '#type' => 'textfield',
    '#title' => t('Origin Postal Code'),
    '#description' => t('The postal code to ship packages from.'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_canadapost_origin', ''),
    '#size' => 7,
    '#maxlength' => 7,
  );

  $date_format = variable_get('commerce_canadapost_delivery_date_format', '');
  if (!in_array($date_format, array_keys(system_get_date_types()))) {
    $date_format = '';
  }
  $form['sellonline']['commerce_canadapost_delivery_date_format'] = array(
    '#type' => 'radios',
    '#title' => t('Delivery Date Estimate'),
    '#description' => t('Select a display format for the estimated delivery date returned by Canada Post. See !url to modify available date formats.', array(
      '!url' => l(t('this page'), 'admin/config/regional/date-time'),
    )),
    '#options' => commerce_canadapost_option_list_date_formats(),
    '#default_value' => $date_format,
  );

  $form['sellonline']['commerce_canadapost_turnaround'] = array(
    '#type' => 'textfield',
    '#states' => array(
      'invisible' => array(
        ':input[name="commerce_canadapost_delivery_date_format"]' => array('value' => FALSE),
      ),
    ),
    '#title' => t('Turnaround Time'),
    '#description' => t('Amount of time to add to the delivery date estimate.'),
    '#default_value' => variable_get('commerce_canadapost_turnaround', ''),
    '#field_suffix' => t('hours'),
    '#size' => 4,
    '#maxlength' => 4,
  );

  $form['sellonline']['commerce_canadapost_package_count'] = array(
    '#type' => 'select',
    '#title' => t('Package Count'),
    '#description' => t('During checkout, display the number of packages required to ship the order.'),
    '#options' => array(
      'hide'     => t('Do not display'),
      'multiple' => t('Display if more than one'),
      'show'     => t('Always display'),
    ),
    '#default_value' => variable_get('commerce_canadapost_package_count', 'hide'),
  );

  $form['sellonline']['commerce_canadapost_cache_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Cached rates timeout'),
    '#description' => t('Enter the number of seconds after which cached shipping rates should no longer be considered valid.') . '<br />' . t('Set to 0 during testing to avoid caching failed rate request responses.'),
    '#default_value' => variable_get('commerce_canadapost_cache_timeout', 60),
    '#field_suffix' => t('seconds'),
    '#size' => 4,
    '#maxlength' => 4,
  );

  $form['sellonline']['commerce_canadapost_request_method'] = array(
    '#type' => 'radios',
    '#title' => t('Request method'),
    '#options' => array(
      'drupal' => t('Use drupal_http_request()'),
      'curl' => t('Use cURL'),
    ),
    '#default_value' => variable_get('commerce_canadapost_request_method', 'drupal'),
  );

  $form['sellonline']['commerce_canadapost_log'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => variable_get('commerce_canadapost_log', array()),
  );

  $form['markup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Markup / Handling Fees'),
    '#collapsible' => TRUE,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'commerce_canadapost') . '/theme/commerce_canadapost.admin.css',
      ),
      'js' => array(
        drupal_get_path('module', 'commerce_canadapost') . '/theme/commerce_canadapost.admin.js',
      ),
    ),
  );

  $rate_markup = variable_get('commerce_canadapost_rate_markup', array(
    'type' => 'none',
    'amount' => 0,
  ));

  // These two fields get combined together with #prefix, #suffix, and css.
  $form['markup']['commerce_canadapost_rate_markup_type'] = array(
    '#type' => 'select',
    '#title' => t('Rate Markup'),
    '#prefix' => '<div class="rate-markup-field">',
    '#options' => array(
      'none'        => t('None'),
      'flat'        => t('Flat rate'),
      'package'     => t('Flat rate per package'),
      'percentage'  => t('Percentage of shipping rate'),
    ),
    '#default_value' => $rate_markup['type'],
  );
  $form['markup']['commerce_canadapost_rate_markup'] = array(
    '#type' => 'textfield',
    '#states' => array(
      'invisible' => array(
        ':input[name="commerce_canadapost_rate_markup_type"]' => array('value' => 'none'),
      ),
    ),
    '#suffix' => '</div>',
    '#field_prefix' => '$',
    '#field_suffix' => '%',
    '#size' => 6,
    '#maxlength' => 6,
    '#default_value' => $rate_markup['amount'],
  );

  $weight_markup = variable_get('commerce_canadapost_weight_markup', array(
    'weight' => 0,
    'unit' => 'g',
  ));

  // These two fields get combined together with #prefix, #suffix, and css.
  $form['markup']['commerce_canadapost_weight_markup'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight Markup'),
    '#prefix' => '<div class="weight-markup-field">',
    '#size' => 6,
    '#maxlength' => 6,
    '#default_value' => $weight_markup['weight'],
  );
  $form['markup']['commerce_canadapost_weight_markup_unit'] = array(
    '#type' => 'select',
    '#suffix' => '</div>',
    '#description' => t('An amount of weight to add to each package.'),
    // Get an options list for weight units.
    // Reverse the array so that metric comes first (smallest to largest).
    '#options' => array_reverse(physical_weight_unit_options()),
    '#default_value' => $weight_markup['unit'],
  );

  $form['packaging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Packaging methods'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Build option list for packaging methods.
  module_load_include('inc', 'commerce_canadapost', 'includes/commerce_canadapost.package');
  $packaging_options = array();
  foreach (commerce_canadapost_list_packaging_methods() as $shortname => $details) {
    $packaging_options[$shortname] = format_string('<strong>!name</strong><br />!desc', array(
      '!name' => $details['name'],
      '!desc' => $details['description'],
    ));
  }

  $form['packaging']['commerce_canadapost_packaging_method'] = array(
    '#type' => 'radios',
    '#options' => $packaging_options,
    '#default_value' => variable_get('commerce_canadapost_packaging_method', 'weight'),
  );

  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled shipping services'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  // Only get service option list if API key is set.
  if ($api_key) {
    // Build options array.
    $shipping_services = commerce_canadapost_option_list_shipping_services();

    if (!empty($shipping_services)) {
      $enabled_services = commerce_canadapost_load_enabled_services();

      foreach ($shipping_services as $optgroup => $optgroup_options) {
        $form['services'][strtr($optgroup, ' ', '-')] = array(
          '#type' => 'checkboxes',
          '#title' => $optgroup,
          '#options' => $optgroup_options,
          '#default_value' => array_keys($enabled_services),
        );
      }

      // Collapse fieldset if options have already been selected.
      if (empty($enabled_services)) {
        $form['services']['#collapsed'] = FALSE;
      }
    }
  }

  // Remove form element if there are no services listed.
  if (!isset($shipping_services) || empty($shipping_services)) {
    unset($form['services']);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#suffix' => l(t('Cancel'), 'admin/commerce/config/shipping/methods'),
  );

  return $form;
}

/**
 * Validate handler: validates turnaround time and origin postal code.
 */
function commerce_canadapost_settings_form_validate($form, &$form_state) {
  // Check and format customer number.
  $customer_number = $form_state['values']['commerce_canadapost_customer_number'];
  if (!is_numeric($customer_number)) {
    form_set_error('commerce_canadapost_customer_number', t('That is not a valid Canada Post customer number.'));
  }
  else {
    // Remove leading zeroes.
    form_set_value($form['sellonline']['commerce_canadapost_customer_number'], ltrim($customer_number, '0'), $form_state);
  }

  // Format API key (lowercase alphanumeric with colon in middle).
  $api_key = preg_replace('/[^a-z0-9:]+/', '', strtolower($form_state['values']['commerce_canadapost_api_key']));
  form_set_value($form['sellonline']['commerce_canadapost_api_key'], $api_key, $form_state);

  // Test api key, skip if already saved to db.
  if ($api_key != variable_get('commerce_canadapost_api_key', '')) {
    $response = commerce_canadapost_api_request('https://soa-gw.canadapost.ca/rs/ship/service', NULL, t('Testing the API key.'), $api_key);
    if (!$response) {
      form_set_error('commerce_canadapost_api_key', t('The API key is invalid.'));
    };
  }

  // Check markup = valid percentage or currency amount.
  $rate_markup = $form_state['values']['commerce_canadapost_rate_markup'];
  if ($rate_markup != '') {
    switch ($form_state['values']['commerce_canadapost_rate_markup_type']) {
      case 'none':
        form_set_value($form['markup']['commerce_canadapost_rate_markup'], 0, $form_state);
        break;

      case 'percentage':
        // Check for and remove percent sign.
        $rate_markup = str_replace('%', '', $form_state['values']['commerce_canadapost_rate_markup']);
        if (!is_numeric($rate_markup) || ($rate_markup < 0)) {
          form_set_error('commerce_canadapost_rate_markup', t('You must enter a positive numeric value for rate markup / handling percentage.'));
        }
        form_set_value($form['markup']['commerce_canadapost_rate_markup'], $rate_markup, $form_state);
        break;

      default:
        if (!is_numeric($form_state['values']['commerce_canadapost_rate_markup']) || ($rate_markup < 0)) {
          form_set_error('commerce_canadapost_rate_markup', t('You must enter a positive numeric value for rate markup / handling amount.'));
        }
    }
  }

  // Weight markup = positive float value.
  if (!is_numeric($form_state['values']['commerce_canadapost_weight_markup']) || $form_state['values']['commerce_canadapost_weight_markup'] < 0) {
    form_set_error('commerce_canadapost_weight_markup', t('You must enter a positive numeric value for weight markup.'));
  }

  // Turnaround = positive integer.
  if ($form_state['values']['commerce_canadapost_turnaround'] && !ctype_digit($form_state['values']['commerce_canadapost_turnaround'])) {
    form_set_error('commerce_canadapost_turnaround', t('If you enter a value for <em>Turnaround Time</em> it must be a whole number.'));
  }

  // Valid Canadian postal code.
  $origin = commerce_canadapost_format_postal_code($form_state['values']['commerce_canadapost_origin'], 'CA');
  // Canadian postal codes: 6 characters, alternating letter-number,
  // cannot contain DFIOQU, first character cannot be W or Z.
  if ($origin && !preg_match('/^[ABCEGHJKLMNPRSTVXY][0-9][ABCEGHJKLMNPRSTVWXYZ][ ]?[0-9][ABCEGHJKLMNPRSTVWXYZ][0-9]$/', $origin)) {
    form_set_error('commerce_canadapost_origin', t('The value you entered for <em>Origin Postal Code</em> is not a valid Canadian postal code.'));
  }
  else {
    // Update value with formatted version.
    form_set_value($form['sellonline']['commerce_canadapost_origin'], $origin, $form_state);
  }

  // Check for cURL extension.
  if (($form_state['values']['commerce_canadapost_request_method'] == 'curl') && !extension_loaded('curl')) {
    drupal_set_message(t('<a href="@url">cURL</a> extension not found, Canada Post web services API request method set to drupal_http_request().', array(
      '@url' => 'http://www.php.net/manual/en/curl.setup.php',
    )), 'error');
    form_set_value($form['sellonline']['commerce_canadapost_request_method'], 'drupal', $form_state);
  }
}

/**
 * Submit handler: saves settings and enables/deletes selected services.
 */
function commerce_canadapost_settings_form_submit($form, &$form_state) {
  // Save settings.
  variable_set('commerce_canadapost_customer_number', $form_state['values']['commerce_canadapost_customer_number']);
  variable_set('commerce_canadapost_api_key', $form_state['values']['commerce_canadapost_api_key']);
  variable_set('commerce_canadapost_origin', $form_state['values']['commerce_canadapost_origin']);
  variable_set('commerce_canadapost_turnaround', $form_state['values']['commerce_canadapost_turnaround']);
  variable_set('commerce_canadapost_delivery_date_format', $form_state['values']['commerce_canadapost_delivery_date_format']);
  variable_set('commerce_canadapost_package_count', $form_state['values']['commerce_canadapost_package_count']);
  variable_set('commerce_canadapost_cache_timeout', $form_state['values']['commerce_canadapost_cache_timeout']);
  variable_set('commerce_canadapost_log', $form_state['values']['commerce_canadapost_log']);
  variable_set('commerce_canadapost_rate_markup', array(
    'type'  => $form_state['values']['commerce_canadapost_rate_markup_type'],
    'amount' => (float) $form_state['values']['commerce_canadapost_rate_markup'],
  ));
  variable_set('commerce_canadapost_weight_markup', array(
    'weight' => (float) $form_state['values']['commerce_canadapost_weight_markup'],
    'unit'  => $form_state['values']['commerce_canadapost_weight_markup_unit'],
  ));
  variable_set('commerce_canadapost_packaging_method', $form_state['values']['commerce_canadapost_packaging_method']);
  variable_set('commerce_canadapost_request_method', $form_state['values']['commerce_canadapost_request_method']);

  if (isset($form_state['values']['services'])) {
    // Load the enabled services.
    $enabled_services = commerce_canadapost_load_enabled_services();
    $all_services = commerce_canadapost_option_list_shipping_services();

    // Loop over the shipping services...
    $services_updated = FALSE;
    foreach ($form_state['values']['services'] as $optgroup => $options) {
      foreach ($options as $key => $value) {
        // If a service was marked to be enabled and hasn't been saved yet, save
        // it to the database now.
        if ($key === $value && !isset($enabled_services[$key])) {
          $service = array(
            'name' => $key,
            'title' => $all_services[strtr($optgroup, '-', ' ')][$key],
            'status' => 'enabled',
          );

          commerce_canadapost_service_save($service);
          $services_updated = TRUE;
        }
        // If a service that is enabled was unchecked, delete it from the
        // database now.
        elseif (isset($enabled_services[$key]) && ($value == '0')) {
          commerce_canadapost_service_delete($key);
          $services_updated = TRUE;
        }
      }
    }

    // Clear service cache, rebuild default rules & menu items.
    if ($services_updated) {
      commerce_canadapost_rebuild_enabled_services();
    }
  }

  drupal_set_message(t('Canada Post Shipping configuration saved.'));

  // Stay on the same page if services still need to be selected.
  $enabled_services = commerce_canadapost_load_enabled_services();
  if (empty($enabled_services)) {
    drupal_set_message(t('<a href="!anchor">Click here</a> to enable some Canada Post shipping services.', array('!anchor' => '#edit-services')));
  }
  // Redirect to shipping method list.
  else {
    $form_state['redirect'] = 'admin/commerce/config/shipping/methods';
  }
}

/**
 * Builds the form for deleting services.
 */
function commerce_canadapost_service_delete_form($form, &$form_state, $shipping_service) {
  $form_state['shipping_service'] = $shipping_service;

  $form = confirm_form($form,
    t('Remove Canada Post service'),
    'admin/commerce/config/shipping/services/canadapost',
    t('Are you sure you want to remove the <strong>@title</strong> service?', array('@title' => $shipping_service['title'])),
    t('Remove'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_canadapost_service_delete_form().
 */
function commerce_canadapost_service_delete_form_submit($form, &$form_state) {
  commerce_canadapost_service_delete($form_state['shipping_service']['name']);

  // Clear service cache, rebuild default rules & menu items.
  commerce_canadapost_rebuild_enabled_services();

  drupal_set_message(t('The shipping service: <strong>@title</strong> has been removed.', array('@title' => $form_state['shipping_service']['title'])));

  $form_state['redirect'] = 'admin/commerce/config/shipping/services/canadapost';
}

/**
 * Displays the delete confirmation form for an existing service.
 *
 * @param string $name
 *   The machine-name of the service to delete.
 */
function commerce_canadapost_service_delete_page($name) {
  return drupal_get_form('commerce_canadapost_service_delete_form', commerce_shipping_service_load($name));
}

/**
 * Returns an option list of available Canada Post shipping services.
 *
 * @return array
 *   An associative array keyed by destination and then machine name.
 */
function commerce_canadapost_option_list_shipping_services() {
  $services = array();

  foreach (commerce_canadapost_services_list() as $service_code => $service) {
    switch (substr($service_code, 0, 3)) {
      case 'DOM':
        $destination = t('Domestic');
        break;

      case 'USA':
        $destination = t('USA');
        break;

      case 'INT':
        $destination = t('International');
        break;
    }

    $services[$destination][commerce_canadapost_generate_service_machine_name($service_code)] = $service['name'];
  }

  return $services;
}

/**
 * Returns an option list of date format types.
 *
 * @return array
 *   An associative array keyed by machine name.
 */
function commerce_canadapost_option_list_date_formats() {
  $date_options_list = array('' => t('Do not display'));

  foreach (system_get_date_types() as $type) {
    $date_options_list[$type['type']] = t('<strong>!title:</strong> !format', array(
      '!title' => $type['title'],
      '!format' => format_date(REQUEST_TIME, $type['type']),
    ));
  }

  return $date_options_list;
}
