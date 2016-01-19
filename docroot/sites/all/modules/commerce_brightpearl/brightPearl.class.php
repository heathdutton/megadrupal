<?php

/**
 * @file
 * Class to handle all communication with brightpearl CRM
 *
 * @todo - Use dependency injection to move watchdog calls to the Drupal module
 * or exceptions to handle raise in class and catch in Drupal module.
 */


define('BRIGHTPEARL_BASE_URL', 'brightpearl.com/');
define('BRIGHTPEARL_VERSION', '4.74.2');

// Request type.
define('BRIGHTPEARL_REQUEST_TYPE_RESOURCE_UNKKNOWN', 0);
define('BRIGHTPEARL_REQUEST_TYPE_RESOURCE_GET', 1);
define('BRIGHTPEARL_REQUEST_TYPE_RESOURCE_SEARCH', 2);
define('BRIGHTPEARL_REQUEST_TYPE_RESOURCE_PUT', 3);

class BrightPearl {
  private $signedToken;
  private $datacentre;
  private $accountCode;
  private $channelId;
  private $lastResponse = '';
  private $lastRequestType = BRIGHTPEARL_REQUEST_TYPE_RESOURCE_UNKKNOWN;
  private $lastRequestName = '';
  private $lastResponseErros = array();
  private $reserveStockList = array('products' => array());
  private $retryOn503Count = 3;
  private $logAllRequests = FALSE;
  private $requestCounter = 0;
  private $stockCheckWarehouseId = FALSE;
  public $stockUpdateFunc;


  /***************************************************************************
   *  Getters / setters  & Util functions
   ***************************************************************************/

  /**
   * Return the last received response.
   */
  public function getLastResponse() {
    return $this->lastResponse;
  }

  /**
   * Set how many  times to retry on a 503 response.
   */
  public function setRetryOn503Count($count) {
    $this->retryOn503Count = $count;
  }

  /**
   * Set the signed token.
   */
  public function setSignedToken($signed_token) {
    $this->signedToken = $signed_token;
  }

  /**
   * Set the data centre. Used for building the url.
   */
  public function setDatacentre($datacentre) {
    $this->datacentre = $datacentre;
  }

  /**
   * Set the account code. Used for building the url.
   */
  public function setAccountCode($customer_account_code) {
    $this->accountCode = $customer_account_code;
  }

  /**
   * Set the shop's channel id.
   */
  public function setChannelId($channel_id) {
    $this->channelId = $channel_id;
  }


  /**
   * Set the "log all requests" option.
   */
  public function setLogAllRequests($log_all_requests) {
    $this->logAllRequests = $log_all_requests;
  }

  /**
   * Set to a warehouse id or FALSE for all.
   */
  public function setStockCheckWarehouseId($stock_check_warehouse_id) {
    $this->stockCheckWarehouseId = $stock_check_warehouse_id;
  }

  /**
   * Get the URL.
   */
  public function getUrl($path, $use_version = TRUE) {
    if ($use_version) {
      $version = '2.0.0/';
    }
    else {
      $version = '';
    }
    // Build the URL.
    $url = 'https://ws-' . $this->datacentre . '.' . BRIGHTPEARL_BASE_URL
        . $version . $this->accountCode . '/' . $path;
    return $url;
  }


  /**
   * Check if we should re send data on a 503.
   *
   * If yes we will wait 60sec before returning TRUE.
   * This is to get around the 200 request per minute limit.
   * We are using a counter to avoid going into an infinite loop.
   * To disable this feature set $retry_on503_count to 0.
   */
  protected function resendOn503($path) {
    if ($this->retryOn503Count > 0) {
      $this->retryOn503Count--;
      watchdog('brightpearl', '503 response - sleeping for 60sec. Path = @path', array('@path' => $path), WATCHDOG_ALERT);
      sleep(60);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Log the request's response.
   */
  protected function logRequest($path, &$response) {
    if ($this->logAllRequests) {
      // Keep count of the request.
      $this->requestCounter++;
      $wd_array = array(
        '@counter' => $this->requestCounter,
        '@path' => $path,
        '@code' => $response->code,
      );
      watchdog('brightpearl', 'Request number @counter path = @path returnd a @code code', $wd_array, WATCHDOG_INFO);
    }
  }


  /**
   * Make a get request.
   */
  public function makeGetRequest($path) {
    // Get the url and make the request.
    $url = $this->getUrl($path);
    $options = array(
      'method' => 'GET',
      'timeout' => 30,
      'headers' => array(
        'Content-Type' => 'application/json',
        'brightpearl-dev-ref' => 'bluebag',
        'brightpearl-app-ref' => 'drupal',
        'brightpearl-account-token' => $this->signedToken,
      ),
    );
    $response = drupal_http_request($url, $options);

    // Optional log request.
    $this->logRequest($path, $response);

    // Handle the response.
    if ($response->code != 200) {
      if (($response->code == 503) && ($this->resendOn503($path))) {
        // Make the call again.
        $this->makeGetRequest($path);
      }
      else {
        // Something went wrong.
        watchdog('brightpearl',
                'Get request failed for @path, code: @code error: @error',
                array(
                  '@path' => $path,
                  '@code' => $response->code,
                  '@error' => $response->error,
                ),
                WATCHDOG_ERROR);
        $this->lastResponse = '';
        $json_response = drupal_json_decode($response->data);
        if (isset($json_response['errors'])) {
          $this->lastResponseErros = $json_response['errors'];
        }
        return FALSE;
      }
    }
    else {
      // All good.
      $json_response = drupal_json_decode($response->data);
      $this->lastResponse = $json_response;
      $this->lastResponseErros = '';
      return TRUE;
    }
  }

  /**
   * Make a get request.
   */
  public function makeDeleteRequest($path) {
    // Get the url and make the request.
    $url = $this->getUrl($path);
    $options = array(
      'method' => 'DELETE',
      'timeout' => 30,
      'headers' => array(
        'Content-Type' => 'application/json',
        'brightpearl-dev-ref' => 'bluebag',
        'brightpearl-app-ref' => 'drupal',
        'brightpearl-account-token' => $this->signedToken,
      ),
    );
    $response = drupal_http_request($url, $options);

    // Optional log request.
    $this->logRequest($path, $response);

    // Handle the response.
    if ($response->code != 200) {
      if (($response->code == 503) && ($this->resendOn503($path))) {
        // Make the call again.
        $this->makeDeleteRequest($path);
      }
      else {
        // Something went wrong.
        watchdog('brightpearl',
                'Delete request failed for @path, code: @code error: @error',
                array(
                  '@path' => $path,
                  '@code' => $response->code,
                  '@error' => $response->error),
                WATCHDOG_ERROR);
        $this->lastResponse = '';
        $json_response = drupal_json_decode($response->data);
        if (isset($json_response['errors'])) {
          $this->lastResponseErros = $json_response['errors'];
        }
        return FALSE;
      }
    }
    else {
      // All good.
      $json_response = drupal_json_decode($response->data);
      $this->lastResponse = $json_response;
      $this->lastResponseErros = '';
      return TRUE;
    }
  }

  /**
   * Make a get request.
   */
  public function makePostRequest($path, $data = array()) {
    // Get the url and make the request.
    $url = $this->getUrl($path);
    $encoded_data = drupal_json_encode($data);
    $options = array(
      'method' => 'POST',
      'data' => $encoded_data,
      'timeout' => 30,
      'headers' => array(
        'Content-Type' => 'application/json',
        'brightpearl-dev-ref' => 'bluebag',
        'brightpearl-app-ref' => 'drupal',
        'brightpearl-account-token' => $this->signedToken,
      ),
    );

    $response = drupal_http_request($url, $options);
    $json_response = drupal_json_decode($response->data);

    // Optional log request.
    $this->logRequest($path, $response);

    // Handle the response.
    if ($response->code != 200) {
      if (($response->code == 503) && ($this->resendOn503($path))) {
        // Make the call again.
        $this->makePostRequest($path, $data);
      }
      else {
        // Something went wrong.
        watchdog('brightpearl',
                'Post request failed for @path, code: @code, error: @error, request: @request',
                array(
                  '@path' => $path,
                  '@code' => $response->code,
                  '@error' => $response->error,
                  '@request' => print_r($options, TRUE)),
                WATCHDOG_ERROR);
        $this->lastResponseErros = $json_response['errors'];
        $this->lastResponse = $json_response;
        return FALSE;
      }
    }
    else {
      // All good.
      $this->lastResponse = $json_response;
      $this->lastResponseErros = array();
      return TRUE;
    }
  }

  /**
   * Allows you to get a column from the search result.
   */
  public function getFromSearchResult($column_name, $index = 0) {
    // Make sure we had a successful search.
    if ($this->lastRequestType != BRIGHTPEARL_REQUEST_TYPE_RESOURCE_SEARCH) {
      return FALSE;
    }
    // Find the column name.
    $column_id = -1;
    foreach ($this->lastResponse['response']['metaData']['columns'] as $key => $value) {
      if ($column_name == $value['name']) {
        $column_id = $key;
        break;
      }
    }
    // Do we have the result index.
    if (isset($this->lastResponse['response']['results'][$index])) {
      // This should always be true.
      if (isset($this->lastResponse['response']['results'][$index][$column_id])) {
        return $this->lastResponse['response']['results'][$index][$column_id];
      }
    }
    return FALSE;
  }

  /***************************************************************************
   *  brightPearl - Methods
   ***************************************************************************/

  /**
   * Register a web hook.
   */
  public function registerWebhook($subscribe_to, $callback_uri, $body_template, $http_method = 'POST', $id_set = FALSE) {
    $webhook = array(
      'subscribeTo' => $subscribe_to,
      'httpMethod' => $http_method,
      'uriTemplate' => $callback_uri,
      'bodyTemplate' => $body_template,
      'contentType' => 'application/json',
      'idSetAccepted' => $id_set,
    );
    return $this->makePostRequest('integration-service/webhook', $webhook);
  }

  /**
   * Delete a webhook by ID.
   */
  public function deleteWebhook($webhook_id) {
    return $this->makeDeleteRequest('integration-service/webhook/' . $webhook_id);
  }

  /**
   * List the webhooks.
   */
  public function listWebhooks() {
    if ($this->makeGetRequest('integration-service/webhook')) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Register the stock update web hook.
   *
   * Return the ID of the webhook.
   */
  public function registerStockWebhook($callback_uri, $user_token) {
    $subscribe_to = 'product.modified.on-hand-modified';
    $usertoken = 'token=' . $user_token;
    $body_template = $usertoken . '&resource_id=${resource-id}&resource_type=${resource-type}&event_type=${lifecycle-event}&account_code=${account-code}';
    $ok = $this->registerWebhook($subscribe_to, $callback_uri, $body_template, 'POST');

    if ($ok) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Process a webhook.
   */
  public function processWebhook($post_body, $communication_token) {
    // Make sure a valid request and get the data.
    if ($rd = $this->loadWebhookData($post_body, $communication_token)) {
      // A stock update.
      if (($rd['resource_type'] == 'product') && ($rd['event_type'] == 'modified')) {
        $stock_level = $this->getProductsStockOnHand(array($rd['resource_id']));
        if (isset($this->stockUpdateFunc)) {
          $func = $this->stockUpdateFunc;
          $func($this, $stock_level);
        }
      }
    }
    else {
      if ($this->logAllRequests) {
        watchdog('brightpearl', 'Invalid Webhook received: @body', array('@body' => $post_body), WATCHDOG_WARNING);
      }
    }
  }

  /**
   * Load the body of a webhook into an array and return it.
   */
  private function loadWebhookData($post_body, $communication_token) {
    // Get an array of veriables.
    $rd = array();
    parse_str($post_body, $rd);
    // Make sure we have the corect token.
    if (empty($rd) || !isset($rd['token']) || ($rd['token'] != $communication_token)) {
      return FALSE;
    }
    // Make sure we have the expected data structure.
    if (isset($rd['resource_id']) && isset($rd['resource_type'])
            && isset($rd['event_type']) && isset($rd['account_code'])) {
      if ($this->validateWebhookAccountCode($rd)) {
        return $rd;
      }
    }
    return FALSE;
  }

  /**
   * Validate the account code for the webhook.
   */
  private function validateWebhookAccountCode($request_data) {
    // Make sure accounts match.
    if ($this->accountCode == $request_data['account_code']) {
      return TRUE;
    }
    return FALSE;
  }


  /**
   * Function to add a customer.
   */
  public function findProduct($sku) {
    $this->lastRequestName = 'find_product';
    $this->lastRequestType = BRIGHTPEARL_REQUEST_TYPE_RESOURCE_SEARCH;
    $path = 'product-service/product-search?SKU=' . $sku;
    if ($this->makeGetRequest($path)) {
      return $this->getFromSearchResult('productId');
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the onHand (available for sale) stock level for the product_id.
   *
   * If a warehouse id (int) is specified or a Warehouse Id is set only the
   * warehouse stock will be returned otherwise the total of all available stock
   * is returned.
   */
  public function getProductStockOnHand($product_id, $warehouse_id = NULL) {
    if (is_null($warehouse_id)) {
      $warehouse_id = $this->stockCheckWarehouseId;
    }
    $stock = $this->getProductsAvailability(array($product_id));
    if (!$stock) {
      return FALSE;
    }
    $product_stock = $stock[$product_id];
    if (!$warehouse_id) {
      return $product_stock['total']['onHand'];
    }
    else {
      if (isset($product_stock['warehouses']) && isset($product_stock['warehouses'][$warehouse_id])) {
        return $product_stock['warehouses'][$warehouse_id]['onHand'];
      }
      else {
        // If we did not find the warehouse we don't know the stock level.
        return FALSE;
      }
    }
  }

  /**
   * Get the stock level info for the product_id.
   *
   * Same as getProductStockOnHand() only returns all stock data not only the
   * onHand value.
   */
  public function getProductStockReport($product_id, $warehouse_id = NULL) {
    if (is_null($warehouse_id)) {
      $warehouse_id = $this->stockCheckWarehouseId;
    }

    $stock = $this->getProductsAvailability(array($product_id));
    if (!$stock) {
      return FALSE;
    }
    $product_stock = $stock[$product_id];
    if (!$warehouse_id) {
      return $product_stock['total'];
    }
    else {
      if (isset($product_stock['warehouses']) && isset($product_stock['warehouses'][$warehouse_id])) {
        return $product_stock['warehouses'][$warehouse_id];

      }
      else {
        // If we did not find the warehouse we don't know the stock level.
        return FALSE;
      }
    }
  }

  /**
   * Get the onHand (available for sale) stock level for the array product_ids.
   *
   * Same as getProductStockOnHand() only takes an array of product IDs.
   */
  public function getProductsStockOnHand($product_ids, $warehouse_id = NULL) {
    if (is_null($warehouse_id)) {
      $warehouse_id = $this->stockCheckWarehouseId;
    }

    $stock_array = $this->getProductsAvailability($product_ids);
    if (!$stock_array) {
      return FALSE;
    }
    // Build a simple array to return.
    $stock_list = array();
    // Cycle the reurned array.
    foreach ($stock_array as $product_id => $stock) {
      if (!$warehouse_id) {
        $stock_list[$product_id] = $stock['total']['onHand'];
      }
      else {
        if (isset($stock['warehouses']) && isset($stock['warehouses'][$warehouse_id])) {
          $stock_list[$product_id] = $stock['warehouses'][$warehouse_id]['onHand'];
        }
        else {
          // If we did not find the warehouse we don't know the stock level.
          // so we do nothing.
        }
      }
    }
    return $stock_list;
  }


  /**
   * Get the stock availability response for the specified product.
   */
  public function getProductsAvailability($product_ids = array()) {
    // Make sure ids are sorted low to high.
    asort($product_ids);
    // Covert IDs to a string.
    $ids = implode(',', $product_ids);
    // Create the call path.
    $path = 'warehouse-service/product-availability/' . $ids;
    if ($this->makeGetRequest($path)) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the list of brightpearl tax codes.
   */
  public function getTaxCodes() {
    if ($this->makeGetRequest('accounting-service/tax-code')) {
      return $this->lastResponse;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the list of brightpearl nominal codes.
   */
  public function getNominalCodes() {
    if ($this->makeGetRequest('accounting-service/nominal-code')) {
      return $this->lastResponse;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the product response from brightpearl using the brightpearl product id.
   */
  public function getProduct($product_id) {
    $this->lastRequestName = 'get_product';
    $this->lastRequestType = BRIGHTPEARL_REQUEST_TYPE_RESOURCE_GET;
    $path = 'product-service/product/' . $product_id;
    if ($this->makeGetRequest($path)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the Brightpearl SKU for the brightpearl product id.
   */
  public function getProductSku($product_id) {
    if ($this->getProduct($product_id)) {
      $response = $this->lastResponse['response'];
      // We are expecting one or zero elements.
      if (isset($response[0]) && isset($response[0]['identity']) && isset($response[0]['identity']['sku'])) {
        return $response[0]['identity']['sku'];
      }
    }
    // If we got here then we did not find the product.
    return FALSE;
  }

  /**
   * Find a contact by email.
   */
  public function findContact($email) {
    $this->lastRequestName = 'find_contact';
    $this->lastRequestType = BRIGHTPEARL_REQUEST_TYPE_RESOURCE_SEARCH;
    $path = 'contact-service/contact-search?primaryEmail=' . $email;
    if ($this->makeGetRequest($path)) {
      return $this->getFromSearchResult('contactId');
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get a list of channels.
   */
  public function getChannels() {
    if ($this->makeGetRequest('product-service/channel')) {
      return $this->lastResponse;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get a list of warehouses.
   */
  public function getWarehouses() {
    if ($this->makeGetRequest('warehouse-service/warehouse')) {
      return $this->lastResponse;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get a list of order statuses.
   */
  public function getOrderStatusList() {
    if ($this->makeGetRequest('order-service/order-status')) {
      return $this->lastResponse;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Function to add a customer.
   */
  public function AddContact($first_name, $last_name, $bill_address_id, $delivery_address_id, $email) {
    // Implement character limits.
    if (strlen($first_name) > 32) {
      $first_name = substr($first_name, 0, 32);
    }
    if (strlen($last_name) > 32) {
      $last_name = substr($last_name, 0, 32);
    }

    $contact = array(
      'firstName' => $first_name,
      'lastName' => $last_name,
      'postAddressIds' => array(
        'DEF' => $bill_address_id,
        'BIL' => $bill_address_id,
        'DEL' => $delivery_address_id,
      ),
      'communication'  => array(
        'emails' => array(
          'PRI' => array('email' => $email),
        ),
      ),
    );

    $ok = $this->makePostRequest('contact-service/contact', $contact);
    if ($ok) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }


  /**
   * Function to set the create an address.
   */
  public function createAddress($l1_street, $l2_suburb, $l3_city, $l4_state,
          $post_code, $country_iso_code) {
    // Implement character limits.
    // Street = 256 characters.
    if (strlen($l1_street) > 256) {
      $l1_street = substr($l1_street, 0, 256);
    }
    // Suburb = 256 characters.
    if (strlen($l2_suburb) > 256) {
      $l2_suburb = substr($l2_suburb, 0, 256);
    }
    // Postcode = 32 characters.
    if (strlen($post_code) > 32) {
      $post_code = substr($post_code, 0, 32);
    }
    // City = 64 characters.
    if (strlen($l3_city) > 64) {
      $l3_city = substr($l3_city, 0, 64);
    }
    // State = 64 characters.
    if (strlen($l4_state) > 64) {
      $l4_state = substr($l4_state, 0, 64);
    }

    // Build the address.
    $address = array(
      'addressLine1' => $l1_street,
      'addressLine2' => $l2_suburb,
      'addressLine3' => $l3_city,
      'addressLine4' => $l4_state,
      'postalCode' => $post_code,
      'countryIsoCode' => $country_iso_code,
    );

    $ok = $this->makePostRequest('contact-service/postal-address', $address);
    if ($ok) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Function to set the order address.
   */
  public function createOrder($contact_id, $urrency_code, $reference,
          $name, $company_name,
          $l1_street, $l2_suburb, $l3_city, $l4_state, $post_code,
          $country_iso_code,
          $telephone, $mobile_telephone, $email, $status_id) {

    // Implement character limits.
    // Street = 256 characters.
    if (strlen($l1_street) > 256) {
      $l1_street = substr($l1_street, 0, 256);
    }
    // Suburb = 256 characters.
    if (strlen($l2_suburb) > 256) {
      $l2_suburb = substr($l2_suburb, 0, 256);
    }
    // Postcode = 32 characters.
    if (strlen($post_code) > 32) {
      $post_code = substr($post_code, 0, 32);
    }
    // City = 64 characters.
    if (strlen($l3_city) > 64) {
      $l3_city = substr($l3_city, 0, 64);
    }
    // State = 64 characters.
    if (strlen($l4_state) > 64) {
      $l4_state = substr($l4_state, 0, 64);
    }
    // Company name = 64 characters.
    if (strlen($company_name) > 64) {
      $company_name = substr($company_name, 0, 64);
    }
    // Contact name = 64 characters.
    if (strlen($name) > 64) {
      $name = substr($name, 0, 64);
    }

    $order = array(
      'orderTypeCode' => 'SO',
      'reference' => $reference,
      // 'placedOn' => date_iso8601($date),
      // 'orderCurrencyCode' => $urrency_code,
      'currency' => array(
        'orderCurrencyCode' => $urrency_code,
      ),
      'orderStatus' => array(
        'orderStatusId' => $status_id,
      ),
      'contactId' => $contact_id,
      'parties' => array(
        'customer' => array(
          'contactId' => $contact_id,
        ),
        'delivery' => array(
          'addressFullName' => $name,
          'companyName' => $company_name,
          'addressLine1' => $l1_street,
          'addressLine2' => $l2_suburb,
          'addressLine3' => $l3_city,
          'addressLine4' => $l4_state,
          'postalCode' => $post_code,
          'countryIsoCode' => $country_iso_code,
          'telephone' => $telephone,
          'mobileTelephone' => $mobile_telephone,
          'email' => $email,
        ),
      ),
      'assignment' => array(
        'current' => array(
          'channelId' => $this->channelId,
        ),
      ),
    );
    $ok = $this->makePostRequest('order-service/order', $order);
    if ($ok) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Function to add rows to an order.
   */
  public function addRowToOrder($order_id, $product_id, $qty, $row_net, $row_vat, $tax_code, $nominal_code = NULL) {
    $order = array(
      'productId' => $product_id,
      'quantity' => array(
        'magnitude' => (int) $qty,
      ),
      'rowValue' => array(
        'taxCode' => $tax_code,
        'rowNet' => array('value' => $row_net),
        'rowTax' => array('value' => $row_vat),
      ),
    );

    $ok = $this->makePostRequest("/order-service/order/$order_id/row", $order);
    if ($ok) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Function to add rows to an order.
   */
  public function addFreeTextRowToOrder($order_id, $text, $qty, $row_net, $row_vat, $tax_code, $nominal_code = NULL) {
    $order = array(
      'productName' => $text,
      'quantity' => array(
        'magnitude' => (int) $qty,
      ),
      'rowValue' => array(
        'taxCode' => $tax_code,
        'rowNet' => array('value' => $row_net),
        'rowTax' => array('value' => $row_vat),
      ),
    );
    if (!is_null($nominal_code)) {
      $order['nominalCode'] = $nominal_code;
    }

    $ok = $this->makePostRequest("/order-service/order/$order_id/row", $order);
    if ($ok) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Add products to the reserve list.
   *
   * Once all products are added we need to call reserve_stock().
   */
  public function addToReserveStockList($product_id, $row_id, $qty) {
    $this->reserveStockList['products'][] = array(
      'productId' => $product_id,
      'salesOrderRowId' => $row_id,
      'quantity' => (int) $qty,
    );
    return TRUE;
  }

  /**
   * Clear the temporary reserveStockList.
   */
  public function clearReserveStockList() {
    $this->reserveStockList = array('products' => array());
  }

  /**
   * Function to add rows to an order.
   */
  public function reserveStock($order_id, $warehouse_id) {
    $ok = $this->makePostRequest("/warehouse-service/order/$order_id/reservation/warehouse/$warehouse_id", $this->reserveStockList);
    return $ok;
  }

  /**
   * Create a payment / sales receipt.
   */
  public function createSalesReceipt($order_id, $currency, $value, $bank_account, $date, $reference, $description) {
    $details = array(
      'orderId' => $order_id,
      'received' => array(
        'currency' => $currency,
        'value' => $value,
      ),
      'invoiceReference' => $reference,
      'bankAccountNominalCode'  => $bank_account,
      'description' => $description,
      'taxDate' => $date,
    );
    $ok = $this->makePostRequest('accounting-service/sales-receipt', $details);
    if ($ok) {
      return $this->lastResponse['response'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Test the connection.
   */
  public function testConnection() {
    if ($this->makeGetRequest('product-service/channel/' . $this->channelId)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  public function getLastlastResponseError() {
    $error = '';
    $errors = $this->lastResponseErros;
    foreach ($errors as $key => $value) {
      $s = $value['code'] . ': ' . $value['message'];
      if ($error == '') {
        $error = $s;
      }
      else {
        $error .= ', ' .$s;
      }
    }
    return $error;
  }
}
