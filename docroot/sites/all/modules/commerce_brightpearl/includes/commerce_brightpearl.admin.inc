<?php

/**
 * @file
 * Brightpearl admin pages.
 */

/**
 * The overview form.
 */
function commerce_brightpearl_overview_form() {
  $form['brightpearl_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl overview'),
  );
  $brightpearltest = commerce_brightpearl_get_object();
  if (!$brightpearltest) {
    drupal_set_message(t('You are currently not connected to Brightpearl. You can still configure the stock integration but it will have no effect until connected.'), 'error');
  }

  return system_settings_form($form);
}

/**
 * The brightpearl connection form.
 */
function commerce_brightpearl_connect_form() {

  $form['brightpearl_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl Authentication'),
  );

  $brightpearltest = commerce_brightpearl_get_object();
  if (!$brightpearltest) {
    $form['brightpearl_config']['info'] = array(
      '#markup' => t('<div class="messages error">You are currently not connected to Brightpearl, Please provide the needed authentication details below.</div>'),
    );
  }
  else {
    $form['brightpearl_config']['info'] = array(
      '#markup' => t('<div class="messages status">You are connected to Brightpearl, you can updated your details below (you will need to provide your signed token if you do).</div>'),
    );
  }

  $form['brightpearl_config']['commerce_brightpearl_customer_account_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Code'),
    '#default_value' => variable_get('commerce_brightpearl_customer_account_code'),
    '#description' => t('The Customer account code.'),
    '#required' => TRUE,
  );
  $form['brightpearl_config']['commerce_brightpearl_datacentre'] = array(
    '#type' => 'textfield',
    '#title' => t('Datacentre'),
    '#default_value' => variable_get('commerce_brightpearl_datacentre'),
    '#description' => t('The customer datacentre code.'),
    '#required' => TRUE,
  );
  $form['brightpearl_config']['commerce_brightpearl_signed_account_token'] = array(
    '#type' => 'password',
    '#title' => t('Token'),
    '#default_value' => variable_get('commerce_brightpearl_signed_account_token'),
    '#description' => t('The signed account token provided on registration.'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 100,
  );
  return $form;
}

/**
 * Submit function for the connection form.
 */
function commerce_brightpearl_connect_form_submit($form, &$form_state) {
  // Test connection.
  $brightpearltest = new BrightPearl();

  $brightpearltest->setAccountCode($form_state['values']['commerce_brightpearl_customer_account_code']);
  $brightpearltest->setSignedToken($form_state['values']['commerce_brightpearl_signed_account_token']);
  $brightpearltest->setDatacentre($form_state['values']['commerce_brightpearl_datacentre']);
  $brightpearltest->setLogAllRequests(FALSE);
  if ($brightpearltest->makeGetRequest('product-service/channel/')) {
    // Set the variables.
    variable_set('commerce_brightpearl_customer_account_code', $form_state['values']['commerce_brightpearl_customer_account_code']);
    variable_set('commerce_brightpearl_datacentre', $form_state['values']['commerce_brightpearl_datacentre']);
    variable_set('commerce_brightpearl_signed_account_token', $form_state['values']['commerce_brightpearl_signed_account_token']);
    drupal_set_message(t('Connection to brightpearl authenticate.'), 'status', FALSE);
  }
  else {
    drupal_set_message(t('The details you provided did not authenticate - Not saved.'), 'error', FALSE);
  }
}

/**
 * Form builder for brightpearl settings.
 */
function commerce_brightpearl_config_form() {
  $options = _commerce_brightpearl_admin_get_options();
  if (empty($options)) {
    drupal_goto('admin/commerce/config/brightpearl/connect');
  }

  $form['brightpearl_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl module configuration'),
  );
  $form['brightpearl_config']['commerce_brightpearl_log_all_requests'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log all requests'),
    '#description' => t('If set all requests made to brightpearl will be logged in the watchdog. !!Don not use on a live production site!!'),
    '#default_value' => variable_get('commerce_brightpearl_log_all_requests', FALSE),
  );

  $form['brightpearl_config']['commerce_brightpearl_product_minimum_cache_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum cache days'),
    '#default_value' => variable_get('commerce_brightpearl_product_minimum_cache_days', '1'),
    '#description' => t('The minimum number of days to cache products.'),
    '#required' => TRUE,
  );

  $form['brightpearl_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl Settings'),
  );

  $form['brightpearl_settings']['commerce_brightpearl_tax_code'] = array(
    '#type' => 'select',
    '#title' => t('Tax code'),
    '#options' => $options['tax_codes'],
    '#default_value' => variable_get('commerce_brightpearl_tax_code'),
    '#description' => t("The brightpearl tax code to use."),
    '#required' => TRUE,
  );

  $form['brightpearl_settings']['commerce_brightpearl_channel_id'] = array(
    '#type' => 'select',
    '#title' => t('Channel'),
    '#options' => $options['channels'],
    '#default_value' => variable_get('commerce_brightpearl_channel_id'),
    '#description' => t('The brightpearl channel to use.'),
    '#required' => TRUE,
  );

  $form['brightpearl_settings']['commerce_brightpearl_warehouse_id'] = array(
    '#type' => 'select',
    '#title' => t('Warehouse'),
    '#options' => $options['warehouses'],
    '#default_value' => variable_get('commerce_brightpearl_warehouse_id'),
    '#description' => t('The brightpearl warehouse to use.'),
    '#required' => TRUE,
  );
  $form['brightpearl_settings']['commerce_brightpearl_warehouse_stock_check_all'] = array(
    '#type' => 'select',
    '#title' => t('Check all available stock'),
    '#options' => array(0 => 'Selected Warehouse', 1 => 'All on Hand stock'),
    '#default_value' => variable_get('commerce_brightpearl_warehouse_stock_check_all'),
    '#description' => t('If Selected Warehouse option then only the selected warehouse is checked else all on hand stock is reported.'),
    '#required' => TRUE,
  );

  $form['brightpearl_settings']['commerce_brightpearl_new_order_status_id'] = array(
    '#type' => 'select',
    '#title' => t('Order Status'),
    '#options' => $options['status_list'],
    '#default_value' => variable_get('commerce_brightpearl_new_order_status_id'),
    '#description' => t('The brightpearl Status for new orders created by the Drupal site.'),
    '#required' => TRUE,
  );

  $form['brightpearl_settings']['commerce_brightpearl_bank_account_nominal_code'] = array(
    '#type' => 'select',
    '#title' => t('Bank account nominal code'),
    '#options' => $options['nominal_codes'],
    '#default_value' => variable_get('commerce_brightpearl_bank_account_nominal_code'),
    '#description' => t('The brightpearl Bank account nominal code to use.'),
    '#required' => TRUE,
  );

  $form['brightpearl_settings']['commerce_brightpearl_shipping_nominal_code'] = array(
    '#type' => 'select',
    '#title' => t('Shipping nominal code'),
    '#options' => $options['nominal_codes'],
    '#default_value' => variable_get('commerce_brightpearl_shipping_nominal_code'),
    '#description' => t('The brightpearl shipping nominal code to use.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}


/**
 * The brightpearl stock form.
 */
function commerce_brightpearl_stock_form() {
  $brightpearltest = commerce_brightpearl_get_object();
  if (!$brightpearltest) {
    drupal_set_message(t('You are currently not connected to Brightpearl. You can still configure the stock integration but it will have no effect until connected.'), 'error');
  }
  $form['brightpearl_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl Stock integration'),
  );

  if (module_exists('commerce_ss')) {
    $form['brightpearl_config']['commerce_ss'] = array(
      '#markup' => t('<div class="messages status">Stock integration is enabled. to configure stock got to the !url</div>', array('!url' => l(t('simple stock configuration screen'), "admin/commerce/config/stock/ss"))),
    );
  }
  else {
    $form['brightpearl_config']['commerce_ss'] = array(
      '#markup' => t('<div class="messages error">Stock integration is not enabled. you will need to install the commerce stock module available from !url</div>', array('!url' => l(t('drupal.org'), "https://www.drupal.org/project/commerce_stock"))),
    );
  }

  $form['brightpearl_config']['stock_update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl Stock update type'),
  );
  $form['brightpearl_config']['stock_update']['update_type'] = array(
    '#type' => 'select',
    '#title' => t('Update type'),
    '#options' => array(
      'disabled' => t('No updates'),
      'webhook' => t('Real time using a webhook'),
      'cron' => 'Full update on cron run',
    ),
    '#default_value' => variable_get('commerce_brightpearl_stock_update_type', 'disabled'),
    '#description' => t('Full update on cron run.'),
  );
  $form['brightpearl_config']['stock_update']['batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch size'),
    '#description' => t('Number of products to update each cron run.'),
    '#default_value' => variable_get('brightpearl_stock_update_batch_size', 100),
    '#element_validate' => array('element_validate_integer_positive'),
  );


  // Disable if not enabled.
  if (!module_exists('commerce_ss') || !$brightpearltest) {
    $form['brightpearl_config']['stock_update']['update_type']['#disabled'] = TRUE;
    $form['brightpearl_config']['stock_update']['update_type']['#description'] = t('Stock needs to be installed and the brightpearl connection made before this can be set.');
  }
  else {
    $form['brightpearl_config']['stock_init'] = array(
      '#type' => 'fieldset',
      '#title' => t('Brightpearl Stock Initialisation'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['brightpearl_config']['stock_init']['log'] = array(
      '#markup' => t('<div class="brightpearl_stock_log"!log</div>', array('!log' => commerce_brightpearl_get_stock_log())),
    );

    $form['brightpearl_config']['stock_init']['init_stock_run'] = array(
      '#type' => 'submit',
      '#value' => t('Initialise stock level of all products.'),
      '#submit' => array('commerce_brightpearl_stock_form_init_stock'),
      '#weight' => 100,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 100,
  );
  return $form;
}

function commerce_brightpearl_get_stock_log() {
  $pid = variable_get('brightpearl_last_updated_pid', 0);
  $init_stock = variable_get('brightpearl_init_stock', FALSE);
  $updated_count = variable_get('brightpearl_updated_count', 0);
  $log_array = variable_get('brightpearl_full_stock_update_log', array());
  $log = t('Log:</br>') . implode('</br>', $log_array);
  if ($init_stock) {
    if ($updated_count > 0) {
      $state = t('<div>Full stock initilasation is curently running and !count products have been processed.</div>', array('!count' => $updated_count) );
    }
    else {
      $state = t('<div>Full stock initilasation is curently running.</div>');
    }
    return $state .'</br>'. $log;
  }
  else {
    return $log;
  }

}

function commerce_brightpearl_stock_form_init_stock($form, &$form_state) {
  drupal_set_message(t('Stock is being updated from Brightpearl.'));
  variable_set('brightpearl_last_updated_pid', 0);
  variable_set('brightpearl_updated_count', 0);
  variable_set('brightpearl_init_stock', TRUE);

  // Update the log
  $update_log = variable_get('brightpearl_full_stock_update_log', array());
  $update_log[] = t('Started on !start_time', array('!start_time' => date('l jS \of F Y h:i:s A')));
  if (count($update_log) > 6) {
    array_shift($update_log);
  }
  variable_set('brightpearl_full_stock_update_log', $update_log);

}

/**
 * The brightpearl stock form validation.
 */
function commerce_brightpearl_stock_form_validate($form, &$form_state) {
  global $base_url;

  // Set the batch size.
  $batch_size = $form['brightpearl_config']['stock_update']['batch_size']['#value'];
  variable_set('brightpearl_stock_update_batch_size', $batch_size);

  $update_type = $form['brightpearl_config']['stock_update']['update_type']['#value'];

  $brightpearl = commerce_brightpearl_get_object(FALSE);

  if ($update_type == 'webhook') {
    $webhook_id = variable_get('commerce_brightpearl_stock_webhook_id');
    // If we dont have a webhook.
    if (!$webhook_id) {
      // Register the stock webhook.
      $uri = $base_url . '/brightpearl/event';
      $communication_token = commerce_brightpearl_get_user_communication_token();
      $webhook_id = $brightpearl->registerStockWebhook($uri, $communication_token);
      if ($webhook_id) {
        variable_set('commerce_brightpearl_stock_webhook_id', $webhook_id);
        variable_set('commerce_brightpearl_stock_update_type', $update_type);
      }
      else {
        form_set_error('update_type', t('Problem registering the Web Hook! Option not saved.'));
      }
    }
  }

  // Disable & cron options.
  else {
    // If we have a webhook delete it.
    $webhook_id = variable_get('commerce_brightpearl_stock_webhook_id');
    // If we dont have a webhook.
    if ($webhook_id) {
      // Delete it.
      if ($brightpearl->deleteWebhook($webhook_id)) {
      }
      variable_set('commerce_brightpearl_stock_webhook_id', FALSE);
    }
    // Set the update option.
    variable_set('commerce_brightpearl_stock_update_type', $update_type);
  }
}


function commerce_brightpearl_admin_form() {
  $brightpearltest = commerce_brightpearl_get_object();
  if (!$brightpearltest) {
    drupal_set_message(t('You are currently not connected to Brightpearl. You can still configure the stock integration but it will have no effect until connected.'), 'error');
  }
  $form['brightpearl_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('administration and alerts'),
  );
  $form['brightpearl_config']['commerce_brightpearl_notify_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Alerts Email'),
    '#description' => t('Email to use for sending alerts & notifications to.'),
    '#default_value' => variable_get('commerce_brightpearl_notify_email', ''),
  );
  $form['brightpearl_order_alerts'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Order Alerts'),
  );
  $log = implode('</br>', variable_get('commerce_brightpearl_order_notifications', array()));
  $form['brightpearl_order_alerts']['log'] = array(
    '#markup' => t('<div class="brightpearl_order_alerts">!log</div>', array('!log' => $log)),
  );
  $form['brightpearl_stock_alerts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl Stock Initialisation'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['brightpearl_stock_alerts']['stock_init'] = array(
    '#markup' => t('<div class="brightpearl_stock_log"!log</div>', array('!log' => commerce_brightpearl_get_stock_log())),
  );

  $form['brightpearl_failed_orders'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl Failed Orders'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  //$log = implode('</br>', variable_get('brightpearl_failed_orders', array()));

  $log = '';
  $failed_orders = variable_get('brightpearl_failed_orders', array());
  foreach ($failed_orders as $key => $value) {
    $log .= 'Order ID: ' . $key . ' = ' . $value . ' erros</br>';
  }
  $form['brightpearl_failed_orders']['failed_orders'] = array(
    '#markup' => t('<div class="brightpearl_failed_order_log">!log</div>', array('!log' => $log)),
  );
  $form['brightpearl_failed_orders']['clear_failed_orders'] = array(
    '#type' => 'submit',
    '#value' => t('Clear failed orders'),
    '#submit' => array('commerce_brightpearl_clear_failed_orders'),
  );

  return system_settings_form($form);
}


function commerce_brightpearl_clear_failed_orders($form, &$form_state) {
  $failed_orders = array();
  variable_set('brightpearl_failed_orders', $failed_orders);
  drupal_set_message('Failed orders cleared');
}

/**
 * The brightpearl orders form.
 */
function commerce_brightpearl_orders_form($form, &$form_state) {
  $brightpearltest = commerce_brightpearl_get_object();
  if (!$brightpearltest) {
    drupal_set_message(t('You are currently not connected to Brightpearl. You can still configure the order integration but it will have no effect until connected.'), 'error', FALSE);
  }

  $form['brightpearl_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brightpearl Orders integration'),
  );

  $instance = field_info_instance('commerce_order', 'brightpearl_processed', 'commerce_order');

  $form['brightpearl_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#weight' => 10,
  );
  $form['brightpearl_settings']['order_state'] = array(
    '#type' => 'select',
    '#title' => t('Completed order state'),
    '#options' => _commerce_brightpearl_orders_get_order_states(),
    '#default_value' => variable_get('commerce_brightpearl_order_state', 'pending'),
    '#description' => t('The state the order needs to get to so it is reported to Brightpearl.'),
  );
  $form['brightpearl_settings']['order_integration_state'] = array(
    '#type' => 'select',
    '#title' => t('Order integration state'),
    '#options' => array(1 => t('Export orders'), 0 => t('Suspended')),
    '#default_value' => variable_get('commerce_brightpearl_order_integration_state', 1),
    '#description' => t('The state the order integration.'),
  );

  // Turn on/off integration.
  $form['brightpearl_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Integration state'),
    '#weight' => 20,
  );
  if (!empty($instance)) {
    $form['brightpearl_config']['info'] = array(
      '#markup' => t('The order integration is enabled.'),
    );
    $form['brightpearl_config']['delete'] = array(
      '#type' => 'fieldset',
      '#title' => t('Remove integration'),
      '#description' => t("Use if you are removing the functionality from your system and don't want to keep any records relating to the Brightpearl integration."),
    );
    $form['brightpearl_config']['delete']['delete_order_integration'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove order integration.'),
      '#field_prefix' => t('<div>WARNING: This will cause permanent data loss.</div>'),
      '#default_value' => FALSE,
    );
  }
  else {
    $form['brightpearl_config']['info'] = array(
      '#markup' => t('<p>Order integration needs enabling. Once enabled all orders that are placed will be processed by brightpearl).</p>'),
    );
    $form['brightpearl_config']['enable_order_integration'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable order integration.'),
      '#default_value' => FALSE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 100,
  );
  return $form;
}

/**
 * Add or remove the Stock field from product types.
 */
function commerce_brightpearl_orders_form_submit($form, &$form_state) {

  // Configuration.
  variable_set('commerce_brightpearl_order_state', $form_state['values']['order_state']);
  variable_set('commerce_brightpearl_order_integration_state', $form_state['values']['order_integration_state']);

  // Integration.

  // Add the processed field.
  if (isset($form_state['values']['enable_order_integration']) && $form_state['values']['enable_order_integration']) {
    // If the field is allready created we will come out.
    $instance = field_info_instance('commerce_order', 'brightpearl_processed', 'commerce_order');
    if (!empty($instance)) {
      return;
    }
    // Create the instance.
    commerce_brightpearl_admin_create_brightpearl_processed_field();
    drupal_set_message(t('Enabled order integration.'));
  }

  // Remove the processed field.
  elseif ($form_state['values']['delete_order_integration']) {
    $instance = field_info_instance('commerce_order', 'brightpearl_processed', 'commerce_order');
    if (empty($instance)) {
      return;
    }
    field_delete_instance($instance);
  }

}


/**
 * Create the order brightpearl processed field.
 */
function commerce_brightpearl_admin_create_brightpearl_processed_field() {
  $label = t('Brightpearl processed');
  $description = t('A Unix timestamp holding the time the order was pushed to brightpearl');
  $entity_type = 'commerce_order';
  $bundle = 'commerce_order';
  $field_name = 'brightpearl_processed';
  $field_type = 'number_integer';
  $required = FALSE;
  $weight = 0;

  $field = field_info_field($field_name);
  $instance = field_info_instance($entity_type, $field_name, $bundle);

  if (empty($field)) {
    $field = array(
      'field_name' => $field_name,
      'type' => $field_type,
      'cardinality' => 1,
      'entity_types' => array($entity_type),
      'translatable' => FALSE,
      'locked' => FALSE,
    );
    $field = field_create_field($field);
  }

  if (empty($instance)) {
    $instance = array(
      'field_name' => $field_name,
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'label' => $label,
      'required' => $required,
      'settings' => array(),
      'display' => array(),
      'description' => $description,
      'default_value' => array(array('value' => "0")),
      'widget' => array(
        'type' => 'brightpearl_processed',
      ),
      'display' => array(
        'default' => array(
          'type' => 'hidden',
        ),
      ),
    );
    field_create_instance($instance);
  }
}


/**
 * Get the list of Drupal commerce order states.
 */
function _commerce_brightpearl_orders_get_order_states() {
  $sts_list = commerce_order_statuses();
  $formated_list = array();
  foreach ($sts_list as $key => $value) {
    $formated_list[$key] = $value['title'];
  }
  return $formated_list;
}

/**
 * Get lists of Brightpearl options.
 */
function _commerce_brightpearl_admin_get_options() {
  $options = array();
  $brightpearlconfig = commerce_brightpearl_get_object();
  if (!$brightpearlconfig) {
    return $options;
  }
  // Get channels.
  $response = $brightpearlconfig->getChannels();
  $options['channels'] = _commerce_brightpearl_admin_extract_options($response, 'id', 'name');

  // Get tax codes.
  $response = $brightpearlconfig->getTaxCodes();
  $options['tax_codes'] = _commerce_brightpearl_admin_extract_options($response, 'code', 'description');

  // Get warehouses.
  $response = $brightpearlconfig->getWarehouses();
  $options['warehouses'] = _commerce_brightpearl_admin_extract_options($response, 'id', 'name');

  // Get status list.
  $response = $brightpearlconfig->getOrderStatusList();
  $options['status_list'] = _commerce_brightpearl_admin_extract_options($response, 'statusId', 'name');

  // Get nominal codes.
  $response = $brightpearlconfig->getNominalCodes();
  $options['nominal_codes'] = _commerce_brightpearl_admin_extract_options($response, 'code', 'name', TRUE);

  return $options;
}

/**
 * Extract the key & value from the brightpearl response.
 */
function _commerce_brightpearl_admin_extract_options($response, $okey = 'id', $ovalue = 'name', $inc_key = FALSE) {
  $options = array();
  if (isset($response['response'])) {
    foreach ($response['response'] as $key => $value) {
      if (isset($value[$okey]) && isset($value[$ovalue])) {
        if ($inc_key) {
          $options[$value[$okey]] = $value[$okey] . ' - ' . $value[$ovalue];
        }
        else {
          $options[$value[$okey]] = $value[$ovalue];
        }

      }
    }
  }
  return $options;

}
