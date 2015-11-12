<?php

/**
 * @file
 * FedEx Shipping Quotes module administration menu and callbacks.
 *
 * @author Tim Rohaly.    <http://drupal.org/user/202830>
 */


/**
 * Default FedEx Web Services API settings.
 *
 * Records FedEx account information neccessary to use service. Allows testing
 * or production mode. Configures which FedEx services are quoted to customers.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 *
 * @see uc_fedex_admin_settings_validate()
 * @ingroup forms
 */
function uc_fedex_admin_settings() {

  // Label printing routines
  module_load_include('inc', 'uc_fedex', 'uc_fedex.ship');

  // Container for credentials forms
  $form['uc_fedex_credentials'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Credentials'),
    '#description'   => t('Account number and authorization information.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  // Form to set the developer key
  $form['uc_fedex_credentials']['uc_fedex_user_credential_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Web Services API User Key'),
    '#default_value' => variable_get('uc_fedex_user_credential_key', ''),
    '#required'      => TRUE,
  );

  // Form to set the developer password
  $form['uc_fedex_credentials']['uc_fedex_user_credential_password'] = array(
    '#type'          => 'password',
    '#title'         => t('FedEx Web Services API Password'),
    '#default_value' => variable_get('uc_fedex_user_credential_password', ''),
  );

  // Form to set user account number
  $form['uc_fedex_credentials']['uc_fedex_account_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Account #'),
    '#default_value' => variable_get('uc_fedex_account_number', 0),
    '#required'      => TRUE,
  );

  // Form to set user meter number
  $form['uc_fedex_credentials']['uc_fedex_meter_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Meter #'),
    '#default_value' => variable_get('uc_fedex_meter_number', 0),
    '#required'      => TRUE,
  );

  // Form to set choose between Production and Testing server
  // ***Defaults to Testing!***
  $form['uc_fedex_credentials']['uc_fedex_server_role'] = array(
    '#type'          => 'select',
    '#title'         => t('FedEx Server Role'),
    '#description'   => t('Use the Testing server while developing and configuring your site.  Switch to the Production server only after you have demostrated that transactions on the Testing server are working and you are ready to go live.'),
    '#options'       => array(
      'testing'      => t('Testing'),
      'production'   => t('Production'),
    ),
    '#default_value' => variable_get('uc_fedex_server_role', 'testing'),
  );

  // Container for service selection forms
  $form['uc_fedex_service_selection'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Service Options'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  // Form to restrict FedEx Ground services available to customer
  $form['uc_fedex_service_selection']['uc_fedex_ground_services'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('FedEx Ground Services'),
    '#default_value' => variable_get('uc_fedex_ground_services', _uc_fedex_ground_services()),
    '#options'       => _uc_fedex_ground_services(),
    '#description'   => t('Select the FedEx Ground services customers are allowed to use.'),
  );

  if (module_exists('rules')) {
    $form['uc_fedex_service_selection']['uc_fedex_ground_conditions'] = array(
      '#type' => 'fieldset',
      '#title' => t('FedEx Ground service conditions'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $conditions = rules_config_load('get_quote_from_fedex_ground');
    if ($conditions) {
      $conditions->form($form['uc_fedex_service_selection']['uc_fedex_ground_conditions'], $form_state);
    }
  }


  // Form to restrict FedEx Express services available to customer
  $form['uc_fedex_service_selection']['uc_fedex_express_services'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('FedEx Express Services'),
    '#default_value' => variable_get('uc_fedex_express_services', _uc_fedex_express_services()),
    '#options'       => _uc_fedex_express_services(),
    '#description'   => t('Select the FedEx Express services customers are allowed to use.'),
  );

  if (module_exists('rules')) {
    $form['uc_fedex_service_selection']['uc_fedex_express_conditions'] = array(
      '#type' => 'fieldset',
      '#title' => t('FedEx Express service conditions'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $conditions = rules_config_load('get_quote_from_fedex');
    if ($conditions) {
      $conditions->form($form['uc_fedex_service_selection']['uc_fedex_express_conditions'], $form_state);
    }
  }

  // Form to restrict FedEx Express Freight services available to customer
  $form['uc_fedex_service_selection']['uc_fedex_freight_services'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('FedEx Express Freight Services'),
    '#default_value' => variable_get('uc_fedex_freight_services', _uc_fedex_freight_services()),
    '#options'       => _uc_fedex_freight_services(),
    '#description'   => t('Select the FedEx Express Freight services customers are allowed to use.'),
  );

  if (module_exists('rules')) {
    $form['uc_fedex_service_selection']['uc_fedex_express_freight_conditions'] = array(
      '#type' => 'fieldset',
      '#title' => t('FedEx Express Freight service conditions'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $conditions = rules_config_load('get_quote_from_fedex');
    if ($conditions) {
      $conditions->form($form['uc_fedex_service_selection']['uc_fedex_express_freight_conditions'], $form_state);
    }
  }

  // Form to restrict FedEx Freight services available to customer
  $form['uc_fedex_service_selection']['uc_fedex_freight'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('FedEx Freight Services'),
    '#default_value' => variable_get('uc_fedex_freight_services', _uc_fedex_freight_services()),
    '#options'       => _uc_fedex_freight_services(),
    '#description'   => t('Select the FedEx Freight services customers are allowed to use.'),
  );

  if (module_exists('rules')) {
    $form['uc_fedex_service_selection']['uc_fedex_freight_conditions'] = array(
      '#type' => 'fieldset',
      '#title' => t('FedEx Freight service conditions'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $conditions = rules_config_load('get_quote_from_fedex_freight');
    if ($conditions) {
      $conditions->form($form['uc_fedex_service_selection']['uc_fedex_freight_conditions'], $form_state);
    }
  }

  // Form to set freight account number
  $form['uc_fedex_credentials']['uc_fedex_freight_account_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Freight Account #'),
    '#default_value' => variable_get('uc_fedex_freight_account_number', 0),
    '#required'      => FALSE,
  );

  // Container for quote options forms
  $form['uc_fedex_quote_options'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Quote Options'),
    '#description'   => t('Preferences that affect computation of quote.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  // Form to set choose between LIST quotes and ACCOUNT quotes
  // ***Defaults to LIST!***
  $form['uc_fedex_quote_options']['uc_fedex_quote_type'] = array(
    '#type'          => 'select',
    '#title'         => t('FedEx Quote Type'),
    '#description'   => t('Choose to present the customer with FedEx list prices or your discounted FedEx account prices. LIST prices only exist for US shipments - if you specify LIST for international shipments you will not receive any quotes.  Note that ACCOUNT prices are accurate only on the PRODUCTION server!'),
    '#options'       => array(
      'list'    => t('List Prices'),
      'account' => t('Discount Account Prices'),
    ),
    '#default_value' => variable_get('uc_fedex_quote_type', 'list'),
  );

  // Form to set how the package is handed over to FedEx
  $form['uc_fedex_quote_options']['uc_fedex_dropoff_type'] = array(
    '#type'          => 'select',
    '#title'         => t('FedEx Pickup/Dropoff Options'),
    '#default_value' => variable_get('uc_fedex_dropoff_type', _uc_fedex_dropoff_types()),
    '#options'       => _uc_fedex_dropoff_types(),
    '#description'   => t('Pickup/Dropoff options.  It is assumed that all your packages are using the same method.'),
  );

  // Form to select FedEx packaging to use
  $form['uc_fedex_quote_options']['uc_fedex_package_type'] = array(
    '#type'          => 'select',
    '#title'         => t('FedEx Package Type'),
    '#default_value' => variable_get('uc_fedex_package_type', _uc_fedex_package_types()),
    '#options'       => _uc_fedex_package_types(),
    '#description'   => t('Package Type.  It is assumed that all your packages are using the same packaging.'),
  );

  // Form to select packaging type
  $form['uc_fedex_quote_options']['uc_fedex_all_in_one'] = array(
    '#type'          => 'radios',
    '#title'         => t('Number of Packages'),
    '#default_value' => variable_get('uc_fedex_all_in_one', 1),
    '#options' => array(
      0 => t('Each product in its own package'),
      1 => t('All products in one package'),
    ),
    '#description'   => t('Indicate whether each product is quoted as shipping separately or all in one package.'),
  );

  // Form to add optional Insurance coverage in the amount of the order
  $form['uc_fedex_quote_options']['uc_fedex_insurance'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add Insurance to shipping quote'),
    '#default_value' => variable_get('uc_fedex_insurance', FALSE),
    '#description'   => t('When enabled, products are insured for their full value.'),
  );

  // Container for markup forms
  $form['uc_fedex_markups'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Markups'),
    '#description'   => t('Modifiers to the shipping weight and quoted rate.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  // Form to select type of rate markup
  $form['uc_fedex_markups']['uc_fedex_rate_markup_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Rate Markup Type'),
    '#default_value' => variable_get('uc_fedex_rate_markup_type', 'percentage'),
    '#options'       => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'currency'   => t('Addition (!currency)', array('!currency' => variable_get('uc_currency_sign', '$'))),
    ),
  );

  // Form to select rate markup amount
  $form['uc_fedex_markups']['uc_fedex_rate_markup'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Shipping Rate Markup'),
    '#default_value' => variable_get('uc_fedex_rate_markup', '0'),
    '#description'   => t('Markup FedEx shipping rate quote by dollar amount, percentage, or multiplier.'),
  );

  // Form to select type of weight markup
  $form['uc_fedex_markups']['uc_fedex_weight_markup_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Weight Markup Type'),
    '#default_value' => variable_get('uc_fedex_weight_markup_type', 'percentage'),
    '#options'       => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'mass'       => t('Addition (!mass)', array('!mass' => '#')),
    ),
  );

  // Form to select weight markup amount
  $form['uc_fedex_markups']['uc_fedex_weight_markup'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Shipping Weight Markup'),
    '#default_value' => variable_get('uc_fedex_weight_markup', '0'),
    '#description'   => t('Markup FedEx shipping weight on a per-package basis before quote, by weight amount, percentage, or multiplier.'),
  );

  // Container for validation forms
  $form['uc_fedex_validation'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Address Validation'),
    '#description'   => t('Preferences for FedEx Address Validation.  Additional permissions from FedEx are required to use this feature.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  // Form to select Residential/Commercial destination address
  $form['uc_fedex_validation']['uc_fedex_address_validation'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Let FedEx determine if an address is Commercial or Residential'),
    '#default_value' => variable_get('uc_fedex_address_validation', FALSE),
    '#description'   => t('When enabled, FedEx Address Validation Web Service will be used to determine if a shipping address is Commercial or Residential.'),
  );

  // Form to select Residential/Commercial destination address
  $form['uc_fedex_validation']['uc_fedex_residential_quotes'] = array(
    '#type'          => 'radios',
    '#title'         => t('Quote rates assuming destination is a'),
    '#default_value' => variable_get('uc_fedex_residential_quotes', 1),
    '#options'       => array(
      0 => t('Commercial address'),
      1 => t('Residential address (extra fees)'),
    ),
    '#description'   => t('This selection will be used if Address Validation is disabled above, or if Address Validation is enabled and fails or times out.'),
  );

  // Form to select Residential/Commercial destination address
  $form['uc_fedex_validation']['uc_fedex_checkout_validation'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable checkout address validation'),
    '#default_value' => variable_get('uc_fedex_checkout_validation', FALSE),
    '#description'   => t('When enabled, JavaScript will be used to validate the shipping address entered by the customer on the checkout page.  Errors will be flagged immediately, before the order is submitted.'),
    '#disabled'      => TRUE,
  );

  // Container for label printing
  $form['uc_fedex_labels'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Label Printing'),
    '#description'   => t('Preferences for FedEx Shipping Label Printing.  Additional permissions from FedEx are required to use this feature.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  // Form to select label image format
  $form['uc_fedex_labels']['uc_fedex_label_image_format'] = array(
    '#type'          => 'select',
    '#title'         => t('Select image format for shipping labels'),
    '#default_value' => variable_get('uc_fedex_label_image_format', 'PDF'),
    '#options'       => _uc_fedex_label_image_types(),
  );

  // Form to select label stock
  $form['uc_fedex_labels']['uc_fedex_label_stock'] = array(
    '#type'          => 'select',
    '#title'         => t('Select Laser or Thermal label stock size'),
    '#default_value' => variable_get('uc_fedex_label_stock', 'PAPER_7X4.75'),
    '#options'       => _uc_fedex_label_stock_types(),
  );

  // Form to select label orientation
  $form['uc_fedex_labels']['uc_fedex_label_orientation'] = array(
    '#type'          => 'select',
    '#title'         => t('Select orientation for label printing'),
    '#default_value' => variable_get('uc_fedex_label_orientation', _uc_fedex_label_orientation_types()),
    '#options'       => _uc_fedex_label_orientation_types(),
  );

  $period = drupal_map_assoc(array(86400, 302400, 604800, 1209600, 2419200, 0), 'format_interval');
  $period[0] = t('Forever');

  // Form to select how long labels stay on server
  $form['uc_fedex_labels']['uc_fedex_label_lifetime'] = array(
    '#type'          => 'select',
    '#title'         => t('Label lifetime'),
    '#default_value' => variable_get('uc_fedex_label_lifetime', 604800),
    '#options'       => $period,
    '#description'   => t('Controls how long labels are stored on the server before being automatically deleted. Cron must be enabled for automatic deletion. Default lifetime is 1 week (604800 seconds).'),
  );

  // Register additional validation handler
  $form['#validate'][] = 'uc_fedex_admin_settings_validate';

  return system_settings_form($form);
}


/**
 * Validation handler for uc_fedex_admin_settings form.
 *
 * Requires password only if it hasn't been set.
 *
 * @see uc_fedex_admin_settings()
 */
function uc_fedex_admin_settings_validate($form, &$form_state) {

  $old_password = variable_get('uc_fedex_user_credential_password', '');
  if (!$form_state['values']['uc_fedex_user_credential_password']) {
    if ($old_password) {
      form_set_value($form['uc_fedex_credentials']['uc_fedex_user_credential_password'], $old_password, $form_state);
    }
    else {
      form_set_error('uc_fedex_user_credential_password', t('Password field is required.'));
    }
  }

  if (!is_numeric($form_state['values']['uc_fedex_rate_markup'])) {
    form_set_error('uc_fedex_rate_markup', t('Rate markup must be a numeric value.'));
  }

  if (!is_numeric($form_state['values']['uc_fedex_weight_markup'])) {
    form_set_error('uc_fedex_weight_markup', t('Weight markup must be a numeric value.'));
  }
}
