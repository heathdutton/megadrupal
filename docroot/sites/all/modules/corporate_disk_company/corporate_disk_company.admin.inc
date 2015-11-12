<?php

/**
 * @file
 * Corporate Disk Fulfillment settings administration UI.
 */

/**
 * Form definition; admin settings.
 */
function corporate_disk_admin_settings($form, &$form_state) {
  $form = array();
  $form['corporate_disk_company_intro'] = array(
    '#markup' => 'Options for Corporate Disk Company Fulfillment.',
  );
  $form['corporate_disk_company_live_status'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction Type Mode'),
    '#options' => array(
      '0' => t('Test Transactions'),
      '1' => t('Live Transactions'),
    ),
    '#description' => t('What mode would you like the transactions to be?'),
    '#default_value' => variable_get('corporate_disk_company_live_status', 0),
    '#ajax' => array(
      'callback' => 'corporate_disk_company_live_testing',
      'wrapper' => 'corporate-disk-company-ajax-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['corporate_disk_company_live_test'] = array(
    '#prefix' => '<div id="corporate-disk-company-ajax-wrapper">',
    '#suffix' => '</div>',
  );
  $live_test_value = variable_get('corporate_disk_company_live_status', 0);
  if ((isset($form_state['values']['corporate_disk_company_live_status']) && $form_state['values']['corporate_disk_company_live_status'] > 0) || $live_test_value == 1) {
    $form['corporate_disk_company_live_test'] = array(
      '#prefix' => '<div id="corporate-disk-company-ajax-wrapper">',
      '#suffix' => '</div>',
      '#title' => t('Test your live transaction.'),
      '#type' => 'checkbox',
      '#description' => t('You can submit information to Disk.com that will not be processed. This sets the "attention" field to "TEST".'),
      '#default_value' => variable_get('corporate_disk_company_live_test', 0),
    );
  }

  if (isset($form_state['values']['corporate_disk_company_live_status']) && $form_state['values']['corporate_disk_company_live_status'] == 0) {
    $form['corporate_disk_company_live_test'] = array(
      '#prefix' => '<div id="corporate-disk-company-ajax-wrapper">',
      '#suffix' => '</div>',
    );
  }
  $form['corporate_disk_company_companyid'] = array(
    '#type' => 'textfield',
    '#title' => t('Company ID'),
    '#description' => t('Disk.com company id number.'),
    '#size' => 5,
    '#default_value' => variable_get('corporate_disk_company_companyid'),
    '#required' => TRUE,
  );
  $form['corporate_disk_company_requested_by_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Requested by name'),
    '#description' => t('This is the requested by field in Disk.com order entries.'),
    '#size' => 10,
    '#default_value' => variable_get('corporate_disk_company_requested_by_name'),
    '#required' => TRUE,
  );
  $form['corporate_disk_company_order_view_message_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Order View Footer Message'),
    '#description' => t('This message will appear below the display of the tracking numbers in the order view.  You can use HTML in this field for links.'),
    '#rows' => 3,
    '#cols' => 60,
    '#default_value' => variable_get('corporate_disk_company_order_view_message_footer'),
    '#required' => FALSE,
  );
  $form['cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron Settings'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['cron']['corporate_disk_company_cron_status'] = array(
    '#title' => t('Enable cron to run.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('corporate_disk_company_cron_status', 0),
  );
  $form['cron']['corporate_disk_company_interval'] = array(
    '#title' => t('Cron Interval'),
    '#description' => t('Select the interval for fulfillment to be run.'),
    '#type' => 'select',
    '#options' => array(
      3600 => t('1 hour'),
      7200 => t('2 hours'),
      10800 => t('3 hours'),
      21600 => t('6 hours'),
      43200 => t('12 hours'),
      86400 => t('24 hours'),
    ),
    '#default_value' => variable_get('corporate_disk_company_interval', array()),
  );
  $form['cron']['corporate_disk_company_backtrack_time'] = array(
    '#title' => t('Backtrack Interval'),
    '#description' => t('Select the amount of time to backtrack on the order query that pulls in the disk orders.'),
    '#type' => 'select',
    '#options' => array(
      86400 => t('24 hours'),
      172800 => t('48 hours'),
      259200 => t('72 hours'),
      345600 => t('96 hours'),
      604800 => t('1 week'),
    ),
    '#default_value' => variable_get('corporate_disk_company_backtrack_time', array()),
  );
  $form['sku'] = array(
    '#type' => 'fieldset',
    '#title' => t('SKU Relationships'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['sku']['intro'] = array(
    '#markup' => t('This is a very imporant area of this module.  If you have questions please refer to your system adminstrator before proceeding.'),
    '#weight' => 0,
  );
  $form['sku']['corporate_disk_company_base_sku_list'] = array(
    '#title' => t('Baseline SKU List'),
    '#type' => 'textarea',
    '#description' => t('This is a Key Value pair list of the baseline product SKUs.  The keys are integer values. The values are the SKU.  These values must match what is stored in Disk.com.  <br />An example would be: 07|106-2'),
    '#default_value' => variable_get('corporate_disk_company_base_sku_list', ''),
    '#rows' => 10,
    '#cols' => 60,
    '#weight' => 2,
    '#resizable' => TRUE,
  );
  $form['sku']['substitution'] = array(
    '#type' => 'fieldset',
    '#title' => t('SKU Substitutions'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sku']['substitution']['intro'] = array(
    '#markup' => t('<strong>PROCEED WITH CAUTION</strong><br />These are optional fields for doing a direct substitution on SKUs to get to the base SKU.  <strong>This is a global substitution that will apply to all SKUs processed.</strong> For instance we are running -8 that resolve to a -2 for all SKUs.'),
    '#weight' => 0,
  );
  $form['sku']['substitution']['corporate_disk_company_base_sku_substitution_needle'] = array(
    '#title' => t('Substitution Search'),
    '#type' => 'textfield',
    '#description' => t('If we are running a substitution suffix.  What is the suffix we are looking for.  The needle in a string search'),
    '#default_value' => variable_get('corporate_disk_company_base_sku_substitution_needle', ''),
    '#size' => 10,
    '#resizable' => TRUE,
  );
  $form['sku']['substitution']['corporate_disk_company_base_sku_substitution_haystack'] = array(
    '#title' => t('Substitution Replacement'),
    '#type' => 'textfield',
    '#description' => t('What is our substitution.  <strong>This must match the patterns in the <i>Baseline SKU List</i> above or you will break this module!</strong>'),
    '#default_value' => variable_get('corporate_disk_company_base_sku_substitution_haystack', ''),
    '#size' => 10,
    '#resizable' => TRUE,
  );
  $form['sku']['corporate_disk_company_bundle_sku_list'] = array(
    '#title' => t('Bundled SKU List'),
    '#type' => 'textarea',
    '#description' => t('<b>These are processed before substitutions and hence you must manual translate the substitution here!</b><br />This is a Key Value pair list of the <b>BUNDLED</b> product SKUs.  The keys are bundle SKU. The values are the base SKUs the bundle represents delimitered by spaces.  <b>The values must match the baseline SKUs that are stored on Disk.com listed above!</b>  An example would be: <br /> 502-2|106-2 200-2'),
    '#default_value' => variable_get('corporate_disk_company_bundle_sku_list', ''),
    '#rows' => 10,
    '#cols' => 60,
    '#weight' => 10,
    '#resizable' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Form definition; admin settings shipping services.
 */
function corporate_disk_admin_settings_shipping($form, &$form_state) {
  $form = array();
  $form['corporate_disk_company_shipping_services'] = array(
    '#title' => t('Available shipping methods'),
    '#type' => 'textarea',
    '#description' => t('<b>This is list of the available shipping methods offered at Disk.com as they exist in their API.</b><br /><b>These must match the Disk.com API names exactly</b><br />One per line.'),
    '#default_value' => variable_get('corporate_disk_company_shipping_services', ''),
    '#rows' => 30,
    '#cols' => 40,
    '#weight' => 10,
  );
  return system_settings_form($form);
}

/**
 * Form definition; admin settings shipping services configuration.
 */
function corporate_disk_admin_settings_shipping_configuration($form, &$form_state) {
  $commerce_shipping_services = commerce_shipping_services();
  $values = variable_get('corporate_disk_company_shipping_services');
  $form = array();
  if ($values == '') {
    $shipping_services = array('EMPTY LIST');
    $form['corporate_disk_company_shipping_services_notice'] = array(
      '#markup' => t('You have not added any') . ' ' . l('shipping services', 'admin/config/services/corporatedisk/shippingservices') . ' ' . t('for Corporate Disk Company.'),
      '#weight' => 0,
    );
    return system_settings_form($form);
  }
  else {
    $shipping_services = list_extract_allowed_values(variable_get('corporate_disk_company_shipping_services', "EMPTY LIST\nend"), 'list_text', FALSE);
  }
  $form['corporate_disk_company_shipping_services_intro'] = array(
    '#markup' => t('Setup default shipping services and match available Disk.com services to Drupal Commerce shipping services.'),
  );
  $form['corporate_disk_company_shipping_default_conus'] = array(
    '#title' => t('Default Continental United States Shipping'),
    '#description' => t('Shipping method to use when none is specified as a line item in the order. Continental United States.'),
    '#default_value' => variable_get('corporate_disk_company_shipping_default_conus', ''),
    '#type' => 'select',
    '#options' => $shipping_services,
    '#weight' => 0,
  );
  $form['corporate_disk_company_shipping_default_oconus'] = array(
    '#title' => t('Default International Shipping'),
    '#description' => t('Shipping method to use when none is specified as a line item in the order. Outside Continental United States.'),
    '#default_value' => variable_get('corporate_disk_company_shipping_default_oconus', ''),
    '#type' => 'select',
    '#options' => $shipping_services,
    '#weight' => 3,
  );
  if (!empty($commerce_shipping_services)) {
    $form['services'] = array(
      '#type' => 'fieldset',
      '#title' => t('Shipping Services'),
      '#weight' => 5,
      '#description' => t('Match the Drupal Commerce shipping services to the appropriate Disk.com shipping service.')
    );
    foreach ($commerce_shipping_services AS $key => $value) {
      $form['services']['corporate_disk_company_shipping_services_' . $key] = array(
        '#title' => t('Drupal Commerce:') . ' ' . $value['display_title'],
        '#description' => t('Shipping method to use for Commerce Shipping' . ' ' . $value['display_title']),
        '#default_value' => variable_get('corporate_disk_company_shipping_services_' . $key, ''),
        '#type' => 'select',
        '#options' => $shipping_services,
      );
    }
  }
  else {
    $form['corporate_disk_company_shipping_services_notice'] = array(
      '#markup' => t('You have no shipping services defined in Drupal Commerce.  Once you add a shipping service module this area will populate with mappings that must be configured.'),
      '#weight' => 50,
    );
  }
  return system_settings_form($form);
}

/**
 * AJAX callback to ask for testing under live conditions
 */
function corporate_disk_company_live_testing($form, $form_state) {
  return $form['corporate_disk_company_live_test'];
}

/**
 * Page to list failed transactions to Disk.com
 */
function corporate_disk_company_failed_orders() {
  $results = db_select('corporate_disk_company_shipments', 'c')
      ->fields('c')
      ->condition('confirmation', 'DISK', 'NOT LIKE')
      ->execute();
  $count = $results->rowCount();
  if ($count == 0) {
    // Stop processing no issues.
    $page = array(
      'intro' => array(
        '#markup' => t('There are no orders present with any issues.'),
      ),
    );
    return $page;
  }
  else {
    $rows = array();
    foreach ($results as $node) {
      $rows[] = array($node->order_number,
        $node->confirmation,
        $node->created_at,
      );
    }
    $inforows[] = array(
      'DISK-RETRY',
      t('cURL returned a 404 or not found. This is a connection problem.'),
    );
    $inforows[] = array(
      'DISK-FAILURE-cURL-ERROR-NUMBER-X',
      t('Refer to the php cURL Documentation for error number. X is the error number'),
    );
    $page = array(
      'informaton' => array(
        '#theme' => 'table',
        '#header' => array(t('Error'), t('Description')),
        '#rows' => $inforows,
      ),
      'datatable' => array(
        '#theme' => 'table',
        '#header' => array(t('Order Number'), t('Confirmation'), t('Created At')),
        '#rows' => $rows,
        '#empty' => t('No entries. Something went wrong.'),
      ),
    );
    return $page;
  }
}
