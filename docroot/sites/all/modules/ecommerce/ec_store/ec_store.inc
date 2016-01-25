<?php

/**
 * @file
 * Code here is not specific to any process, might be any user, and is not run infrequently.
 */

/**
 * Saves all addresses in transaction using ec_store_transaction_address_save().
 *
 * @param $txn
 *   Object, transaction with address array.
 */
function ec_store_transaction_addresses_save($txn) {
  // If we where not passed a valid transaction and we can't save.
  if (!isset($txn->txnid) || !is_array($txn->address)) {
    return FALSE;
  }
  foreach ($txn->address as $type => $address) {
    $address['txnid'] = $txn->txnid;
    ec_store_transaction_address_save($address, $type);
  }
}

/**
 * Saves an individual address to the passed transaction.
 *
 * @param $address
 *   Object, address object.
 * @param $type
 *   String, address type(corresponds to type field in database).
 */
function ec_store_transaction_address_save($address, $type) {
  $address['type'] = $type;

  // see if we can do an insert first, and then to an update.
  try {
    drupal_write_record('ec_transaction_address', $address);
  }
  catch (Exception $e) {
    drupal_write_record('ec_transaction_address', $address, array('txnid', 'type'));
  }
}

/**
 * Creates a standard address form.
 * When passed a display field we filter out all other sections and display only
 * the one given. If passed shipping only a shipping address will be shown.
 *
 * @param $txn
 *   Objectm this is a transaction object used for default values.
 * @param $display
 *   String, option field that allows the form to filter sections.
 */
function ec_store_transaction_addresses_form($txn, $display = NULL) {
  $form['address']['#tree'] = TRUE;

  $form['address']['shipping'] = ec_store_address_form($txn->address['shipping']);
  $form['address']['shipping']['#type'] = 'fieldset';
  $form['address']['shipping']['#title'] = t('Shipping address');

  $form['address']['billing'] = ec_store_address_form($txn->address['billing']);
  $form['address']['billing']['#type'] = 'fieldset';
  $form['address']['billing']['#title'] = t('Billing address');

  switch ($display) {
    case 'shipping':
      unset($form['billing']);
      break;

    case 'billing':
      unset($form['shipping']);
      break;
  }
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_address_form($address = array()) {

  module_load_include('inc', 'ec_store', 'ec_store.localization');

  $address += array(
    'firstname' => '',
    'lastname' => '',
    'street1' => '',
    'street2' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => variable_get('ec_country', ''),
  );

  $regions = ec_store_get_region_info(NULL, array('state_name', 'states', 'zip_name'));

  $settings = array(
    'eCommerce' => array('regions' => $regions),
  );

  $form = array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'ec_store') . '/css/address.css',
      ),
      'js' => array(
        drupal_get_path('module', 'ec_store') . '/js/address.js',
        array('data' => $settings, 'type' => 'setting', 'scope' => JS_DEFAULT),
      )
    ),
  );

  if (isset($address['type'])) {
    $form['type'] = array(
      '#type' => 'hidden',
      '#value' => $address['type'],
    );
  }

  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => $address['firstname'],
    '#size' => 32,
    '#maxlength' => 32,
    '#required_external' => TRUE,
  );
  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => $address['lastname'],
    '#size' => 32,
    '#maxlength' => 32,
    '#required_external' => TRUE,
  );
  $form['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => $address['country'],
    '#options' => ec_store_region_country_list(),
    '#attributes' => array(
      'class' => array('address-country-id'),
    ),
  );
  $form['street1'] = array(
    '#type' => 'textfield',
    '#title' => t('Street'),
    '#default_value' => $address['street1'],
    '#size' => 60,
    '#maxlength' => 64,
    '#required_external' => TRUE,
  );
  $form['street2'] = array(
    '#type' => 'textfield',
    '#default_value' => $address['street2'],
    '#size' => 60,
    '#maxlength' => 64,
  );
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $address['city'],
    '#size' => 32,
    '#maxlength' => 32,
    '#required_external' => TRUE,
  );
  $state_select = array(
    '#type' => 'select',
    '#title' => t('State/Province'),
    '#options' => array(),
    '#parents' => array('state_select'),
    '#name' => '',
    '#id' => '',
    '#value' => '',
  );
  $info = element_info('select');
  $state_select += $info;
  $form['state'] = array(
    '#type' => 'textfield',
    '#title' => t('State/Province'),
    '#default_value' => $address['state'],
    '#size' => 32,
    '#maxlength' => 32,
    '#prefix' => '<div class="state-text-wrapper">',
    '#suffix' => '</div><div class="state-select-wrapper">' . drupal_render($state_select) . '</div>',
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip/Postal Code'),
    '#default_value' => $address['zip'],
    '#size' => 10,
    '#maxlength' => 10,
    '#prefix' => '<div class="zip-wrapper">',
    '#suffix' => '</div>',
  );

  $form_state = array('values' => $address);
  $data = &$form;
  $context = array(&$form_state);
  drupal_alter('form_ec_store_address_form', $data, $context);
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_address_form_validate(&$form, &$values) {
  $region = ec_store_get_region_info($values['country'], array('states', 'state_name'));
  
  foreach (array('firstname', 'lastname', 'street1', 'city') as $field) {
    if (isset($values[$field]) && !$values[$field]) {
      form_error($form[$field], t('!type !name field is required.', array('!name' => $form[$field]['#title'], '!type' => drupal_ucfirst($form['#address_type']))));
    }
  }

  if (isset($region['states']) && empty($region['states'][drupal_strtoupper($values['state'])])) {
    form_set_error(implode('][', $form['state']['#parents']), t('Please select a !type @state', array('!type' => $form['#address_type'], '@state' => drupal_strtolower($region['state_name']))));
  }
}

/**
 * A wrapper for theme('store_invoice').
 *
 * Because the menu callback can only handle functions with no parameters, we
 * use this function as a wrapper.
 */
function ec_store_invoice($txn) {
  return theme('invoice', array('transaction' => $txn));
}

/**
 * Allow a user to cancel an invoice.
 */
function ec_store_invoice_cancel($form, &$form_state, $txn) {
  $form = array();

  $form['invoice'] = array(
    '#value' => theme('invoice', array('transaction' => $txn)),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );

  $form['txn'] = array(
    '#type' => 'value',
    '#value' => $txn,
  );

  return confirm_form($form,
    t('Do you really want to cancel this transaction?'),
    '',
    t('Cancelling this transaction will stop delivery of these items.'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_invoice_cancel_submit(&$form, &$form_state) {
  $workflow = db_query_range('SELECT workflow FROM {ec_workflow_statuses} WHERE type = :type ORDER BY weight ASC', array(':type' => EC_WORKFLOW_TYPE_CANCEL))->fetchField();
  $txn = (object) array(
    'txnid' => $form_state['values']['txn']->txnid,
    'workflow' => $workflow,
  );

  ec_store_transaction_save($txn);

  drupal_set_message(t('Transaction %txnid has been cancelled', array('%txnid' => $txn->txnid)));

  $form_state['redirect'] = '';
}
