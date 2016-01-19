<?php

/**
 * @file
 * Contains the administrative page and form callbacks for the pick-pack module.
 */


/**
 * Builds the form for adding and editing pick-pack services.
 */
function commerce_pickpackpoints_shipping_service_form($form, &$form_state, $shipping_service) {
  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_pickpackpoints_shipping') . '/theme/pickpack_shipping.css';

  // Store the initial shipping service in the form state.
  $form_state['shipping_service'] = $shipping_service;

  $form['pickpack'] = array(
    '#tree' => TRUE,
  );

  $form['pickpack']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $shipping_service['title'],
    '#description' => t('The administrative title of this pick-pack shiping. It is recommended that this title begin with a capital letter and contain only letters, numbers, and spaces.'),
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 255,
    '#field_suffix' => ' <small id="edit-pickpack-title-suffix">' . t('Machine name: @name', array('@name' => $shipping_service['name'])) . '</small>',
  );

  if (empty($shipping_service['name'])) {
    $form['pickpack']['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $shipping_service['name'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_shipping_service_load',
        'source' => array('pickpack', 'title'),
      ),
      '#description' => t('The machine-name of this pick-pack. This name must contain only lowercase letters, numbers, and underscores. It must be unique.'),
    );
  }
  else {
    $form['pickpack']['name'] = array(
      '#type' => 'value',
      '#value' => $shipping_service['name'],
    );
  }

  $form['pickpack']['display_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Display title'),
    '#default_value' => $shipping_service['display_title'],
    '#description' => t('The front end display title of this pick-pack shown to customers. Leave blank to default to the <em>Title</em> from above.'),
    '#size' => 32,
  );

  $form['pickpack']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this pick-pack if necessary. The text will be displayed in the pick-pack services overview table.'),
    '#default_value' => $shipping_service['description'],
    '#rows' => 3,
  );

  $form['pickpack']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Base rate'),
    '#default_value' => commerce_currency_amount_to_decimal($shipping_service['base_rate']['amount'], $shipping_service['base_rate']['currency_code']),
    '#required' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="commerce-pickpack-base-rate">',
  );

  // Build a currency options list from all enabled currencies.
  $options = commerce_currency_get_code(TRUE);

  // If the current currency value is not available, add it now with a
  // message in the help text explaining it.
  if (empty($options[$shipping_service['base_rate']['currency_code']])) {
    $options[$shipping_service['base_rate']['currency_code']] = check_plain($shipping_service['base_rate']['currency_code']);

    $description = t('The currency set for this rate is not currently enabled. If you change it now, you will not be able to set it back.');
  }
  else {
    $description = '';
  }

  // If only one currency option is available, don't use a select list.
  if (count($options) == 1) {
    $currency_code = key($options);

    $form['pickpack']['amount']['#field_suffix'] = check_plain($currency_code);
    $form['pickpack']['amount']['#suffix'] = '</div>';

    $form['pickpack']['currency_code'] = array(
      '#type' => 'value',
      '#default_value' => $currency_code,
    );
  }
  else {
    $form['pickpack']['currency_code'] = array(
      '#type' => 'select',
      '#description' => $description,
      '#options' => $options,
      '#default_value' => $shipping_service['base_rate']['currency_code'],
      '#suffix' => '</div>',
    );
  }

  // Add support for base rates including tax.
  if (module_exists('commerce_tax')) {
    // Build an array of tax types that are display inclusive.
    $inclusive_types = array();

    foreach (commerce_tax_types() as $name => $tax_type) {
      if ($tax_type['display_inclusive']) {
        $inclusive_types[$name] = $tax_type['title'];
      }
    }

    // Build an options array of tax rates of these types.
    $options = array();

    foreach (commerce_tax_rates() as $name => $tax_rate) {
      if (in_array($tax_rate['type'], array_keys($inclusive_types))) {
        $options[$inclusive_types[$tax_rate['type']]][$name] = t('Including @title', array('@title' => $tax_rate['title']));
      }
    }

    if (!empty($options)) {
      // Find the default value for the tax included element.
      $default = '';

      if (!empty($shipping_service['data']['include_tax'])) {
        $default = $shipping_service['data']['include_tax'];
      }

      $form['pickpack']['currency_code']['#title'] = '&nbsp;';

      $form['pickpack']['include_tax'] = array(
        '#type' => 'select',
        '#title' => t('Include tax in this rate'),
        '#description' => t('Saving a rate tax inclusive will bypass later calculations for the specified tax.'),
        '#options' => count($options) == 1 ? reset($options) : $options,
        '#default_value' => $default,
        '#required' => FALSE,
        '#empty_value' => '',
      );
    }
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save pick-pack'),
  );

  if (!empty($form_state['shipping_service']['name'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete pick-pack'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/config/shipping/services/pickpack'),
      '#submit' => array('commerce_pickpackpoints_shipping_service_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/commerce/config/shipping/services/pickpack');
  }

  return $form;
}

/**
 * Validate handler: ensures a valid base rate was entered for the pick-pack.
 */
function commerce_pickpackpoints_shipping_service_form_validate($form, &$form_state) {
  // Ensure the rate amount is numeric.
  if (!is_numeric($form_state['values']['pickpack']['amount'])) {
    form_set_error('pickpack][amount', t('You must enter a numeric value for the base rate amount.'));
  }
  else {
    // Convert the decimal amount value entered
    // to an integer based amount value.
    form_set_value($form['pickpack']['amount'], commerce_currency_decimal_to_amount($form_state['values']['pickpack']['amount'], $form_state['values']['pickpack']['currency_code']), $form_state);
  }
}

/**
 * Submit handler: saves the new or updated pick-pack service.
 */
function commerce_pickpackpoints_shipping_service_form_submit($form, &$form_state) {
  $shipping_service = $form_state['shipping_service'];

  // Update the shipping service array with values from the form.
  foreach (array(
    'name',
    'title',
    'display_title',
    'description',
    'amount',
    'currency_code') as $key) {
    $shipping_service[$key] = $form_state['values']['pickpack'][$key];
  }

  // If a tax was specified for inclusion, add it to the data array.
  if (!empty($form_state['values']['pickpack']['include_tax'])) {
    $shipping_service['data']['include_tax'] = $form_state['values']['pickpack']['include_tax'];
  }
  elseif (!empty($shipping_service['data']['include_tax'])) {
    unset($shipping_service['data']['include_tax']);
  }

  // Save the shipping service.
  unset($shipping_service['base_rate']);
  $op = commerce_pickpackpoints_shipping_service_save($shipping_service);

  if (!$op) {
    drupal_set_message(t('The pick-pack service failed to save properly. Please review the form and try again.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message(t('pick-pack service saved.'));
    $form_state['redirect'] = 'admin/commerce/config/shipping/services/pickpack';
  }
}

/**
 * Submit handler: redirects to the pick-pack service delete confirmation form.
 *
 * @see commerce_pickpackpoints_shipping_service_form()
 */
function commerce_pickpackpoints_shipping_service_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/config/shipping/services/pickpack-' . strtr($form_state['shipping_service']['name'], '_', '-') . '/delete';
}

/**
 * Displays the edit form for an existing pick-pack service.
 *
 * @param string $name
 *   The machine-name of the pick-pack service to edit.
 */
function commerce_pickpackpoints_shipping_service_edit_page($name) {
  return drupal_get_form('commerce_pickpackpoints_shipping_service_form', commerce_shipping_service_load($name));
}

/**
 * Builds the form for deleting pick-pack services.
 */
function commerce_pickpackpoints_shipping_service_delete_form($form, &$form_state, $shipping_service) {
  $form_state['shipping_service'] = $shipping_service;

  $form = confirm_form($form,
    t('Are you sure you want to delete the <em>%title</em> pick-pack service?', array('%title' => $shipping_service['title'])),
    'admin/commerce/config/shipping/services/pickpack',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_pickpackpoints_shipping_service_delete_form().
 */
function commerce_pickpackpoints_shipping_service_delete_form_submit($form, &$form_state) {
  $shipping_service = $form_state['shipping_service'];

  commerce_pickpackpoints_shipping_service_delete($shipping_service['name']);

  drupal_set_message(t('The pick-pack service <em>%title</em> has been deleted.', array('%title' => $shipping_service['title'])));
  watchdog('commerce_pickpackpoints_shipping', 'Deleted pick-pack service <em>%title</em>.', array('%title' => $shipping_service['title']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/config/shipping/services/pickpack';
}

/**
 * Displays the delete confirmation form for an existing pick-pack service.
 *
 * @param string $name
 *   The machine-name of the pick-pack service to delete.
 */
function commerce_pickpackpoints_shipping_service_delete_page($name) {
  return drupal_get_form('commerce_pickpackpoints_shipping_service_delete_form', commerce_shipping_service_load($name));
}
