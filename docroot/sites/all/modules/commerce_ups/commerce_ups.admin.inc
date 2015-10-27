<?php

/**
 * @file
 * Handles admin settings page for Commerce UPS module.
 */

/* hook_settings_form() */
function commerce_ups_settings_form($form, &$form_state) {
  $ups_link = l(t('UPS.com'), 'https://www.ups.com/upsdeveloperkit', array('attributes' => array('target' => '_blank')));
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('UPS API credentials'),
    '#collapsible' => TRUE,
    '#description' => t('In order to obtain shipping rate estimates, you must have an account with UPS. You can apply for UPS API credentials at !ups', array('!ups' => $ups_link)),
  );

  $encrypted = variable_get('commerce_ups_encrypt', FALSE);
  $api_vars = commerce_ups_decrypt_vars(FALSE);

  $form['api']['commerce_ups_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Account ID'),
    '#default_value' => $api_vars['ups_accountid'],
    '#required' => TRUE,
  );
  $form['api']['commerce_ups_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#default_value' => $api_vars['ups_userid'],
    '#required' => TRUE,
  );
  $form['api']['commerce_ups_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => !variable_get('commerce_ups_password', FALSE),
    '#description' => t('Please leave blank if you do not want to update your password at this time.'),
  );
  $form['api']['commerce_ups_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Key'),
    '#default_value' => $api_vars['ups_accesskey'],
    '#required' => TRUE,
  );
  $form['api']['encryption'] = array(
    '#type' => 'item',
    '#title' => t('Encryption'),
    'status' => array(
      '#type' => 'item',
      '#title' => FALSE,
    ),
  );
  if (commerce_ups_encryption_available()) {
    $form['api']['encryption']['status']['#markup'] =
      'Encryption is available and configured properly.';
    $form['api']['encryption']['commerce_ups_encrypt'] = array(
      '#type' => 'checkbox',
      '#title' => t('Encrypt UPS credentials (HIGHLY RECOMMENDED)'),
      '#description' => t('Note: Enabling this setting automatically will encrypt your password even though you cannot see it. Disabling this checkbox requires that you re-enter a password.'),
      '#default_value' => $encrypted,
    );
  }
  else {
    $aes_link = l(t('AES'), 'http://drupal.org/project/aes', array('attributes' => array('target' => '_blank')));
    $form['api']['encryption']['status']['#markup'] = '<span class="error">' . t('!aes is not installed - your login credentials will not be encrypted.', array('!aes' => $aes_link)) . '</span>';
  }

  $form['origin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ship From Address'),
    '#collapsible' => TRUE,
  );
  $form['origin']['commerce_ups_company_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Name'),
    '#default_value' => variable_get('commerce_ups_company_name')
  );
  $form['origin']['commerce_ups_address_line_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#default_value' => variable_get('commerce_ups_address_line_1')
  );
  $form['origin']['commerce_ups_address_line_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address (Additional)'),
    '#default_value' => variable_get('commerce_ups_address_line_2')
  );
  $form['origin']['commerce_ups_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => variable_get('commerce_ups_city')
  );
  $form['origin']['commerce_ups_state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#default_value' => variable_get('commerce_ups_state'),
    '#options' => array(
      '' => t('Please Select'),
      'AL' => t('Alabama'),
      'AK' => t('Alaska'),
      'AZ' => t('Arizona'),
      'AR' => t('Arkansas'),
      'CA' => t('California'),
      'CO' => t('Colorado'),
      'CT' => t('Connecticut'),
      'DE' => t('Delaware'),
      'DC' => t('District of Columbia'),
      'FL' => t('Florida'),
      'GA' => t('Georgia'),
      'HI' => t('Hawaii'),
      'ID' => t('Idaho'),
      'IL' => t('Illinois'),
      'IN' => t('Indiana'),
      'IA' => t('Iowa'),
      'KS' => t('Kansas'),
      'KY' => t('Kentucky'),
      'LA' => t('Louisiana'),
      'ME' => t('Maine'),
      'MD' => t('Maryland'),
      'MA' => t('Massachusetts'),
      'MI' => t('Michigan'),
      'MN' => t('Minnesota'),
      'MS' => t('Mississippi'),
      'MO' => t('Missouri'),
      'MY' => t('Montana'),
      'NE' => t('Nebraska'),
      'NV' => t('Nevada'),
      'NH' => t('New Hampshire'),
      'NJ' => t('New Jersey'),
      'NM' => t('New Mexico'),
      'NY' => t('New York'),
      'NC' => t('North Carolina'),
      'ND' => t('North Dakota'),
      'OH' => t('Ohio'),
      'OK' => t('Oklahoma'),
      'OR' => t('Oregon'),
      'PA' => t('Pennsylvania'),
      'RI' => t('Rhode Island'),
      'SC' => t('South Carolina'),
      'SD' => t('South Dakota'),
      'TN' => t('Tennessee'),
      'TX' => t('Texas'),
      'UT' => t('Utah'),
      'VT' => t('Vermont'),
      'VA' => t('Virginia'),
      'WA' => t('Washington'),
      'WV' => t('West Virginia'),
      'WI' => t('Wisconsin'),
      'WY' => t('Wyoming'),
    )
  );
  $form['origin']['commerce_ups_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal Code'),
    '#size' => 5,
    '#default_value' => variable_get('commerce_ups_postal_code')
  );
  $form['origin']['commerce_ups_country_code'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => variable_get('commerce_ups_country_code'),
    '#options' => array(
      '' => t('Please Select'),
      'US' => t('United States of America'),
    )
  );
  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable UPS Shipping Services'),
    '#collapsible' => TRUE,
  );

  foreach (_commerce_ups_service_list() as $key => $service) {
    $array_options[$key] = $service['title'];
  }
  $form['services']['commerce_ups_services'] = array(
    '#type' => 'checkboxes',
    '#options' => $array_options,
    '#default_value' => variable_get('commerce_ups_services', array())
  );
  $form['packaging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable UPS Packaging Types'),
    '#collapsible' => FALSE,
  );
  $form['packaging']['commerce_ups_packaging_explanation'] = array(
    '#type' => 'markup',
    '#markup' => t('Only "Customer supplied packaging" is currently available. Please enter the package size below.'),
  );
  $form['packaging']['commerce_ups_packaging'] = array(
    '#type' => 'hidden',
    '#value' => '02',
  );

  // Fields for default package size (inches)
  $form['default_package_size'] = array(
    '#type' => 'fieldset',
    '#title' => t('Package size (inches)'),
    '#collapsible' => FALSE,
    '#description' => 'UPS requires a package size when determining estimates.',
  );
  $form['default_package_size']['commerce_ups_default_package_size_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length'),
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_ups_default_package_size_length'),
  );
  $form['default_package_size']['commerce_ups_default_package_size_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_ups_default_package_size_width'),
  );
  $form['default_package_size']['commerce_ups_default_package_size_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_ups_default_package_size_height'),
  );
  $form['default_package_size']['details'] = array(
    '#markup' => 'The package size is used to determine the number of packages necessary to
      create a UPS shipping cost estimate. <strong>If products do not have physical dimensions and
      weights associated with them, the estimates will not be accurate.</strong> The logic implemented works as:
      <ul>
      <li>Assume each order requires at least one package.</li>
      <li>Use the combined volume of all products in an order to determine the number of packages.</li>
      </ul>
      This is a simple calculation that can get close to actual shipping costs. More
      complex logic involving multiple package sizes, weights, and void space can be implemented via
      custom modules.
      <br/><br/>
      UPS has a ' . l('maximum package size', 'http://www.ups.com/content/us/en/resources/prepare/oversize.html', array('attributes' => array('target' => '_blank'))) . '.',
  );
  
  $form['advanced_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced_options']['commerce_ups_pick_up_schedule'] = array(
    '#type' => 'select',
    '#title' => t('Pick-up Schedule'),
    '#options' => _commerce_ups_pickup_types(),
    '#default_value' => variable_get('commerce_ups_pick_up_schedule')
  );
  $form['advanced_options']['commerce_ups_shipto_residential'] = array(
    '#type' => 'checkbox',
    '#title' => t('Calculate ALL ship to addresses as residential.'),
    '#default_value' => variable_get('commerce_ups_shipto_residential', 0),
  );
  $form['advanced_options']['commerce_ups_show_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show UPS Logo on Shipping Page'),
    '#default_value' => variable_get('commerce_ups_show_logo', 0),
  );
  $form['advanced_options']['commerce_ups_log'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => variable_get('commerce_ups_log', array()),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/*
 * Implements hook_form_validate().
 */
function commerce_ups_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  // Ensure the default package size is less than the maximum (http://www.ups.com/content/us/en/resources/prepare/oversize.html)
  //  Length (longest side) <= 108 inches
  //  Girth = 2*width + 2*height
  //  Length + Girth <= 165 inches
  $dimensions = array($values['commerce_ups_default_package_size_length'],
                      $values['commerce_ups_default_package_size_width'],
                      $values['commerce_ups_default_package_size_height']);
  sort($dimensions);
  list($height, $width, $length) = $dimensions;
  $girth = 2*$width + 2*$height;
  if ($length > 108) {
    form_set_error('commerce_ups_default_package_size_length', t('The greatest dimension of the package size must be 108 inches or less.'));
  }
  if ($length + $girth > 165) {
    form_set_error('commerce_ups_default_package_size_length', t('The girth (2*width + 2*height) of the package size must be 165 inches or less.'));
  }

  $encrypted = variable_get('commerce_ups_encrypt', FALSE) && function_exists('aes_decrypt');

  // If the Password field is empty, then they're not trying to update it and we should ignore it.
  if (empty($values['commerce_ups_password'])) {
    unset($form_state['values']['commerce_ups_password']);
    if ($encrypted && empty($form_state['input']['commerce_ups_password'])) {
      form_set_error('commerce_ups_password', t('You must enter in a password to turn off encryption.'));
    }
    return;
  }
  
  /* If we are setting or resetting values, test the connection.
  $ups_connection = ups_api_function_here();
  if (is_good_connection($ups_connection)) {
    drupal_set_message(t('Connection established. UPS credentials updated.'));
  }
  else {
    drupal_set_message(
      t('Resetting UPS credentials failed.'), 'error');
    form_set_error('commerce_ups_account_id', t('Unable to connect to UPS. Please check your credentials.'));
    form_set_error('commerce_ups_user_id');
    form_set_error('commerce_ups_access_key');
    form_set_error('commerce_ups_password');
  }
  */
}

/*
 * Implements hook_form_submit().
 */
function commerce_ups_settings_form_submit($form, &$form_state) {
  $encrypted = variable_get('commerce_ups_encrypt', FALSE) && function_exists('aes_decrypt');
  // Encrypt the UPS API credentials if available
  if (isset($form_state['input']['commerce_ups_encrypt']) && $form_state['input']['commerce_ups_encrypt']) {
    $form_state['input']['commerce_ups_user_id'] = commerce_ups_encrypt($form_state['input']['commerce_ups_user_id']);
    $form_state['input']['commerce_ups_account_id'] = commerce_ups_encrypt($form_state['input']['commerce_ups_account_id']);
    $form_state['input']['commerce_ups_access_key'] = commerce_ups_encrypt($form_state['input']['commerce_ups_access_key']);
    if (!empty($form_state['input']['commerce_ups_password'])) {
      $form_state['input']['commerce_ups_password'] = commerce_ups_encrypt($form_state['input']['commerce_ups_password']);
    }
    elseif ($encrypted == FALSE) {
      // This is an odd case, if encryption is turned on but the password is already in the db
      // Then we need to pull it from the db, not from the field
      $form_state['input']['commerce_ups_password'] = commerce_ups_encrypt(variable_get('commerce_ups_password', ''));
    }
  }

  if (empty($form_state['input']['commerce_ups_password'])) {
    unset($form_state['input']['commerce_ups_password']);
  }

  if (!isset($form_state['input']['commerce_ups_encrypt'])) {
    $form_state['input']['commerce_ups_encrypt'] = FALSE;
  }

  $fields = array(
    'commerce_ups_account_id',
    'commerce_ups_user_id',
    'commerce_ups_password',
    'commerce_ups_access_key',
    'commerce_ups_encrypt',
    'commerce_ups_services',
    'commerce_ups_company_name',
    'commerce_ups_address_line_1',
    'commerce_ups_address_line_2',
    'commerce_ups_city',
    'commerce_ups_state',
    'commerce_ups_postal_code',
    'commerce_ups_country_code',
    'commerce_ups_packaging',
    'commerce_ups_pick_up_schedule',
    'commerce_ups_show_logo',
    'commerce_ups_default_package_size_length',
    'commerce_ups_default_package_size_width',
    'commerce_ups_default_package_size_height',
    'commerce_ups_shipto_residential',
    'commerce_ups_log',
  );

  $services = variable_get('commerce_ups_services', NULL);
  foreach ($fields as $key) {
    if (array_key_exists($key, $form_state['input'])) {
      $value = $form_state['input'][$key];
      variable_set($key, $value);
    }
  }
  
  // If the selected services have changed then rebuild caches.
  if ($services !== $form_state['values']['commerce_ups_services']) {
    commerce_shipping_services_reset();
    entity_defaults_rebuild();
    rules_clear_cache(TRUE);
    menu_rebuild();
  }

  drupal_set_message(t('The UPS configuration options have been saved.'));
}
