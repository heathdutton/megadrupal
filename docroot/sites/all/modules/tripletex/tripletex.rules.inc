<?php

/**
 * @file
 * Actions, Rules and Events integration with Tripletex API
 */

/**
 * Implements hook_rules_event_info().
 *
 * Register events available
 *
 * @ingroup rules
 */
function tripletex_rules_event_info() {
  return array(
    'tripletex_invoice_paid' => array(
      'label' => t('An invoice is updated with paid status'),
      'group' => t('Tripletex'),
      'module' => 'Tripletex',
      'arguments' => array(
        'invoice' => array(
          'type' => 'invoice',
          'label' => t('Invoice object.'),
        ),
      ),
    ),
    'tripletex_invoice_updated' => array(
      'label' => t('An invoice is updated'),
      'group' => t('Tripletex'),
      'module' => 'Tripletex',
      'arguments' => array(
        'invoice' => array(
          'type' => 'invoice',
          'label' => t('Invoice object.'),
        ),
      ),
    ),
    'tripletex_invoice_overdue' => array(
      'label' => t('An invoice is marked as overdue'),
      'group' => t('Tripletex'),
      'module' => 'Tripletex',
      'arguments' => array(
        'invoice' => array(
          'type' => 'invoice',
          'label' => t('Invoice object.'),
        ),
      ),
    ),
  );
}


/**
 * Implements hook_rules_action_info().
 *
 * @ingroup rules
 */
function tripletex_rules_action_info() {
  return array(
    'tripletex_action_create_invoice' => array(
      'label' => t('Create an invoice for a registered user and send to Tripletex'),
      'group' => t('Tripletex'),
      'arguments' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Page of origin'),
        ),
        'uid' => array(
          'type' => 'user',
          'label' => t('User, which is customer'),
        ),
        'count' => array(
          'type' => 'number',
          'label' => t('Number of items'),
        ),
        'product' => array(
          'type' => 'number',
          'label' => t('Product description.'),
        ),
        'price' => array(
          'type' => 'number',
          'label' => t('Product price.'),
        ),
      ),
      'module' => 'tripletex',
    ),
    'tripletex_action_create_invoice_anonymous' => array(
      'label' => t('Create an invoice for anonymous user and send to Tripletex'),
      'group' => t('Tripletex'),
      'arguments' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Page of origin'),
        ),
        'name' => array(
          'type' => 'string',
          'label' => t('Name of customer'),
        ),
        'email' => array(
          'type' => 'string',
          'label' => t('Customer e-mail'),
        ),
        'count' => array(
          'type' => 'number',
          'label' => t('Number of items'),
        ),
        'product' => array(
          'type' => 'number',
          'label' => t('Product description'),
        ),
        'price' => array(
          'type' => 'number',
          'label' => t('Product price'),
        ),
      ),
      'module' => 'tripletex',
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 *
 * Register the conditions available
 *
 * @ingroup rules
 */
function tripletex_rules_condition_info() {

  // conditions we can check
  return array(
    'tripletex_invoice_is_due' => array(
      'label' => t('Invoice is due and not paid'),
      'group' => t('Tripletex'),
      'arguments' => array(
        'invoice' => array(
          'type' => 'invoice',
          'label' => t('Invoice'),
        ),
      ),
      'parameters' => array(
        'tripletex_invoice' => array(
      	  'type' => 'tripletex_invoice',
          'label' => t('Invoice'),
      	  'description' => t('The invoice whose line items should be checked for the specified product. If the specified invoice does not exist, the comparison will act as if it is against a quantity of 0.'),
      	),
      	'product_id' => array(
      	  'type' => 'text',
      	  'label' => t('Product SKU'),
      	  'description' => t('The SKU of the product to look for on the invoice.'),
      	),
      	'operator' => array(
      	  'type' => 'text',
      	  'label' => t('Operator'),
      	  'description' => t('The operator used with the quantity value below to compare the quantity of the specified product on the invoice.'),
      	  'default value' => '>=',
      	  'options list' => 'tripletex_numeric_comparison_operator_options_list',
      	  'restriction' => 'input',
      	),
      	'value' => array(
      	  'type' => 'text',
      	  'label' => t('Quantity'),
      	  'default value' => '1',
      	  'description' => t('The value to compare against the quantity of the specified product on the invoice.'),
      	),
      		
        ),		
      'module' => 'tripletex',
    ),
    'tripletex_invoice_is_paid' => array(
      'label' => t('Invoice is paid'),
      'group' => t('Tripletex'),
      'arguments' => array(
        'invoice' => array(
          'type' => 'invoice',
          'label' => t('Invoice'),
        ),
      ),
      'module' => 'tripletex',
    ),
  );
}

/**
 * Implements hook_rules_data_info().
 */
function rules_tripletex_data_info() {
  return array(
    'invoice' => array(
      'label' => t('Invoice'),
      'wrap' => TRUE,
      'group' => t('Tripletex'),
    ),
  );
}

/**
 * Returns an array of numerical comparison operators for use in Rules.
 */
function tripletex_numeric_comparison_operator_options_list() {
  return drupal_map_assoc(array('<', '<=', '=', '>=', '>'));
}

/**
 * Implements hook_rules_data_type_info().
 *
 * Defines the Invoice data type
 *
 */
// TODO: Update data types to D7
/* function tripletex_rules_data_type_info() {

  return array(
    'ivoice' => array(
      'label' => t('Invoice'),
      'class' => 'rules_data_type_invoice',
      'savable' => TRUE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'tripletex',
    ),
  );
} */

/**
 * Declares the invoice data type handler
 *
 * @author sten
 *
 */
 // TODO: Update date types to D7
/* class rules_data_type_invoice extends rules_data_type {

  function save() {
    // Save an invoice
    $invoice = &$this->get();

    return TRUE;
  }

  function load($iid) {
    // Load an invoice
    return _tripletex_load_invoice($iid);
  }

  function get_identifier() {
    // Get the invoice identifier
    $invoice = &$this->get();
    return $invoice->id;
  }
} */

/**
 * Action to create an invoice for a registered user
 *
 * @param unknown_type $node
 * @param unknown_type $uid
 * @param unknown_type $count
 * @param unknown_type $product
 * @param unknown_type $price
 */
function tripletex_action_create_invoice($node, $uid, $count, $product, $price) {

  $usr = user_load($uid);

  $customer = array(
    'Customer-name' => $usr->name,
    'Customer-email' => $usr->email,
    'Unit-price' => $price, // Required unit price of item.
    'Order-line-description' => $product, //  Required item description
    'Count' => $count, // Opional number of items. Defaults to 1
  );

  $invoice = tripletex_create_invoice($customer);

  $output = implode(';', $invoice) . ";\r\n"; // Create serialised invoice data


  $session = tripletex_login();
  if ($session) {
    // If login is OK

    $result = tripletex_api_request('Invoice.importInvoicesTripletexCSV', $output, $session);

    if ($result == NULL) {
      _tripletex_add_invoice_log($invoice);
      return $customer['Invoice-number'];
    }
  }

  return NULL;

}

/**
 * Action to create an invoice for an anonymous user
 *
 * @param unknown_type $node
 * @param unknown_type $uid
 * @param unknown_type $count
 * @param unknown_type $product
 * @param unknown_type $price
 */
function tripletex_action_create_invoice_anonymous($node, $name, $email, $count, $product, $price) {

  $customer = array(
    'Customer-name' => $name,
    'Customer-email' => $email,
    'Unit-price' => $price, // Required unit price of item.
    'Order-line-description' => $product, //  Required item description
    'Count' => $count, // Opional number of items. Defaults to 1
  );

  $invoice = tripletex_create_invoice($customer);

  $output = implode(';', $invoice) . ";\r\n"; // Create serialised invoice data

  $session = tripletex_login();
  if ($session) {
    // If login is OK
    $result = tripletex_api_request('Invoice.importInvoicesTripletexCSV', $output, $session);
    if ($result == NULL) {
      _tripletex_add_invoice_log($invoice);
      return $customer['Invoice-number'];
    }
  }

  return NULL;

}

/**
 * Check if Invoice is due
 *
 * @param object $invoice
 */
function tripletex_invoice_is_due($invoice) {
  if ($invoice->due_date < mktime() && $invoice->remaining_amount) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Check if Invoice is paid
 *
 * @param object $invoice
 */
function tripletex_invoice_is_paid($invoice) {

  if ($invoice->remaining_amount == 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }

}
