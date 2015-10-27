<?php

/**
 * @file Contains export and pagae callbacks for the Commerce WinBIZ module.
 */

/**
 * 	Function provides menu callback that is used by WinBIZ to interact with Drupal.
 *  Executed action and passed arguments depend on set variables in $_GET.
 *  @todo Product import function that creates product within drupal commerce.
 *	  case 'import_products':
 * 	    break;
 * 		/*
 *		Possible Action:
 * 		- import products/taxes/categories/manufacturers/specials from the file ***
 * 		- update products price/quantity only
 * 		- purge products/taxes/categories/manufacturers/specials
 * 		*
 * 		case 'import_products':
 * 		case 'import_taxes':
 * 		case 'import_categories':
 * 		case 'import_manufacturers':
 * 		case 'import_specials':
 * 		case 'update_products_quantity':
 * 		case 'update_products_price':
 * 		case 'purge_products':
 * 		case 'purge_taxes':
 * 		case 'purge_categories':
 * 		case 'purge_specials':
 * 		case 'purge_specials_from_products':
 * 		case 'purge_manufacturers':
 * 		case 'import_products_categories':
 * 			$result = winbiz_load_and_do($file, $action);
 * 			echo($result);
 * 			break;
 */
function commerce_winbiz_command_router() {
  if (module_exists('devel') && variable_get('commerce_winbiz_debug', FALSE)) {
    dd($_GET, '$_GET ARGUMENTS');
    dd($_SERVER['REMOTE_ADDR'], 'REQUESTER');
    dd(date('H:i:s'), 'REQUEST TIME');
    dd($_POST, 'POST ARGS');
    dd($_SERVER, '$_SERVER');
  }

  // Parse all accepted parameters.
  $action = isset($_GET['action']) ? $_GET['action'] : '';
  $from = isset($_GET['oIDfrom']) ? $_GET['oIDfrom'] : 0;
  $to = isset($_GET['oIDto']) ? $_GET['oIDto'] : 99999999;

  if (!empty($action)) {
    switch ($action) {
      case 'export_orders':
        // Send headers to oupput in CSV format if debug mode is off.
        if (!variable_get('commerce_winbiz_debug', FALSE)) {
          header('Content-type: text/plain');
          header('Content-Disposition: attachment; filename="Bizexdoc--' . $from . '--' . $to . '.csv"');
        }
        // Export all orders (from given oIDfrom to oIDto).
        $result = commerce_winbiz_export_orders($from, $to);
        echo($result);
        break;
      case 'get_default_currency':
        // Get the default currency of Drupal commerce.
        $currency = commerce_default_currency();
        if (!empty($currency)) {
          echo(WINBIZ_SUCCESS . $currency);
        }
        else {
          echo(WINBIZ_FAILURE . "Default currency not found!" . WINBIZ_EOLINE);
        }
        break;
      default:
        echo(WINBIZ_FAILURE . 'Unknown action: ' . $action . WINBIZ_EOLINE);
        break;
    }
  }
  else {
    echo(WINBIZ_FAILURE . 'Missing parameter: action' . WINBIZ_EOLINE);
  }

  drupal_exit();
}

/**
 * Provide form for some global settings that cannot be retrieved
 * from the Drupal Commerce settings.
 */
function commerce_winbiz_admin_settings_form($form, &$form_state) {
  
  $form['accounts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Accounts codes'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['accounts']['commerce_winbiz_profit_account_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Profit account number'),
    '#description' => t('Accountnumber used for profit booking in WinBIZ.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('commerce_winbiz_profit_account_number', ''),
  );

  $form['accounts']['commerce_winbiz_vattax_account_number'] = array(
    '#type' => 'textfield',
    '#title' => t('VAT account number'),
    '#description' => t('Accountnumber used for VAT booking in WinBIZ.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('commerce_winbiz_vattax_account_number', ''),
  );
  
  $form['accounts']['commerce_winbiz_debit_account_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Debit account number'),
    '#description' => t('Accountnumber used for debit booking.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('commerce_winbiz_debit_account_number', ''),
  );

  $form['tax'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tax configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if ($commerce_taxes = commerce_winbiz_commerce_taxes_available()) {
    // Get configured Commerce taxes.
    foreach ($commerce_taxes as $name => $tax) {
      $tax_rate = number_format($tax['rate'] * 100, 2);
      $commerce_taxes_available[$tax_rate] = $tax['display_title'];
    }

    $form['tax']['commerce_winbiz_use_commerce_tax'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use configured tax rate of Drupal Commerce'),
      '#size' => 10,
      '#maxlength' => 10,
      '#default_value' => variable_get('commerce_winbiz_use_commerce_tax', FALSE),
    );
  }

  $form['tax']['commerce_winbiz_custom_tax_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom tax factor for invoices'),
    '#description' => t('Enter as percentage value, e.g. 8.00 %'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('commerce_winbiz_custom_tax_rate', 0.00),
  );
  if (commerce_winbiz_commerce_taxes_available()) {
    $form['tax']['commerce_winbiz_custom_tax_rate']['#states'] = array(
      'visible' => array(':input[name="commerce_winbiz_use_commerce_tax"]' => array('checked' => FALSE)),
    );
  }

  // Flag for Incl./Excl. tax rate.
  $form['tax']['commerce_winbiz_tax_inclusion'] = array(
    '#type' => 'select',
    '#title' => t('Tax rate inclusion'),
    '#description' => t('This option will tell WinBIZ wether or not VAT tax rates are included in the exported price.'),
    '#options' => array(
      1 => t('Inclusive'),
      2 => t('Exclusive'),
    ),
    '#default_value' => variable_get('commerce_winbiz_tax_inclusion', 1),
  );

  $form['commerce_winbiz_article_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update articles'),
    '#description' => t('Check this if you want to automatically create and update your articles in WinBIZ upon imports.'),
    '#default_value' => variable_get('commerce_winbiz_article_update', 1),
  );

  $form['commerce_winbiz_invoice_layout'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice layout code'),
    '#description' => t('Code of default layout for invoices of all exported orders in the WinBIZ application.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('commerce_winbiz_invoice_layout', ''),
  );

  // Callback URL token.
  $form['commerce_winbiz_authstr'] = array(
    '#type' => 'textfield',
    '#title' => t('Authentication token'),
    '#description' => t('This token will be used to protect the WinBIZ callback from unauthorized access.'),
    '#default_value' => variable_get('commerce_winbiz_authstr', ''),
  );

  return system_settings_form($form);
}

/**
 * Provides mapping table for payment method settings.
 */
function commerce_winbiz_admin_payment_settings_form($form, &$form_state) {
  // Get the payment methods
  $methods = commerce_payment_methods();
  // Build the table header.
  $header = array(
    'payment_method' => t('Payment method'),
    'method_code' => t('Payment method code'),
    'method_enabled' => t('Online payment enabled'),
  );

  $rows = array();
  // Loop over every payment method, add the necessary fields
  // to the form and build the base for the row.
  foreach ($methods as $methodid => $method) {
    $row = array();
    $form['fields']['commerce_winbiz_payment_method_code_' . $methodid] = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#maxlength' => 30,
      '#default_value' => variable_get('commerce_winbiz_payment_method_code_' . $methodid, ''),
    );
    $form['fields']['commerce_winbiz_payment_method_enabled_' . $methodid] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('commerce_winbiz_payment_method_enabled_' . $methodid, ''),
    );
    $row = array(
      'payment_method' => $method['title'],
      'method_code' => '',
      'method_enabled' => '',
    );
    $rows[$methodid] = $row;
  }

  $form['info'] = array(
    '#type' => 'item',
    '#markup' => '<div class="wb-payment-settings-info">' . t('If your webshop features online payment methods like credit card or paypal payment,
      you will have to enable those payment methods here, in order to make sure your orders are imported as already paid.
      It is recommended to create the payment method first and add a code, or to look up the code of the corresponding payment method in WinBIZ
      and then add the payment method code here afterwards.') .
      '</div><br />',
  );

  // Add the table element.
  $form['table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
    '#weight' => 1,
  );

  // Special theming in order to display the fields in a table.
  $form['#theme'] = 'commerce_winbiz_payment_mapping_table';

  return system_settings_form($form);
}
