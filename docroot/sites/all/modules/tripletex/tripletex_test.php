<?php
/*****************************************************************************************************************************
* TESTING FUNCTIONS
* Available at specific testing page when module is installed.
*
* Will be removed for live module
*/

if (TRIPLETEX_ENABLE_TEST_FUNCTIONS) {
  /**
   * Test form function to test the Tripletex API
   *
   */
  function tripletex_testing($form, &$form_state) {

    $form = array();
    $form['testing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test the API'),
    '#description' => t('Run tests for the Tripletex JSON backend.'),
    );

    // User test data form

    $form['testing']['user-details'] = array(
    "#type" => "fieldset",
    "#title" => t("Test data"),
    "#description" => t("Test data for various API tests."),
    );

    $form['testing']['user-details']['user-name'] = array(
    "#type" => "textfield",
    "#title" => "Name of test customer",
    '#size' => 20,
    '#maxlength' => 80,
    '#autocomplete_path' => 'invoices/customer/autocomplete',
    );

    $form['testing']['user-details']['user-email'] = array(
    "#type" => "textfield",
    "#title" => "Email of test customer",
    '#size' => 20,
    '#maxlength' => 80,
    );

    $form['testing']['user-details']['product-price'] = array(
    "#type" => "textfield",
    "#title" => "Product unit price",
    '#size' => 20,
    '#maxlength' => 80,
    //  '#autocomplete_path' => 'invoices/prices/autocomplete',
    );

    $form['testing']['user-details']['product-description'] = array(
    "#type" => "textfield",
    "#title" => "Product description",
    '#size' => 20,
    '#maxlength' => 80,
    '#autocomplete_path' => 'invoices/products/autocomplete',
    );

    $form['testing']['user-details']['product-count'] = array(
    "#type" => "textfield",
    "#title" => "Number of products",
    '#size' => 20,
    '#maxlength' => 80,
    );

    // Buttons

    $form['testing']['order'] = array(
    "#type" => "submit",
    "#submit" => array("tripletex_order_test"),
    "#value" => t("Submit order"),
    );

    $form['testing']['customer'] = array(
    "#type" => "submit",
    "#submit" => array("tripletex_customer_test"),
    "#value" => t("Save customer"),
    );

    $form['testing']['tripletex_check_payment'] = array(
    "#type" => "submit",
    "#submit" => array("tripletex_check_payment"),
    "#value" => t("Check invoice status"),
    );

    $form['testing']['tripletex_switch_company'] = array(
    "#type" => "submit",
    "#submit" => array("tripletex_switch_company"),
    "#value" => t("Switch company"),
    );

    $form['testing']['logout'] = array(
    "#type" => "submit",
    "#submit" => array("tripletex_logout_test"),
    "#value" => t("Logout Tripletex"),
    );
    $form['testing']['search'] = array(
    "#type" => "submit",
    "#submit" => array("tripletex_search_test"),
    "#value" => t("Search customer"),
    );
    $form['testing']['check'] = array(
    "#type" => "submit",
    "#submit" => array("tripletex_check_payment"),
    "#value" => t("Check payment"),
    );
    return $form;

  }

  /**
   * @todo Document this function.
   */
  function tripletex_order_test() {
    $session = tripletex_login();
    $result = tripletex_send_order($session);
    //dpm($result);
  }

  /**
   * @todo Document this function.
   */
  function tripletex_search_test($form, &$form_state) {
// 1481895
    $result = tripletex_search_customer($form_state['values']['user-name']);
    //dpm($result, 'tripletex_search_customer result');
    drupal_set_message("tripletex_search_customer result: <pre>" . print_r($result, true)."</pre>");
    
    $result = tripletex_search_product($form_state['values']['product-description'], TRUE);
    //dpm($result, 'tripletex_search_product result');
    drupal_set_message("tripletex_search_product result: <pre>" . print_r($result, true)."</pre>");
    foreach ($result as $prod) {
//      tripletex_commerce_product_sync($prod);
    }
  }

  /**
   * @todo Document this function.
   */
  function tripletex_customer_test($form, &$form_state) {
    // $session = tripletex_login();
    $customer = array(
    'name' => $form_state['values']['user-name'], // Required
    'email' => $form_state['values']['user-email'],
    'invoiceEmail' => $form_state['values']['user-email'],
    );
    $result = tripletex_create_update_customer($customer);
    $login_result = tripletex_login();
    $result = tripletex_save_customer($result, $login_result);
    
    $customers = tripletex_search_customer($customer['name'], true);
    
    drupal_set_message("Customers saved with Tripletex: <pre>" . print_r($customers, true)."</pre>");
    
    $customer = tripletex_create_customer(array_shift($customers));

    drupal_set_message("New Customer: <pre>" . print_r($customer, true)."</pre>");
    
    //dpm($result);
  }

  /**
   * @todo Document this function.
   */
  function tripletex_logout_test() {
    $session = tripletex_login();
    $result = tripletex_logout($session);
    //dpm($result);
  }

  /**
   * Callback function to run the tests when button in form above is hit.
   *
   */
  function tripletex_run_tests() {

    $session = tripletex_login();

    tripletex_send_invoice($session);

    tripletex_logout($session);

  }

  /**
   * @todo Document this function.
   */
  function tripletex_send_invoice($session, $form_state) {

    $customer = array(
    'Customer-name'    => $form_state['values']['user-name'],
    'Customer-email'   => $form_state['values']['user-email'],
    'Unit-price'       => $form_state['values']['product-price'], // Required unit price of item.
    'Order-line-description' => $form_state['values']['product-description'], //  Required item description
    'Count'            => $form_state['values']['product-count'], // Opional number of items. Defaults to 1
    );

    $invoice = tripletex_create_invoice($customer);

    $output = implode(';', $invoice) . ";\r\n";
    //  dpm($output, 'tripletex_send_invoice output');

    $result = tripletex_api_request('Invoice.importInvoicesTripletexCSV', $output, $session);
    _tripletex_add_invoice_log($invoice);

    //dvm($result, 'tripletex_send_invoice result');

  }

  /**
   * @todo Document this function.
   */
  function tripletex_send_order($session) {

    $customer = array(
    'Customer-name' => 'Sten Johnsen',
    //  'Customer-number'   => 10002, //480331,
    'Customer-email' => 'sten@thinea.no',
    'Unit-price' => 250, // Required unit price of item.
    'Order-line-description' => 'Produktbeskrivelse ordre', //  Required item description
    'Count' => 2, // Opional number of items. Defaults to 1
    );

    $order = tripletex_create_order($customer);

    $output = implode(';', $order) . ";\r\n";
    //  dpm($output, 'tripletex_send_invoice output');

    $result = tripletex_api_request('Project.importOrdersTripletexCSV', $output, $session);

    ///dvm($result, 'tripletex_send_order result');

  }

  /**
   * @todo Document this function.
   */
  function tripletex_login_example() {

    $base_url = $params['path'];
    $data = array(
    'username' => $params['username'],
    'password' => $params['password'],
    );
    $data = http_build_query($data, '', '&');
    $headers = array();
    $options = array(
    'headers' => array('Accept' => 'application/json'),
    'method' => 'POST',
    'data' => $data,
    );

    // See if the dummy API is enabled and active
    $response = module_invoke('tripletex_dummy_api', 'tripletex_api_request_call', $headers, $data);

    if (!count($result)) {
      $response = drupal_http_request($params['path'] . '/user/login', array('headers' => $options));
    }
    $data = json_decode($response->data);

    // Check if login was successful

    if ($response->code == 200) {
      // Now recycle the login cookie we recieved in the first request

      $options['headers']['Cookie'] = $data->session_name . '=' . $data->sessid;

      // Create a new node

      $data = array(
      'node' => array(
        'type' => 'cloudnode',
        'title' => 'node1',
        'field_ip_address' => array('value' => '1.2.3.4'),
      ),
      );
      $options['data'] = http_build_query( $data, '', '&');
      $options['method'] = 'POST';

      // See if the dummy API is enabled and active
      $response = module_invoke('tripletex_dummy_api', 'tripletex_api_request_call', $headers, $data);

      if (!count($result)) {
        $response = drupal_http_request($base_url . '/node', array('headers' => $options));
      }
    }
    else {
      die('Failed to login');
    }

  }

  /**
   * @todo Document this function.
   */
  function tripletex_switch_company() {

    $session = tripletex_login();

    // if ($session) {

    // If login is OK
    $companyId = variable_get('tripletext_company_id', 0);
    $result = tripletex_api_request('Sync.switchCompany', array($companyId), $session);
    // }

    //dvm($result, 'tripletex_switch_company result');

  }

}
/**********************************************************************************************************************************
 * END TEST FUNCTIONS
*
**********************************************************************************************************************************/
