<?php

/**
 * Administrative settings
 * Income, COGS and Asset account references must be set in order for QBWC to
 * create inventory products out of Commerce products. Only Income account is
 * necessary for non-inventory products.
 */
function commerce_qb_webconnect_admin_form() {

  // Income account.
  $form['commerce_qb_webconnect_income_account'] = array(
    '#prefix' => '<h3>' . t('Quickbooks account/service references') . '</h3>',
    '#title' => t('Main income account'),
    '#description' => t('When exporting products through Quickbooks WebConnect, the resulting Quickbooks products will be linked to this account'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_qb_webconnect_income_account', ''),
  );

  // COGS (Cost of Goods Sold) account.
  $form['commerce_qb_webconnect_cogs_account'] = array(
    '#title' => t('COGS account'),
    '#description' => t('Provide the name of the Cost of Goods Sold (COGS) account for exported products.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_qb_webconnect_cogs_account', ''),
  );

  // Assets account.
  $form['commerce_qb_webconnect_assets_account'] = array(
    '#title' => t('Assets account'),
    '#description' => t('Provide the name of the Assets account for exported products.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_qb_webconnect_assets_account', ''),
  );

  // Product type.
  $form['commerce_qb_webconnect_product_type'] = array(
    '#title' => t('Type of item to create in quickbooks'),
    '#description' => t('At this point only inventory and non-inventory items are supported.'),
    '#type' => 'select',
    '#options' => array(
      'add_inventory_product' => t('Inventory'),
      'add_non_inventory_product' => t('Non-inventory'),
    ),
    '#default_value' => variable_get('commerce_qb_webconnect_product_type', 'add_non_inventory_product'),
  );

  // Shipping settings.
  if (module_exists('commerce_shipping')) {

    // Shipping service item.
    $form['commerce_qb_webconnect_shipping_service'] = array(
      '#prefix' => '<h3>' . t('Shipping settings') . '</h3><p>' . t('Commerce QB Webconnect can optionally direct shipping costs into a separate account, specified here.') . '</p>',
      '#type' => 'textfield',
      '#title' => t('Shipping service'),
      '#description' => t('Provide the name of default shipping service-item so that Quickbooks can keep track of shipping charges'),
      '#default_value' => variable_get('commerce_qb_webconnect_shipping_service', ''),
    );

    // Shipping service description.
    $form['commerce_qb_webconnect_shipping_service_description'] = array(
      '#type' => 'textfield',
      '#title' => t('Shipping service description'),
      '#description' => t('Provide a description for the shipping charge. This will show up on the Quickbooks invoice'),
      '#default_value' => variable_get('commerce_qb_webconnect_shipping_service_description', ''),
    );
  }

  // Log file settings.
  // Enable logging.
  $form['commerce_qb_webconnect_enable_logging'] = array(
    '#prefix' => '<h3>' . t('Log file settings') . '</h3>',
    '#title' => t('Enable logging'),
    '#description' => t('Write QB Webconnect update details to log file'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('commerce_qb_webconnect_enable_logging', 0),
  );

  // Truncate log.
  $form['commerce_qb_webconnect_truncate_log'] = array(
    '#title' => t('Truncate log before update'),
    '#description' => t('If set, each update run will overwrite the last one recorded in the log.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('commerce_qb_webconnect_truncate_log', 0),
  );

  $scheme = variable_get('file_default_scheme', 'public');
  $path = variable_get('file_' . $scheme . '_path', '');

  // Log filepath.
  $form['commerce_qb_webconnect_log_filepath'] = array(
      '#title' => t('Path to log file'),
    '#description' => t('Must be writeable by webserver'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_qb_webconnect_log_filepath', $path . '/commerce_qb_webconnect'),
  );

  $states = commerce_order_states();

  foreach ($states as $name => $state) {
    $state_options[$name] = $state['title'];
  }

  $defaults = variable_get('commerce_qb_webconnect_export_statuses', array());

  // Export status triggers.
  $form['commerce_qb_webconnect_export_statuses'] = array(
    '#prefix' => '<h3>' . t('Export settings') . '</h3>',
    '#type' => 'checkboxes',
    '#title' => t('Order status export trigger'),
    '#description' => t('When an order is created having one of these defined statuses, it will create a record in the export queue.'),
    '#default_value' => $defaults,
    '#options' => $state_options,
  );

  $info = commerce_qb_webconnect_export_info();

  foreach ($info as $name => $export_type) {
    $exportables[$name] = $export_type['label'];

    if (isset($export_type['order_export']) && $export_type['order_export']) {
      $order_exportables[$name] = $export_type['label'];
    }
  }

  // Enabled exportables.
  if (isset($exportables)) {
    $form['commerce_qb_webconnect_exportables'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Exportables'),
      '#description' => t('Specify which kinds of exportables you want to enable.'),
      '#options' => $exportables,
      '#default_value' => variable_get('commerce_qb_webconnect_exportables', array()),
    );
  }

  // Order export type.
  if (isset($order_exportables)) {
    $form['commerce_qb_webconnect_order_export_type'] = array(
      '#type' => 'select',
      '#title' => t('Order export type'),
      '#description' => t('Export action to take when orders are created.'),
      '#options' => $order_exportables,
      '#default_value' => variable_get('commerce_qb_webconnect_order_export_type', 'add_sales_receipt'),
    );
  }

  $profile_fields = field_info_instances('commerce_customer_profile');
  $phone_options = array();

  foreach ($profile_fields as $bundle_name => $instances) {
    foreach ($instances as $instance) {

      // This has to be a text field.
      if ($instance['widget']['module'] == 'text') {
        $phone_options[$bundle_name][$instance['id']] = $instance['label'];
      }
    }
  }

  // Field mappings.
  if (!empty($phone_options)) {
    $form['commerce_qb_webconnect_customer_phone_field'] = array(
      '#prefix' => '<h3>' . t('Field mappings') . '</h3>',
      '#type' => 'select',
      '#title' => t('Customer phone field'),
      '#description' => t('Specify a customer profile field to be exported as customer phone'),
      '#options' => $phone_options,
      '#default_value' => variable_get('commerce_qb_webconnect_customer_phone_field', NULL),
    );
  }

  if (module_exists('commerce_payment_example')) {
    $form['commerce_qb_webconnect_example_payment_method'] = array(
      '#type' => 'textfield',
      '#title' => t('Quickbooks payment method for example payments'),
      '#description' => t('Set this value so that the example payment method can create payment records in Quickbooks'),
      '#default_value' => variable_get('commerce_qb_webconnect_example_payment_method', ''),
    );
  }

  return system_settings_form($form);
}

/**
 * Render callback for manage page.
 */
function commerce_qb_webconnect_manage() {
  $form = drupal_get_form('commerce_qb_webconnect_manage_form');
  $view = views_embed_view('quickbooks_exports', 'default');
  return drupal_render($form) . $view;
}

/**
 * Form part of the manage page.
 */
function commerce_qb_webconnect_manage_form() {

  // Update exports button.
  $form['update_exports'] = array(
    '#type' => 'submit',
    '#value' => t('Update exports table'),
    '#suffix' => '<div ' . drupal_attributes(array('class' => array('description'))) . '>' . t('Populate exports table with new orders, products and customers.') . '</div>',
  );

  return $form;
}

/**
 * Submit handler for admin form.
 */
function commerce_qb_webconnect_manage_form_submit(&$form, &$form_state) {
  if ($form_state['triggering_element']['#parents'][0] == 'update_exports') {
    if ($orders = commerce_qb_webconnect_exportable_orders()) {
      // We take care of adding customers when we add the invoices.
      $export_entities['add_invoice'] = $orders;
    }
    $export_entities['add_product'] = entity_load('commerce_product');

    foreach ($export_entities as $type => $type_entity_list) {
      foreach ($type_entity_list as $type_entity) {
        $operations[] = array('commerce_qb_webconnect_qb_export_batch_save', array($type, $type_entity));
      }
    }

    $batch = array(
      'operations' => $operations,
      'finished' => 'commerce_qb_webconnect_batch_export_update_finished',
    );

    batch_set($batch);
  }
}

/**
 * Validation handler for admin form.
 */
function commerce_qb_webconnect_admin_form_validate(&$form, &$form_state) {
  $path = $form_state['values']['commerce_qb_webconnect_log_filepath'];

  if (!is_writable($path) && !mkdir($path)) {
    form_set_error('commerce_qb_webconnect_log_filepath', t('File path ' . $path . 'is not writeable'));
  }
}
