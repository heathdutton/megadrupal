<?php

/**
 * @file
 * Administrative page callbacks for the Multivendor Shipping module.
 */

/**
 * Displays all vendors in a table.
 */
function commerce_multivendor_shipping_overview() {

  $header = array(
    t('Title'),
    t('Operations'),
  );

  $vendors = commerce_multivendor_shipping_vendors();

  $rows = array();
  foreach ($vendors as $vendor) {
    $arg = strtr($vendor->name, '_', '-');
    $links = menu_contextual_links('commerce_multivendor_shipping', 'admin/commerce/config/multivendorshipping', array($arg));

    $rows[] = array(
      $vendor->label . ' <small> (' . t('Machine name: @name', array('@name' => $vendor->name)) . ')</small>',
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );

  }

  // If no items are defined...
  if (empty($rows)) {
    $empty_text = t('There are no shipping vendors defined.');

    $rows[] = array(
      array(
        'data' => $empty_text,
        'colspan' => 2,
      ),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Vendor edit form.
 */
function commerce_multivendor_shipping_vendor_form($form, &$form_state, $name) {

  // Store the initial vendor in the form state.
  if ($name == 'is_new') {
    $form_state['initial'] = commerce_multivendor_shipping_vendor_new();
  }
  else {
    $name = strtr($name, '-', '_');
    $form_state['initial'] = commerce_multivendor_shipping_vendor_load($name);
  }

  $form['vendor'] = array(
    '#tree' => TRUE,
  );

  $form['vendor']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $form_state['initial']['label'],
    '#description' => t('The administrative title of this vendor.'),
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 255,
    '#field_suffix' => ' <small>' . t('Machine name: @name', array('@name' => $form_state['initial']['name'])) . '</small>',
  );

  if (empty($form_state['initial']['name'])) {
    $form['vendor']['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $form_state['initial']['name'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_multivendor_shipping_exists',
        'source' => array('vendor', 'label'),
      ),
      '#description' => t('The machine-name of this shippinh vendor. This name must contain only lowercase letters, numbers, and underscores. It must be unique.'),
    );
  }
  else {
    $form['vendor']['name'] = array(
      '#type' => 'value',
      '#value' => $form_state['initial']['name'],
    );
  }

 $form['vendor']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $form_state['initial']['description'],
    '#description' => t('The description of this vendor.'),
  );

  // Only support default currency and no taxes for now.
  $currency = commerce_default_currency();

  $form['vendor']['base_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Base price'),
    '#default_value' => commerce_currency_amount_to_decimal($form_state['initial']['base_amount'], $currency),
    '#description' => t('The base price for this vendor.'),
    '#element_validate' => array('element_validate_number'),
    '#size' => 10,
    '#field_suffix' => $currency,
  );

  $form['vendor']['package_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Package price'),
    '#default_value' => commerce_currency_amount_to_decimal($form_state['initial']['package_amount'], $currency),
    '#description' => t('The package price for this vendor.'),
    '#element_validate' => array('element_validate_number'),
    '#size' => 10,
    '#field_suffix' => $currency,
  );

  $form['vendor']['package_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Package size'),
    '#default_value' => $form_state['initial']['package_size'],
    '#description' => t('The package size for this vendor. Size must be a positive integer.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 10,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save vendor'),
  );

  if (!empty($form_state['initial']['name'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete vendor'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/config/multivendorshipping'),
      '#submit' => array('commerce_multivendor_shipping_vendor_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/commerce/config/multivendorshipping');
  }

  return $form;

}

/**
 * Submit handler: saves the new or updated shipping vendor.
 */
function commerce_multivendor_shipping_vendor_form_submit($form, &$form_state) {

  $vendor = $form_state['initial'];

  $field_names = array(
    'name',
    'label',
    'description',
    'package_size',
    'base_amount',
    'package_amount',
  );

  // Update the vendor array with values from the form.
  foreach ($field_names as $key) {
    $value = $form_state['values']['vendor'][$key];
    switch ($key) {
      case 'package_size':
        $vendor[$key] = $value > 0 ? $value : 1;
        break;

      case 'base_amount':
      case 'package_amount':
        $vendor[$key] = $value * 100;
        break;

      default:
        $vendor[$key] = $value;
    }

  }

  $op = commerce_multivendor_shipping_vendor_save($vendor);

  if (!$op) {
    drupal_set_message(t('The vendor failed to save. Please review the form and try again.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message(t('Shipping vendor saved.'));
    $form_state['redirect'] = 'admin/commerce/config/multivendorshipping';
  }
}

/**
 * Submit handler: redirects to the delete confirmation form.
 *
 * @see commerce_multivendor_shipping_vendor_form()
 */
function commerce_multivendor_shipping_vendor_form_delete_submit($form, &$form_state) {
  $name = strtr($form_state['initial']['name'], '_', '-');
  $form_state['redirect'] = 'admin/commerce/config/multivendorshipping/' . $name . '/delete';
}

/**
 * Builds the form for deleting shipping vendor.
 */
function commerce_multivendor_shipping_delete_form($form, &$form_state, $name) {

  $name = strtr($name, '-', '_');
  $vendor = commerce_multivendor_shipping_vendor_load($name);
  $form_state['vendor'] = $vendor;

  $form = confirm_form($form,
    t('Are you sure you want to delete the <em>%label</em> shipping vendor?', array('%label' => $vendor['label'])),
    'admin/commerce/config/multivendorshipping',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_multivendor_shipping_delete_form().
 */
function commerce_multivendor_shipping_delete_form_submit($form, &$form_state) {

  $vendor = $form_state['vendor'];

  commerce_multivendor_shipping_delete($vendor);

  drupal_set_message(t('The shipping vendor <em>%label</em> has been deleted.', array('%label' => $vendor->label)));
  watchdog('commerce_multivendor_shipping', 'Deleted shipping vendor <em>%label</em>.', array('%label' => $$vendor->label), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/config/multivendorshipping';
}
