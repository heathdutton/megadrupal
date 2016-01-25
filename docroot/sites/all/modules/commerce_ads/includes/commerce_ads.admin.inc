<?php

/**
 * Configuration form to store ads settings.
 */
function commerce_ads_configure_form($form, &$form_state) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  $form['commerce_ads_ws'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('My ADS account'),
  );
  // Shopid.
  $form['commerce_ads_ws']['ads_shopid'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop id'),
    '#description' => t('The unique identifier of the shop'),
    '#default_value' => isset($commerce_ads_ws['ads_shopid']) ? $commerce_ads_ws['ads_shopid'] : '',
    '#required' => TRUE,
  );
  // Authorization key.
  $form['commerce_ads_ws']['ads_authorization_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Authorization key'),
    '#description' => t('The authorization key to include in webservice header calls'),
    '#default_value' => isset($commerce_ads_ws['ads_authorization_key']) ? $commerce_ads_ws['ads_authorization_key'] : '',
    '#required' => TRUE,
  );
  // Mode
  $form['commerce_ads_ws']['ads_ws_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Mode'),
    '#options' => array(
      COMMERCE_ADS_WS_MODE_TEST => ('Test - allow you to test in real conditions.'),
      COMMERCE_ADS_WS_MODE_LIVE => ('Live - process real export import to a live account'),
    ),
   '#default_value' => isset($commerce_ads_ws['ads_ws_mode']) ? $commerce_ads_ws['ads_ws_mode'] : COMMERCE_ADS_WS_MODE_TEST,
  );
  $form['commerce_ads_ws']['ads_debug'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Debug'),
    '#options' => array(
      'log_api_responses' => t('Log full ADS API responses.'),
    ),
    '#default_value' => isset($commerce_ads_ws['ads_debug']) ? $commerce_ads_ws['ads_debug'] : array(),
  );
  $form['help'] = array(
    '#type' => 'markup',
    '#value' => t("If you don't have an ADS account, then please refer to ..."),
  );
  $form['#validate'] = array('commerce_ads_configure_form_validate');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function commerce_ads_configure_form_validate(&$form, &$form_state) {
  if (
    isset($form_state['values']['commerce_ads_ws']['ads_authorization_key'])
    && isset($form_state['values']['commerce_ads_ws']['ads_ws_mode'])
    && !commerce_ads_check_login_ws($form_state['values']['commerce_ads_ws']['ads_ws_mode'], $form_state['values']['commerce_ads_ws']['ads_authorization_key'])
  ) {
    form_set_error('commerce_ads', t('The ADS webservice is not reachable with these credential. Please retry or call your ADS contact.'));
  }
}

function commerce_ads_configure_form_submit($form, &$form_state) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (isset($form_state['values']['commerce_ads_ws']['ads_shopid'])) {
    $commerce_ads_ws['ads_shopid'] = $form_state['values']['commerce_ads_ws']['ads_shopid'];
  }
  if (isset($form_state['values']['commerce_ads_ws']['ads_authorization_key'])) {
    $commerce_ads_ws['ads_authorization_key'] = $form_state['values']['commerce_ads_ws']['ads_authorization_key'];
  }
  if (isset($form_state['values']['commerce_ads_ws']['ads_ws_mode'])) {
    $commerce_ads_ws['ads_ws_mode'] = $form_state['values']['commerce_ads_ws']['ads_ws_mode'];
  }
  if (isset($form_state['values']['commerce_ads_ws']['ads_debug'])) {
    $commerce_ads_ws['ads_debug'] = $form_state['values']['commerce_ads_ws']['ads_debug'];
  }
  variable_set('commerce_ads_ws', $commerce_ads_ws);
  drupal_set_message(t('Configuration saved'));
}

/**
 * Configure product for the ads solution.
 */
function commerce_ads_product_configure_form($form, &$form_state) {
  $form = array();
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  // Product types.
  $form['ads_product_types'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Product types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $types = array_keys(commerce_product_types());
  $form['ads_product_types']['types'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Product types'),
    '#options' => array_combine($types, $types),
    '#description' => t("Select the product types which will be synchronize with ADS. If an order doesn't contains one of this selected product type, it won't be sent to ADS"),
    '#default_value' => !empty($commerce_ads_ws['ads_product_types']) ? $commerce_ads_ws['ads_product_types'] : '',
  );

  // Product field mapping.
  $form['ads_product_mapping'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Product mapping'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (!empty($commerce_ads_ws['ads_product_types'])) {
    $fields = commerce_ads_get_all_product_fields($commerce_ads_ws['ads_product_types']);
    foreach ($commerce_ads_ws['ads_product_fields'] as $ads_field_name => $info) {
      $description = !empty($info['description']) ? $info['description'] : '';
      $values = !empty($info['values']) ? $info['values'] : '';
      $form['ads_product_mapping'][$ads_field_name] = array(
        '#title' => t($ads_field_name),
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => $fields,
        '#description' => $description . '|' . $values,
        '#required' => $info['required'],
        '#default_value' => !empty($commerce_ads_ws['ads_product_mapping'][$ads_field_name]) ? $commerce_ads_ws['ads_product_mapping'][$ads_field_name] : '',
      );
    }
  }
  else {
    $form['ads_product_mapping'] = array(
      '#type' => 'markup',
      '#value' => t("You first have to choose which product types will be synchronize with ADS"),
    );
  }

  // Product synchronization.
  $form['ads_product_synchronization'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Synchronization'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['ads_product_synchronization']['ads_send_newly_created_product'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send ADS the newly created/updated products'),
    '#default_value' => array_key_exists('ads_send_newly_created_product', $commerce_ads_ws) ? $commerce_ads_ws['ads_send_newly_created_product'] : TRUE,
  );
  $form['ads_product_synchronization']['ads_product_send_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Synchronization frequency'),
    '#options' => commerce_ads_frequency_options(),
    '#default_value' => !empty($commerce_ads_ws['ads_product_send_frequency']) ? $commerce_ads_ws['ads_product_send_frequency'] : '',
  );
  $form['ads_product_synchronization']['ads_send_product'] = array(
    '#type' => 'submit',
    '#value' => t('Send all products.'),
    '#submit' => array('commerce_ads_send_all_products_batch_submit'),
  );

  if (module_exists('commerce_stock')) {
    $form['ads_product_stock_synchronization'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Stock synchronization'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['ads_product_stock_synchronization']['ads_product_stock_sync_frequency'] = array(
      '#type' => 'select',
      '#title' => t('Stock synchronization frequency'),
      '#description' => t('Selecting a value will enable stock synchronization for all products on the cron run.'),
      '#options' => commerce_ads_frequency_options(),
      '#default_value' => !empty($commerce_ads_ws['ads_product_stock_sync_frequency']) ? $commerce_ads_ws['ads_product_stock_sync_frequency'] : '',
    );
    $form['ads_product_stock_synchronization']['ads_product_stock_sync_load'] = array(
      '#type' => 'checkbox',
      '#title' => t('Synchronize product stock from ADS on product load'),
      '#description' => t("Enabling this option will synchronize single product's stock each time a product is loaded from the database."),
      '#default_value' => !empty($commerce_ads_ws['ads_product_stock_sync_load']),
    );
    $form['ads_product_stock_synchronization']['ads_product_stock_sync_now'] = array(
      '#type' => 'submit',
      '#value' => t('Synchronize stock for all products now'),
      '#submit' => array('commerce_ads_product_stock_sync_batch_submit'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function commerce_ads_product_configure_form_submit($form, &$form_state) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  $commerce_ads_ws['ads_product_types'] = $form_state['values']['ads_product_types']['types'];
  if (!empty($form_state['values']['ads_product_mapping'])) {
    $commerce_ads_ws['ads_product_mapping'] = $form_state['values']['ads_product_mapping'];
  }

  // Product sync settings.
  $commerce_ads_ws['ads_send_newly_created_product'] = $form_state['values']['ads_product_synchronization']['ads_send_newly_created_product'];
  $commerce_ads_ws['ads_product_send_frequency'] = $form_state['values']['ads_product_synchronization']['ads_product_send_frequency'];

  // Product stock sync settings.
  $commerce_ads_ws['ads_product_stock_sync_frequency'] = $form_state['values']['ads_product_stock_synchronization']['ads_product_stock_sync_frequency'];
  $commerce_ads_ws['ads_product_stock_sync_load'] = $form_state['values']['ads_product_stock_synchronization']['ads_product_stock_sync_load'];

  variable_set('commerce_ads_ws', $commerce_ads_ws);
  drupal_set_message(t('Configuration saved'));
}

function theme_commerce_ads_product_configure_form($variables) {
  $form = array();
  $form['ads_product_types'] = array('#markup' => drupal_render($variables['form']['ads_product_types']));
  $rows = array();
  $header = array(t('Field'), t('Description'), t('Value'), t('Mapping'));
  foreach (element_children($variables['form']['ads_product_mapping']) as $key) {
    $info = explode('|', $variables['form']['ads_product_mapping'][$key]['#description']);
    unset($variables['form']['ads_product_mapping'][$key]['#title']);
    unset($variables['form']['ads_product_mapping'][$key]['#description']);
    $rows[] = array(
      $key,
      !empty($info[0]) ? $info[0] : '',
      !empty($info[1]) ? $info[1] : '',
      drupal_render($variables['form']['ads_product_mapping'][$key]),
    );
  }
  unset($variables['form']['ads_product_mapping']);
  if (!empty($rows)) {
    $form['ads_product_mapping'] = array('#markup' => theme('table', array('header' => $header, 'rows' => $rows)));
  }
  $form['ads_product_synchronization'] = array('#markup' => drupal_render($variables['form']['ads_product_synchronization']));
  $form['ads_product_stock_synchronization'] = array('#markup' => drupal_render($variables['form']['ads_product_stock_synchronization']));
  $form['submit'] = array('#markup' => drupal_render($variables['form']['submit']));
  return drupal_render($form) . drupal_render_children($variables['form']);
}

/**
 * This function extract all field machine name from a product bundle.
 *
 * @param string $bundles
 *   The machine name of the bundle we want to extract fields from.
 * @return array
 *   An array of field keyed by machine name.
 */
function commerce_ads_get_all_product_fields($bundles) {
  $fields = array();
  foreach (field_read_instances(array('entity_type' => 'commerce_product')) as $field) {
    if (in_array($field['bundle'], $bundles)) {
      $fields[$field['field_name']] = $field['label'] . ' - (' . $field['field_name'] . ')';
    }
  }
  return $fields;
}

/**
 * Configure order for the ads solution.
 */
function commerce_ads_order_configure_form($form, &$form_state) {
  $form = array();
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  // Order status.
  $form['ads_order_status'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Order status for validation'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $options = commerce_order_status_options_list();
  if (!empty($commerce_ads_ws['ads_order_status'])) {
    $default = $commerce_ads_ws['ads_order_status'];
  }
  else {
    $default = 'completed';
  }
  $form['ads_order_status']['status'] = array(
    '#type' => 'select',
    '#title' => t('Order validation status'),
    '#options' => $options,
    '#description' => t('Select the order status for which one the order will be sent to ADS.'),
    '#default_value' => $default,
  );

  // Order mapping.
  $form['ads_order_mapping'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Order mapping'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="ads-order-mapping">',
    '#suffix' => '</div>',
  );
  $form['ads_order_mapping']['ads_customer_profile_type'] = array(
    '#type' => 'select',
    '#title' => t('Customer profile to use'),
    '#required' => TRUE,
    '#options' => array('' => t('Select a customer profile')) + commerce_customer_profile_type_options_list(),
    '#description' => t('Which customer profile store the shipping address of your customer?'),
    '#ajax' => array(
      'callback' => 'commerce_ads_ajax_get_profile_fields',
      'wrapper' => 'ads-order-mapping',
    ),
    '#default_value' => !empty($commerce_ads_ws['ads_customer_profile_type']) ? $commerce_ads_ws['ads_customer_profile_type'] : '',
  );

  if (!empty($form_state['values']['ads_order_mapping']['ads_customer_profile_type'])) {
    $customer_profile_type = $form_state['values']['ads_order_mapping']['ads_customer_profile_type'];
  }
  elseif (!empty($commerce_ads_ws['ads_customer_profile_type'])) {
    $customer_profile_type = $commerce_ads_ws['ads_customer_profile_type'];
  }
  if (!empty($customer_profile_type)) {
    foreach($commerce_ads_ws['ads_order_fields']['customer_profile'] as $ads_field_name => $info) {
      $description = !empty($info['description']) ? $info['description'] : '';
      $values = !empty($info['values']) ? $info['values'] : '';
      $form['ads_order_mapping']['fields'][$ads_field_name] = array(
        '#title' => t($ads_field_name),
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => array_merge(commerce_ads_get_all_customer_profile_fields($customer_profile_type), commerce_ads_get_all_user_fields()),
        '#description' => $description . '|' . $values,
        '#required' => $info['required'],
        '#default_value' => !empty($commerce_ads_ws['ads_order_mapping'][$ads_field_name]) ? $commerce_ads_ws['ads_order_mapping'][$ads_field_name] : '',
      );
    }
  }
  foreach($commerce_ads_ws['ads_order_fields']['commerce_order'] as $ads_field_name => $info) {
    $description = !empty($info['description']) ? $info['description'] : '';
    $values = !empty($info['values']) ? $info['values'] : '';
    $form['ads_order_mapping']['fields'][$ads_field_name] = array(
      '#title' => t($ads_field_name),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => commerce_ads_get_all_commerce_order_fields(),
      '#description' => $description. '|' . $values,
      '#required' => $info['required'],
      '#default_value' => !empty($commerce_ads_ws['ads_order_mapping'][$ads_field_name]) ? $commerce_ads_ws['ads_order_mapping'][$ads_field_name] : '',
    );
  }
  // Order synchronization.
  $form['ads_order_synchronization'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Synchronization'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['ads_order_synchronization']['ads_send_newly_created_order'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send to ADS the newly created order (matching the above validation status)'),
    '#default_value' => array_key_exists('ads_send_newly_created_order', $commerce_ads_ws) ? $commerce_ads_ws['ads_send_newly_created_order'] : TRUE,
  );
  $form['ads_order_synchronization']['ads_order_send_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Send to ADS the newly created order (matching the above validation status)'),
    '#options' => commerce_ads_frequency_options(),
    '#default_value' => !empty($commerce_ads_ws['ads_order_send_frequency']) ? $commerce_ads_ws['ads_order_send_frequency'] : '',
  );
  $form['ads_order_synchronization']['ads_send_order'] = array(
    '#type' => 'submit',
    '#value' => t('Send all orders.'),
    '#submit' => array('commerce_ads_send_all_orders_batch_submit'),
  );

  // Order update.
  $form['ads_order_update'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Update'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['ads_order_update']['ads_order_update_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Order update frequency'),
    '#description' => t('Update frequency of an order shipping status once it has been sent to ADS'),
    '#options' => commerce_ads_frequency_options(),
    '#default_value' => !empty($commerce_ads_ws['ads_order_update_frequency']) ? $commerce_ads_ws['ads_order_update_frequency'] : '',
  );
  $form['ads_order_update']['ads_order_update_end'] = array(
    '#type' => 'select',
    '#title' => t('Order blocking status'),
    '#options' => $options,
    '#description' => t('Select the order status for which orders will not be updated any more.'),
    '#default_value' => !empty($commerce_ads_ws['ads_order_update_end']) ? $commerce_ads_ws['ads_order_update_end'] : array(),
    '#multiple' => TRUE,
  );
  $form['ads_order_update']['ads_update_order'] = array(
    '#type' => 'submit',
    '#value' => t('Update all orders.'),
    '#submit' => array('commerce_ads_update_all_orders'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function commerce_ads_order_configure_form_submit($form, &$form_state) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (!empty($form_state['values']['ads_order_status']['status'])) {
    $commerce_ads_ws['ads_order_status'] = $form_state['values']['ads_order_status']['status'];
  }
  if (!empty($form_state['values']['ads_order_mapping']['ads_customer_profile_type'])) {
    $commerce_ads_ws['ads_customer_profile_type'] = $form_state['values']['ads_order_mapping']['ads_customer_profile_type'];
  }
  if (!empty($form_state['values']['ads_order_mapping']['fields'])) {
    $commerce_ads_ws['ads_order_mapping'] = $form_state['values']['ads_order_mapping']['fields'];
  }
  $commerce_ads_ws['ads_send_newly_created_order'] = $form_state['values']['ads_order_synchronization']['ads_send_newly_created_order'];
  if (!empty($form_state['values']['ads_order_synchronization']['ads_order_send_frequency'])) {
    $commerce_ads_ws['ads_order_send_frequency'] = $form_state['values']['ads_order_synchronization']['ads_order_send_frequency'];
  }
  if (!empty($form_state['values']['ads_order_update']['ads_order_update_frequency'])) {
    $commerce_ads_ws['ads_order_update_frequency'] = $form_state['values']['ads_order_update']['ads_order_update_frequency'];
  }
  if (!empty($form_state['values']['ads_order_update']['ads_order_update_end'])) {
    $commerce_ads_ws['ads_order_update_end'] = $form_state['values']['ads_order_update']['ads_order_update_end'];
  }
  variable_set('commerce_ads_ws', $commerce_ads_ws);
  drupal_set_message(t('Configuration saved'));
}

function theme_commerce_ads_order_configure_form($variables) {
  $form = array();
  $form['ads_order_status'] = array('#markup' => drupal_render($variables['form']['ads_order_status']));
  $form['ads_customer_profile_type'] = array('#markup' => drupal_render($variables['form']['ads_order_mapping']['ads_customer_profile_type']));
  $form['fields'] = array(
    '#markup' => theme('commerce_ads_order_mapping', array('form_element_mapping' => $variables['form']['ads_order_mapping']['fields'])),
  );
  $form['ads_order_synchronization'] = array('#markup' => drupal_render($variables['form']['ads_order_synchronization']));
  unset($variables['form']['ads_order_mapping']);

  $form['ads_order_update'] = array('#markup' => drupal_render($variables['form']['ads_order_update']));
  $form['submit'] = array('#markup' => drupal_render($variables['form']['submit']));
  return drupal_render($form) . drupal_render_children($variables['form']);
}

/**
 * Ajax callback to get customer profile type fields
 */
function commerce_ads_ajax_get_profile_fields($form, $form_state) {
  if (!empty($form_state['values']['ads_order_mapping']['ads_customer_profile_type'])) {
    return theme('commerce_ads_order_mapping', array('form_element_mapping' => $form['ads_order_mapping']['fields']));
  }
  else {
    return '<div id="ads-order-mapping"></div>';
  }
}

/**
 * Theme function to render mapping as table.
 */
function theme_commerce_ads_order_mapping($variables) {
  if (!empty($variables['form_element_mapping'])) {
    $rows = array();
    $header = array(t('Field'), t('Description'), t('Value'), t('Mapping'));
    foreach (element_children($variables['form_element_mapping']) as $key) {
      $info = explode('|', $variables['form_element_mapping'][$key]['#description']);
      $rows[] = array(
        $key,
        !empty($info[0]) ? $info[0] : '',
        !empty($info[1]) ? $info[1] : '',
        drupal_render($variables['form_element_mapping'][$key]),
      );
    }
    if (!empty($rows)) {
      return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => array('ads-order-mapping'))));
    }
  }
  return '<div id="ads-order-mapping"></div>';
}

/**
 * Get all customer profile fields.
 *
 * @param string $customer_profile_type
 *   The customer profile type we want to extract all fields from.
 * @return array
 *   An array of field keyed by machine name.
 */
function commerce_ads_get_all_customer_profile_fields($customer_profile_type) {
  $fields = array();
  $adressfield = FALSE;
  $customer_profile_fields = field_info_instances('commerce_customer_profile', $customer_profile_type);
  foreach($customer_profile_fields as $field) {
    if ($field['widget']['module'] !== "addressfield") {
      $fields[$field['field_name']] = $field['label'] . ' - (' . $field['field_name'] . ')';
    }
    else {
      $adressfield = $field['field_name'];
    }
  }
  if ($adressfield) {
    foreach (addressfield_data_property_info() as $address_field_name => $address_field_info) {
      $fields[$adressfield . '|' . $address_field_name] = $address_field_info['label'] . ' - (' . $address_field_name . ')';
    }
  }
  return $fields;
}

/**
 * Get all commerce order fields.
 */
function commerce_ads_get_all_commerce_order_fields() {
  $fields = array();
  foreach(field_info_instances('commerce_order', 'commerce_order') as $field) {
    if (!in_array($field['widget']['module'], array('commerce_line_item', 'commerce_customer', 'entityreference', 'commerce_price'))) {
      $fields[$field['field_name']] = $field['label'] . ' - (' . $field['field_name'] . ')';
    }
  }
  return $fields;
}

/**
 * Configuration form to store shipping settings.
 */
function commerce_ads_shipping_configure_form($form, &$form_state) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());

  // Shipping statuses.
  $form['ads_shipping_statuses'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Shipping statuses'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (!empty($commerce_ads_ws['ads_ws_mode']) && !empty($commerce_ads_ws['ads_authorization_key'])) {
    foreach (commerce_ads_shipping_statuses() as $ads_status) {
      $form['ads_shipping_statuses'][$ads_status['StateId']] = array(
        '#type' => 'select',
        '#title' => t($ads_status['Label']),
        '#options' => commerce_order_status_options_list(),
        '#default_value' => !empty($commerce_ads_ws['ads_shipping_statuses'][$ads_status['StateId']]) ? $commerce_ads_ws['ads_shipping_statuses'][$ads_status['StateId']] : '',
      );
    }
  }
  else {
    $form['ads_shipping_statuses']['help'] = array(
      '#markup' => t('You have to set shopip in "Configure" tab first'),
    );
  }

  // Shipping services
  $form['ads_shipping_services'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Shipping services'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  foreach (commerce_shipping_services() as $services) {
    $form['ads_shipping_services'][$services['name']] = array(
      '#type' => 'select',
      '#title' => t($services['display_title']),
      '#options' => commerce_ads_shipping_deliverymanid(),
      '#default_value' => !empty($commerce_ads_ws['ads_shipping_services'][$services['name']]) ? $commerce_ads_ws['ads_shipping_services'][$services['name']] : '',
    );
  }

  $form['ads_shipping_services']['help'] = array(
    '#markup' => t('Enter ADS shipping codes for each services. Contact ADS to get these codes.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function commerce_ads_shipping_configure_form_submit($form, &$form_state) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (!empty($form_state['values']['ads_shipping_services'])) {
    $commerce_ads_ws['ads_shipping_services'] = $form_state['values']['ads_shipping_services'];
  }
  if (!empty($form_state['values']['ads_shipping_statuses'])) {
    $commerce_ads_ws['ads_shipping_statuses'] = $form_state['values']['ads_shipping_statuses'];
  }
  variable_set('commerce_ads_ws', $commerce_ads_ws);
  drupal_set_message(t('Configuration saved'));
}

/**
 * This function return an option list with all the user field.
 */
function commerce_ads_get_all_user_fields() {
  $user_fields = field_info_instances('user');
  $fields = array();
  if (!empty($user_fields['user'])) {
    foreach ($user_fields['user'] as $field) {
      $fields[$field['field_name']] = $field['label'] . ' (' . $field['field_name'] . ')';
    }
  }
  return $fields;
}

function commerce_ads_shipping_statuses() {
  $response = commerce_ads_request_ads('api/customerOrders/getStatusList');
  if ($response['Code'] === 0) {
    return $response['Data'];
  }
  else {
    return array();
  }
}

function commerce_ads_send_all_products_batch_submit($form, &$form_state) {
  // Set variable to mark batch update running. This will disable
  // parallel calls to ADS API while products are being updated.
  variable_set(COMMERCE_ADS_WS_PRODUCT_UPDATE_RUNNING, TRUE);

  batch_set(
    array(
      'operations' => array(
        array('commerce_ads_send_all_products', array())
      ),
      'finished' => 'commerce_ads_batch_send_all_products_finished',
      'file' => drupal_get_path('module', 'commerce_ads') . '/includes/commerce_ads.batch.inc',
    )
  );
}

function commerce_ads_send_all_orders_batch_submit($form, &$form_state) {
  // Set variable to mark batch update running. This will disable
  // parallel calls to ADS API while products are being updated.
  variable_set(COMMERCE_ADS_WS_ORDER_UPDATE_RUNNING, TRUE);

  batch_set(
    array(
      'operations' => array(
        array('commerce_ads_send_all_orders', array())
      ),
      'finished' => 'commerce_ads_batch_send_all_orders_finished',
      'file' => drupal_get_path('module', 'commerce_ads') . '/includes/commerce_ads.batch.inc',
    )
  );
}

/**
 * Form submission callback: sets batch to synchronize all products stock.
 *
 * @see commerce_ads_product_configure_form()
 * @see commerce_ads_batch_product_stock_sync_process()
 * @see commerce_ads_batch_product_stock_sync_finished()
 */
function commerce_ads_product_stock_sync_batch_submit($form, &$form_state) {
  // Get stock for all products from ADS.
  $stocks = commerce_ads_get_stock_for_all_products();

  // Set variable to mark batch update running. This will disable
  // parallel calls to ADS API while products are being updated.
  variable_set(COMMERCE_ADS_WS_PRODUCT_UPDATE_RUNNING, TRUE);

  // Prepare batch.
  $batch = array(
    'title' => t('Synchronize stock for all products'),
    'operations' => array(),
    'finished' => 'commerce_ads_batch_product_stock_sync_finished',
    'file' => drupal_get_path('module', 'commerce_ads') . '/includes/commerce_ads.batch.inc',
  );

  if (!empty($stocks)) {
    foreach ($stocks as $stock) {
      $batch['operations'][] = array('commerce_ads_batch_product_stock_sync_process', array($stock));
    }
  }

  batch_set($batch);
}

/**
 * Returns options array for cron frequency form select element.
 *
 * @return array
 *   Options array for cron frequency form select element.
 */
function commerce_ads_frequency_options() {
  $frequency_options = array(
    '' => t('Select a value'),
    '300' => t('5 minutes'),
    '600' => t('10 minutes'),
    '1800' => t('30 minutes'),
    '3600' => t('1 hour'),
    '7200' => t('2 hours'),
    '21600' => t('6 hours'),
    '43200' => t('12 hours'),
    '86400' => t('1 day'),
  );

  // Allow other modules to alter frequency options.
  drupal_alter('commerce_ads_frequency_options', $frequency_options);

  return $frequency_options;
}

function commerce_ads_shipping_deliverymanid() {
  $shippings = array('' => t('Select a matching ads delivery method'));
  $response = commerce_ads_request_ads('api/customerOrders/getDeliverymanIDList');
  if ($response['Code'] === 0) {
    foreach ($response['Data'] as $deliverymanid) {
      $shippings[$deliverymanid['DeliverymanID']] = $deliverymanid['Label'];
    }
  }
  return $shippings;
}
