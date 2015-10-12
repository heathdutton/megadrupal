<?php

/**
 * @file
 * Admin form and validation for Shipwire shipments.
 */

/**
 * Settings form for Shipwire account.
 */
function shipwire_admin_account_settings() {
  $form = array();

  $operational = Shipwire::operational();

  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server settings'),
    '#description' => t('For security reasons do not use your primary Shipwire'
      . ' production server login here. Instead create and use an API sub account.'
      . ' For detailed instructions on account settings please look at the'
      . ' <a href="http://www.codamoda.com/shipwire-guide/shipwire-module-configuration">'
      . 'Shipwire module configuration guide</a>'),
    '#collapsible' => FALSE,
  );
  $form['server']['shipwire_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#description' => t('Shipwire account email address.'),
    '#required' => !$operational,
  );
  $form['server']['shipwire_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Shipwire account password.'),
    '#required' => !$operational,
  );
  $form['server']['shipwire_server'] = array(
    '#type' => 'select',
    '#title' => t('Server'),
    '#description' => t('Set to %production after testing and when you wish to go live.', array(
      '%production' => t('Production'))),
    '#options' => array('Test' => t('Test'), 'Production' => t('Production')),
    '#default_value' => variable_get('shipwire_server', 'Production'),
  );
  $form['server']['shipwire_response'] = array(
    '#type' => 'select',
    '#title' => t('Response'),
    '#description' => t('Set to %real when you wish to go live. Both %test and'
      . ' %production servers can reply with %fake or %real responses. A %fake'
      . ' reply will contain generic data that is not in reponse to requests.'
      . ' This is useful when the %test account does not have any products in'
      . ' warehouses. Once products are added use %real responses for proper'
      . ' testing.', array(
      '%fake' => t('Fake'), '%real' => t('Real'),
      '%test' => t('Test'), '%production' => t('Production'),
      )
    ),
    '#options' => array('Test' => t('Fake'), 'Production' => t('Real')),
    '#default_value' => variable_get('shipwire_response', 'Production'),
  );

  $form['#submit'][] = 'shipwire_admin_account_settings_submit';
  return system_settings_form($form);
}

/**
 * Validation for account settings form.
 */
function shipwire_admin_account_settings_validate($form, &$form_state) {
  $fsv = &$form_state['values'];
  $username = $fsv['shipwire_username'];
  $password = $fsv['shipwire_password'];

  if (empty($username) && !empty($password)) {
    form_set_error('shipwire_username', t('Username must be provided when password is provided.'));
  }
  if (!empty($username) && empty($password)) {
    form_set_error('shipwire_password', t('Password must be provided when username is provided.'));
  }
}

/**
 * Submit handler for account settings form.
 */
function shipwire_admin_account_settings_submit($form, &$form_state) {
  $shipwire = Shipwire::getInstance();
  $fsv = &$form_state['values'];

  if (!empty($fsv['shipwire_username']) && !empty($fsv['shipwire_password'])) {

    $username = filter_xss($fsv['shipwire_username']);
    $password = filter_xss($fsv['shipwire_password']);
    $shipwire->accountSave($username, $password);

    unset($fsv['shipwire_username'], $fsv['shipwire_password']);
  }
}

/**
 * Admin form for Shipwire Cron settings.
 */
function shipwire_admin_cron_settings() {
  $form = array();

  if (!Shipwire::operational()) {
    $form['configure'] = array(
      '#markup' => t('Shipwire account settings must be configured first.') . ' '
      . l(t('Configure Shipwire'), 'admin/shipwire/settings'),
    );
    return system_settings_form($form);
  }

  $form['cron'] = array(
    '#markup' => t('This module depends on CRON to execute tasks at set intervals.'
      . ' The following setting limit the frequency and number of items'
      . ' processed during each CRON run and cannot increase them. For detailed'
      . ' information about configuring Cron and these settings please look at'
      . ' the <a href="">Cache and CRON settings guide</a>'),
  );

  $delay_options = array(
    '3600' => t('@one hour', array('@one' => '1')),
    '7200' => t('@num hours', array('@num' => '2')),
    '10800' => t('@num hours', array('@num' => '3')),
    '14400' => t('@num hours', array('@num' => '4')),
    '18000' => t('@num hours', array('@num' => '5')),
    '21600' => t('@num hours', array('@num' => '6')),
    '25200' => t('@num hours', array('@num' => '7')),
    '28800' => t('@num hours', array('@num' => '8')),
    '32400' => t('@num hours', array('@num' => '9')),
    '36000' => t('@num hours', array('@num' => '10')),
    '39600' => t('@num hours', array('@num' => '11')),
    '43200' => t('@num hours', array('@num' => '12')),
  );

  $form['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rates cache'),
    '#collapsible' => TRUE,
  );
  $form['cache']['shipwire_cache_life'] = array(
    '#type' => 'select',
    '#title' => t('Rates cache lifetime'),
    '#description' => t('During the checkout process shipping rates can be'
      . ' requested multiple times. This module caches shipping rates to reduce'
      . ' the number of requests, speed up checkout, and pass data to shipment'
      . ' tracking once the order is completed. The cache is invalidated if'
      . ' order products or address change during checkout and Shipwire rates'
      . ' do not change frequently so it is safe to set this to'
      . ' a high level.'),
    '#options' => $delay_options,
    '#default_value' => variable_get('shipwire_cache_life', 3600),
  );

  $form['fulfill'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipment fulfillment'),
    '#description' => t('If fulfillment is set to execute in batches per CRON'
      . ' run then these setting will limit the number of fulfillment requests.'),
    '#collapsible' => TRUE,
  );
  $form['fulfill']['shipwire_fulfill_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum shipments fulfilled per request'),
    '#default_value' => variable_get('shipwire_fulfill_max', 200),
    '#size' => 5,
    '#required' => TRUE,
  );
  $form['fulfill']['shipwire_fulfill_freq'] = array(
    '#type' => 'select',
    '#title' => t('Fulfillment frequency'),
    '#description' => t('How often shipments are submitted for fulfillment.'),
    '#options' => $delay_options,
    '#default_value' => variable_get('shipwire_fullfill_freq', 3600),
  );

  $form['track'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipment tracking'),
    '#collapsible' => TRUE,
  );
  $form['track']['shipwire_track_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum shipments tracked per request'),
    '#default_value' => variable_get('shipwire_track_max', 200),
    '#size' => 5,
    '#required' => TRUE,
  );
  $form['track']['shipwire_track_freq'] = array(
    '#type' => 'select',
    '#title' => t('Tracking frequency'),
    '#description' => t('How often shipments are checked for tracking update.'),
    '#options' => $delay_options,
    '#default_value' => variable_get('shipwire_track_freq', 14400),
  );

  $form['inventory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inventory tracking'),
    '#collapsible' => TRUE,
  );
  $form['inventory']['shipwire_inventory_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum products check per inventory request'),
    '#default_value' => variable_get('shipwire_inventory_max', 200),
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['inventory']['shipwire_inventory_freq'] = array(
    '#type' => 'select',
    '#title' => t('Inventory frequency'),
    '#description' => t('How often product inventory is checked.'),
    '#options' => $delay_options,
    '#default_value' => variable_get('shipwire_inventory_freq', 28800),
  );

  $form['#validate'][] = 'shipwire_admin_cron_settings_validate';
  return system_settings_form($form);
}

/**
 * Validation for cron settings form.
 */
function shipwire_admin_cron_settings_validate($form, &$form_state) {
  $fsv = $form_state['values'];
  $fields = array(
    'shipwire_fulfill_max',
    'shipwire_track_max',
    'shipwire_inventory_max');
  foreach ($fields as $field) {
    $value = $fsv[$field];
    if (preg_match('[^0-9]', $value)) {
      form_set_error($field, t('Maximum request value must be an integer.'));
    }
  }
}

/**
 * Admin form for Shipwire miscellaneous settings.
 */
function shipwire_admin_misc_settings() {
  $form = array();

  if (!Shipwire::operational()) {
    $form['configure'] = array(
      '#markup' => t('Shipwire account settings must be configured first.') . ' '
      . l(t('Configure Shipwire'), 'admin/shipwire/settings'),
    );
    return system_settings_form($form);
  }
  $options = array(1 => t('Yes'), 0 => t('No'));

  $form['shipwire_email_share'] = array(
    '#type' => 'radios',
    '#title' => t('Share customer emails'),
    '#options' => $options,
    '#default_value' => variable_get('shipwire_email_share', 0),
    '#description' => t('Do you want to pass customer email addresses to'
      . ' Shipwire when shipments are submitted? When enabled it will allow'
      . ' you to configure "Delight" tools in Shipwire to send customer'
      . ' notifications.'),
  );

  $form['logs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Log data'),
    '#description' => t('Log data using Watchdog.') . ' ' . l(t('Recent log messages'), 'admin/reports/dblog'),
  );
  $form['logs']['shipwire_log_errors'] = array(
    '#type' => 'radios',
    '#title' => t('Log errors'),
    '#options' => $options,
    '#default_value' => variable_get('shipwire_log_errors', 0),
    '#description' => t('Critical errors will stop execution. Non critical'
      . ' errors will be displayed on screen to administrators. When enabled'
      . ' this option will additionally log non critical errors.'),
  );
  $form['logs']['shipwire_log_xml_request'] = array(
    '#type' => 'radios',
    '#title' => t('Log XML requests'),
    '#options' => $options,
    '#default_value' => variable_get('shipwire_log_xml_request', 0),
    '#description' => t('Warning: XML requests contain raw account information and can be large.'),
  );
  $form['logs']['shipwire_log_xml_response'] = array(
    '#type' => 'radios',
    '#title' => t('Log XML responses'),
    '#options' => $options,
    '#default_value' => variable_get('shipwire_log_xml_response', 0),
    '#description' => t('Warning: XML responses can be large.'),
  );

  return system_settings_form($form);
}

/**
 * Shipwire shipment edit form.
 */
function shipwire_shipment_edit($form, &$form_state, ShipwireShipment $shipment) {
  $form_state['shipwire_shipment'] = $shipment;

  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipment details'),
    '#collapsible' => TRUE,
  );

  $form['details']['shipwire_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipwire ID'),
    '#default_value' => isset($shipment->shipwire_id) ? $shipment->shipwire_id : '',
  );

  $carrier_options = Shipwire::getCarriers();
  foreach ($carrier_options as $code => $name) {
    $carrier_options[$code] = $name . ' [' . $code . ']';
  }

  $form['details']['carrier_code'] = array(
    '#type' => 'select',
    '#title' => t('Carrier'),
    '#options' => $carrier_options,
    '#default_value' => isset($shipment->carrier_code) ? $shipment->carrier_code : '',
  );

  $warehouse_options = array('' => '');
  $warehouse_options += Shipwire::getWarehouses(TRUE, FALSE);

  $form['details']['warehouse_code'] = array(
    '#type' => 'select',
    '#title' => t('Warehouse'),
    '#options' => $warehouse_options,
    '#default_value' => isset($shipment->warehouse_code) ? $shipment->warehouse_code : '',
  );

  $form['details']['tracking_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking number'),
    '#default_value' => isset($shipment->tracking_number) ? $shipment->tracking_number : '',
  );

  $form['details']['tracking_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking URL'),
    '#default_value' => isset($shipment->tracking_uri) ? $shipment->tracking_uri : '',
  );

  $form['dates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipment dates'),
    '#collapsible' => TRUE,
  );
  $date_submitted = array(
    '#name' => 'date_submitted',
    '#title' => t('Date submitted'),
    '#title_empty' => t('Not submitted'),
    '#default_value' => $shipment->date_submitted,
  );
  $form['dates'] += shipwire_date_optional_element($date_submitted);

  $date_shipped = array(
    '#name' => 'date_shipped',
    '#title' => t('Date shipped'),
    '#title_empty' => t('Not shipped'),
    '#default_value' => $shipment->date_shipped,
  );
  $form['dates'] += shipwire_date_optional_element($date_shipped);

  $date_expected = array(
    '#name' => 'date_expected',
    '#title' => t('Date expected'),
    '#title_empty' => t('Not expected'),
    '#default_value' => $shipment->date_expected,
  );
  $form['dates'] += shipwire_date_optional_element($date_expected);

  $date_delivered = array(
    '#name' => 'date_delivered',
    '#title' => t('Date delivered'),
    '#title_empty' => t('Not delivered'),
    '#default_value' => $shipment->date_delivered,
  );
  $form['dates'] += shipwire_date_optional_element($date_delivered);

  $currency_options = Shipwire::getCurrencies();
  foreach ($currency_options as $code => $name) {
    $currency_options[$code] = $name . ' [' . $code . ']';
  }

  $form['costs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipment costs'),
    '#collapsible' => TRUE,
  );

  $currency_default = isset($shipment->currency) ? $shipment->currency : '';

  $form['costs']['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => $currency_options,
    '#default_value' => $currency_default,
  );

  $form['costs']['cost_total'] = array(
    '#type' => 'textfield',
    '#title' => t('Total cost'),
    '#default_value' => isset($shipment->cost_total) ? $shipment->cost_total : 0,
    '#size' => 10,
  );
  $form['costs']['cost_freight'] = array(
    '#type' => 'textfield',
    '#title' => t('Freight cost'),
    '#default_value' => isset($shipment->cost_freight) ? $shipment->cost_freight : 0,
    '#size' => 10,
  );
  $form['costs']['cost_handling'] = array(
    '#type' => 'textfield',
    '#title' => t('Handling cost'),
    '#default_value' => isset($shipment->cost_handling) ? $shipment->cost_handling : 0,
    '#size' => 10,
  );
  $form['costs']['cost_packaging'] = array(
    '#type' => 'textfield',
    '#title' => t('Packaging cost'),
    '#default_value' => isset($shipment->cost_packaging) ? $shipment->cost_packaging : 0,
    '#size' => 10,
  );
  $form['costs']['cost_insurance'] = array(
    '#type' => 'textfield',
    '#title' => t('Insurance cost'),
    '#default_value' => isset($shipment->cost_insurance) ? $shipment->cost_insurance : 0,
    '#size' => 10,
  );

  $form['#validate'][] = 'shipwire_shipment_edit_validate';

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save shipment'),
    '#weight' => 10,
    '#submit' => array_merge($submit, array('shipwire_shipment_edit_submit')),
  );

  return $form;
}

/**
 * Validation handler for Shipwire shipment edit form.
 */
function shipwire_shipment_edit_validate($form, &$form_state) {
  $fsv = $form_state['values'];
  $price_fields = array('cost_total', 'cost_freight', 'cost_handling',
    'cost_packaging', 'cost_insurance');
  foreach ($price_fields as $field) {
    if (!empty($fsv[$field]) && (!is_numeric($fsv[$field]))) {
      $label = $form['costs'][$field]['#title'];
      form_set_error($field, t('%name: you must enter a numeric value for the price.', array('%name' => $label)));
    }
  }

  if (!empty($fsv['tracking_uri'])) {
    if (!valid_url($fsv['tracking_uri'], TRUE)) {
      form_set_error('tracking_uri', t('The URL %url is invalid. Enter a fully-qualified URL, such as http://www.example.com/feed.xml.', array('%url' => $fsv['tracking_uri'])));
    }
  }
}

/**
 * Submit handler for Shipwire shipment edit form.
 */
function shipwire_shipment_edit_submit($form, &$form_state) {
  $fsv = $form_state['values'];
  $shipment = $form_state['shipwire_shipment'];

  $date_fields = array(
    'date_submitted', 'date_shipped',
    'date_expected', 'date_delivered',
  );
  foreach ($date_fields as $field) {
    $field_empty = $field . '_empty';
    $timestamp = '0';
    if (empty($fsv[$field_empty]) && !empty($fsv[$field])) {
      $org = getdate();
      if (!empty($shipment->{$field})) {
        $org = getdate($shipment->{$field});
      }
      $org['mday'] = (int) $fsv[$field]['day'];
      $org['mon'] = (int) $fsv[$field]['month'];
      $org['year'] = (int) $fsv[$field]['year'];

      $timestamp = mktime($org['hours'], $org['minutes'], $org['seconds'], $org['mon'], $org['mday'], $org['year']);
    }
    $shipment->{$field} = $timestamp;
  }

  $simple_fields = array('shipwire_id', 'carrier_code', 'warehouse_code',
    'tracking_number', 'tracking_uri', 'currency', 'cost_total', 'cost_freight',
    'cost_handling', 'cost_packaging', 'cost_insurance');
  foreach ($simple_fields as $field) {
    if (isset($shipment->{$field})) {
      $shipment->{$field} = $fsv[$field];
    }
  }
  $shipment->save();

  $form_state['redirect'] = 'admin/shipwire/shipment/' . $shipment->shipwire_shipment_id;
}

/**
 * Callback form to delete Shipwire Shipment.
 *
 * @see shipwire_shipment_delete_confirm_submit()
 * @ingroup forms
 */
function shipwire_shipment_delete_confirm($form, &$form_state, ShipwireShipment $shipment) {
  $form['shipwire_shipment_id'] = array('#type' => 'value', '#value' => $shipment->shipwire_shipment_id);
  $question = t('Are you sure you want to delete this shipment?');
  $path_return = 'admin/shipwire/shipments';
  $path_merchant = 'https://app.shipwire.com/sign-in';

  $description = t('This shipment has not been submitted to Shipwire. Deleting'
    . ' it here will cancel the shipment and products will not be shipped.');
  if (!empty($shipment->date_submitted)) {
    $description = t('This shipment was already submitted to Shipwire and'
      . ' cannot be canceled using the API. Deleting the shipment here will'
      . ' only remove information that may be needed. It is recommended that'
      . ' you first cancel the shipment using the !shipwire_login and then'
      . ' it is safe to delete it here.', array(
      '!shipwire_login' => l('Shipwire Merchant Panel', $path_merchant))
    );
  }

  return confirm_form($form, $question, $path_return, $description, t('Delete'), t('Cancel'));
}

/**
 * Submit handler for shipwire_shipment_delete_confirm().
 *
 * @see shipwire_shipment_delete_confirm()
 */
function shipwire_shipment_delete_confirm_submit($form, &$form_state) {
  $shipment = shipwire_shipment_load($form_state['values']['shipwire_shipment_id']);
  shipwire_shipment_delete($shipment);
}

/**
 * Displays shipment details.
 */
function shipwire_shipment_view(ShipwireShipment $shipment, $view_mode = 'full') {
  $build = array();

  $dates = array();
  $dates[] = array(t('Submitted:'), (empty($shipment->date_submitted)) ? '' : format_date($shipment->date_submitted));
  $dates[] = array(t('Shipped:'), (empty($shipment->date_shipped)) ? '' : format_date($shipment->date_shipped));
  $dates[] = array(t('Expected:'), (empty($shipment->date_expected)) ? '' : format_date($shipment->date_expected));
  $dates[] = array(t('Delivered:'), (empty($shipment->date_delivered)) ? '' : format_date($shipment->date_delivered));

  $build['dates'] = array(
    '#theme' => 'table',
    '#rows' => $dates,
    '#attributes' => array('style' => 'width: auto'),
    '#prefix' => '<div class="shipwire-pane"><div class="shipwire-pane-title">' . t('Schedule') . ':' . '</div>',
    '#suffix' => '</div>',
  );

  $details = array();
  $details[] = array(t('Method:'), Shipwire::getShippingMethodLabel($shipment->shipping_method));
  $details[] = array(t('Carrier code:'), $shipment->carrier_code);
  $details[] = array(t('Carrier:'), Shipwire::getCarrierName($shipment->carrier_code));
  $details[] = array(t('Shipwire ID:'), $shipment->shipwire_id);
  $details[] = array(t('Warehouse:'), Shipwire::getWarehouseLabel($shipment->warehouse_code));
  if (!empty($shipment->tracking_number)) {
    $details[] = array(
      t('Tracking:'),
      l($shipment->tracking_number, $shipment->tracking_uri),
    );
  }

  $build['details'] = array(
    '#theme' => 'table',
    '#rows' => $details,
    '#attributes' => array('style' => 'width:auto;'),
    '#prefix' => '<div class="shipwire-pane"><div class="shipwire-pane-title">' . t('Details') . ':' . '</div>',
    '#suffix' => '</div>',
  );

  $costs = array();
  $costs[] = array(
    t('Freight:'),
    shipwire_currency_format($shipment->cost_freight, $shipment->currency),
  );
  $costs[] = array(
    t('Packaging:'),
    shipwire_currency_format($shipment->cost_packaging, $shipment->currency),
  );
  $costs[] = array(
    t('Handling:'),
    shipwire_currency_format($shipment->cost_handling, $shipment->currency),
  );
  $costs[] = array(
    t('Insurance:'),
    shipwire_currency_format($shipment->cost_insurance, $shipment->currency),
  );
  $costs[] = array(
    t('Total:'),
    shipwire_currency_format($shipment->cost_total, $shipment->currency),
  );

  $build['cost'] = array(
    '#theme' => 'table',
    '#rows' => $costs,
    '#attributes' => array('style' => 'width:auto;'),
    '#prefix' => '<div class="shipwire-pane"><div class="shipwire-pane-title">' . t('Costs') . ':' . '</div>',
    '#suffix' => '</div>',
  );

  if (!empty($shipment->messages)) {
    $messages_output = '<ul class="shipwire-messages">';
    foreach ($shipment->messages as $message) {
      $messages_output .= '<li class="' . $message['type'] . '">' . $message['message'] . '</li>';
    }
    $messages_output .= '</ul>';
    $build['messages'] = array(
      '#type' => 'markup',
      '#value' => $messages_output,
    );
  }

  return $build;
}

/**
 * Summary view of shipments.
 */
function shipwire_shipments_summary() {
  $result_warehouses = db_query('SELECT DISTINCT warehouse_code FROM {shipwire_shipments}');
  $sql_counts = 'SELECT status, count(status) as ship_count, warehouse_code'
    . ' FROM {shipwire_shipments} GROUP BY warehouse_code, status';

  $warehouses = array();
  while ($row = $result_warehouses->fetchObject()) {
    $warehouse_code = $row->warehouse_code;
    $result_counts = db_query($sql_counts, array(':warehouse_code' => $warehouse_code));
    $counts = array();
    while ($count = $result_counts->fetchObject()) {
      $counts[$count->status] = $count->ship_count;
    }
    $warehouses[$warehouse_code] = $counts;
  }
}

/**
 * Build date form elements to support optional dates.
 * 
 * @param array $element
 *   The source date form element values
 * 
 * @return array
 *   Drupal form elements arrays.
 */
function shipwire_date_optional_element($element) {
  if (empty($element['#default_value'])) {
    $date = getdate();
    $date_empty = TRUE;
  }
  else {
    $date = getdate($element['#default_value']);
    $date_empty = FALSE;
  }

  $date_element = $element['#name'];
  $wrapper_element = $element['#name'] . '_wrapper';
  $empty_element = $element['#name'] . '_empty';

  $return = array();

  $return[$wrapper_element] = array(
    '#type' => 'item',
    '#title' => $element['#title'],
    '#prefix' => '<div style="container-inline">',
    '#suffix' => '</div>',
  );

  $return[$wrapper_element][$date_element] = array(
    '#type' => 'date',
    '#default_value' => array(
      'year' => $date['year'],
      'month' => $date['mon'],
      'day' => $date['mday'],
    ),
    '#element_validate' => array('date_validate'),
    '#states' => array(
      'disabled' => array(
        '[name="' . $empty_element . '"]' => array('checked' => TRUE),
      ),
    ),
  );
  $return[$wrapper_element][$empty_element] = array(
    '#type' => 'checkbox',
    '#title' => $element['#title_empty'],
    '#default_value' => $date_empty,
  );
  return $return;
}
