<?php

/**
 * @file
 * Handles admin settings page for Commerce Periship module.
 */

/* hook_settings_form() */
function commerce_periship_settings_form($form, &$form_state) {
  //$periship_link = l(t('Periship.com'), 'http://periship.com/contactperiship.php', array('attributes' => array('target' => '_blank')));
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Periship'),
    '#collapsible' => TRUE,
    '#description' => t('In order to obtain shipping rate estimates, you must have an account with Periship. If you do not have an account with Periship or need your API Keys, @periship', array('@periship' => l(t('Periship.com'), 'http://periship.com/contactperiship.php', array('attributes' => array('target' => '_blank'))))),
  );

  $encrypted = variable_get('commerce_periship_encrypt', FALSE);
  $api_vars = commerce_periship_decrypt_vars(FALSE);

  $form['api']['commerce_periship_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Account ID'),
    '#default_value' => $api_vars['periship_accountid'],
    '#required' => TRUE,
  );
  $form['api']['commerce_periship_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => !variable_get('commerce_periship_password', FALSE),
    '#description' => t('Please leave blank if you do not want to update your password at this time.'),
  );
  $form['api']['encryption'] = array(
    '#type' => 'item',
    '#title' => t('Encryption'),
    'status' => array(
      '#type' => 'item',
      '#title' => FALSE,
    ),
  );
  if (commerce_periship_encryption_available(array('display_all' => TRUE))) {
    $form['api']['encryption']['status']['#markup'] =
      'Encryption is available and configured properly.';
    $form['api']['encryption']['commerce_periship_encrypt'] = array(
      '#type' => 'checkbox',
      '#title' => t('Encrypt Periship credentials (HIGHLY RECOMMENDED)'),
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
  $form['origin']['commerce_periship_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal Code'),
    '#size' => 5,
    '#required' => TRUE,
    '#description' => t('Please enter the 5 digit ZIP code from which the shipments will originate.'),
    '#default_value' => variable_get('commerce_periship_postal_code')
  );

  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable Periship Shipping Services'),
    '#collapsible' => TRUE,
  );
  foreach (_commerce_periship_service_list() as $key => $service) {
    $array_options[$key] = $service['description'];
  }
  $form['services']['commerce_periship_services'] = array(
    '#type' => 'checkboxes',
    '#options' => $array_options,
    '#default_value' => variable_get('commerce_periship_services', array())
  );

  $form['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Store Settings'),
    '#collapsible' => TRUE,
  );
  $form['defaults']['commerce_periship_shipto_residential'] = array(
    '#type' => 'checkbox',
    '#title' => t('Calculate ALL ship to addresses as residential.'),
    '#default_value' => variable_get('commerce_periship_shipto_residential', 0),
  );
  $form['defaults']['commerce_periship_shipto_signature'] = array(
    '#type' => 'radios',
    '#options' => array('D' => 'Direct Signature Required', 'A' => 'Adult Signature Required', 'N' => 'No Signature Required'),
    '#title' => t('Package Signature Requirements:'),
    '#default_value' => variable_get('commerce_periship_shipto_signature', 'N'),
  );
  $form['defaults']['commerce_periship_shipto_saturday'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Saturday Delivery.'),
    '#default_value' => variable_get('commerce_periship_shipto_saturday', 0),
  );
  $form['defaults']['commerce_periship_quickest_day'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'Same Day',
      1 => 'Next Day',
      2 => '2 Days',
      3 => '3 Days',
      4 => '4 Days',
      5 => '5 Days',
    ),
    '#title' => t('Quickest Shipping Day:'),
    '#default_value' => variable_get('commerce_periship_quickest_day', 0),
  );
  $form['defaults']['commerce_periship_cutoff_time'] = array(
    '#type' => 'select',
    '#options' => array(
      10 => '10am',
      11 => '11am',
      12 => 'noon',
      13 => '1pm',
      14 => '2pm',
      15 => '3pm',
      16 => '4pm',
      17 => '5pm',
      18 => '6pm',
      19 => '7pm',
    ),
    '#title' => t('Cutoff Time for Quickest Day Shipping:'),
    '#description' => t('This time will be the same timezone as your site. Current Timezone: @zone', array('@zone' => date('e')) ),
    '#default_value' => variable_get('commerce_periship_cutoff_time', 15),
  );
  $form['defaults']['commerce_periship_default_dryice'] = array(
    '#type' => 'radios',
    '#options' => array('0' => 'No Dry Ice', '1' => 'All Packages contain Dry Ice'),
    '#title' => t('Default Dry Ice status:'),
    '#description' => t('For custom Dry Ice settings, choose a default, then add a field to your products for Dry Ice and use rules to change this value based on that value.'),
    '#default_value' => variable_get('commerce_periship_default_dryice', 0),
  );
  $form['commerce_periship_show_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show FedEx Logo on Shipping Page'),
    '#default_value' => variable_get('commerce_periship_show_logo', 0),
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
function commerce_periship_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $encrypted = variable_get('commerce_periship_encrypt', FALSE) && function_exists('aes_decrypt');

  // If the Password field is empty, then they're not trying to update it and we should ignore it.
  if (empty($values['commerce_periship_password'])) {
    unset($form_state['values']['commerce_periship_password']);
    if ($encrypted && empty($form_state['input']['commerce_periship_encrypt'])) {
      form_set_error('commerce_periship_password', t('You must enter a password to turn off encryption.'));
    }
    return;
  }
}

/*
 * Implements hook_form_submit().
 */
function commerce_periship_settings_form_submit($form, &$form_state) {
  $encrypted = variable_get('commerce_periship_encrypt', FALSE) && function_exists('aes_decrypt');
  // Encrypt the Periship API credentials if available
  if (isset($form_state['input']['commerce_periship_encrypt']) && $form_state['input']['commerce_periship_encrypt']) {
    $form_state['input']['commerce_periship_account_id'] = commerce_periship_encrypt($form_state['input']['commerce_periship_account_id']);
    if (!empty($form_state['input']['commerce_periship_password'])) {
      $form_state['input']['commerce_periship_password'] = commerce_periship_encrypt($form_state['input']['commerce_periship_password']);
    }
    elseif ($encrypted == FALSE) {
      // This is an odd case, if encryption is turned on but the password is already in the db
      // Then we need to pull it from the db, not from the field
      $form_state['input']['commerce_periship_password'] = commerce_periship_encrypt(variable_get('commerce_periship_password', ''));
    }
  }

  if (empty($form_state['input']['commerce_periship_password'])) {
    unset($form_state['input']['commerce_periship_password']);
  }

  if (!isset($form_state['input']['commerce_periship_encrypt'])) {
    $form_state['input']['commerce_periship_encrypt'] = FALSE;
  }

  // Check the Periship service variable before the form is saved to check if
  // this value has been changed in this form submit.
  $services = variable_get('commerce_periship_services', NULL);

  $fields = array(
    'commerce_periship_account_id',
    'commerce_periship_password',
    'commerce_periship_encrypt',
    'commerce_periship_postal_code',
    'commerce_periship_services',
    'commerce_periship_shipto_residential',
    'commerce_periship_shipto_signature',
    'commerce_periship_shipto_saturday',
    'commerce_periship_quickest_day',
    'commerce_periship_cutoff_time',
    'commerce_periship_default_dryice',
    'commerce_periship_show_logo',
  );

  foreach ($fields as $key) {
    if (array_key_exists($key, $form_state['input'])) {
      $value = $form_state['input'][$key];
      variable_set($key, $value);
    }
  }

  // If the selected shipping services have changed with this form submit then
  // clear the shipping services, rules, and menu caches.
  if ($services !== $form_state['values']['commerce_periship_services']) {
    commerce_shipping_services_reset();
    entity_defaults_rebuild();
    rules_clear_cache(TRUE);
    menu_rebuild();
  }

  drupal_set_message(t('The Periship configuration options have been saved.'));
}
