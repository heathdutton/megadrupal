<?php

/**
 * @file
 * Classes for interacting with the Magento Core API.
 */

/**
 * Provide central method to interact with the Magento Core API.
 */
class Magento {

  function __construct(MagentoConnection $connection) {
    try {
      $url = $connection->soap_url . '/api/soap/?wsdl';
      $options = array();

      $this->magento = new SoapClient($url, $options);
      $this->magento_session = $this->magento->login($connection->api_username, $connection->api_password);
    } catch (Exception $e) {
      $message = 'Fatal error on logging into to the Magento API. Details: <pre>@data</pre>';
      $variables = array('@data' => print_r($e, TRUE));
      watchdog('magento', $message, $variables, WATCHDOG_NOTICE, 'link');
      return;
    }

    $this->connected = TRUE;

  }

  /**
   * Wrapper function around call() method on Magento SOAP requests.
   *
   * @param string $api_method
   *   The Magento Core API method to call.
   * @param array $data
   *   Data to pass into the API call.
   * @param boolean $multicall
   *   True if this is a multiple API call where multiple calls are bundled into
   *   one request for efficiency.
   *
   * @return
   *   A SOAP call to the $api_method API method.
   */
  public function call($api_method = NULL, $data = NULL, $multicall = FALSE) {
    if (!isset($this->magento) || !is_object($this->magento)) {
      return FALSE;
    }

    try {
      if ($multicall) {
        // @todo Inspect response errors from Magento and log them to Watchdog
        // to ease debugging.
        return $this->magento->multiCall($this->magento_session, $data);
      }
      else {
        // @todo Inspect response errors from Magento and log them to Watchdog
        // to ease debugging.
        if (!empty($this->magento_session)) {
          return $this->magento->call($this->magento_session, $api_method, $data);
        }
      }
    } catch (Exception $e) {
      $message = 'Fatal error on pinging to the Magento API. Details: <pre>@data</pre>';
      $variables = array('@data' => print_r($e, TRUE));
      watchdog('magento', $message, $variables, WATCHDOG_NOTICE, 'link');
    }
  }

  /**
   * Get a flat array of all categories in the store.
   *
   * @return array
   *   A flat array containing every category id indexed by the same category
   *   id.
   */
  public function getCategories() {
    $categories = &drupal_static(__CLASS__ . __FUNCTION__);
    if (isset($categories)) {
      return $categories;
    }

    // No cache, so ping the API.
    $magento_categories = $this->call('catalog_category.tree');
    $categories = $this->flattenCategories($magento_categories);
    return $categories;
  }

  /**
   * Flatten a tree list of categories.
   *
   * @param array $categories
   *   A tree array of categories returned by Magento via
   *   'catalog_category.tree'.
   * @return array
   *   A flat array containing every category id indexed by the same category
   *   id.
   */
  private function flattenCategories($categories) {
    $flat_catgegory_ids = array();

    // Magento doesn't return an array for the first category so we clip it off
    // and overload $categories with children.
    if (isset($categories['category_id'])) {
      $categories = array($categories);
    }

    foreach ($categories as $category) {
      $flat_catgegory_ids[$category['category_id']] = $category['category_id'];

      if (!empty($category['children'])) {
        $flat_catgegory_ids += $this->flattenCategories($category['children']);
      }
    }

    return $flat_catgegory_ids;
  }

  function __destruct() {
    if (!isset($this->magento) || !is_object($this->magento)) {
      return;
    }

    try {
      // Close our connection to Magento API.
      if (!empty($this->magento_session)) {
        $this->magento->endSession($this->magento_session);
      }
    } catch (Exception $e) {
      $message = 'Fatal error on closing down the Magento API. Details: <pre>@data</pre>';
      $variables = array('@data' => print_r($e, TRUE));
      watchdog('magento', $message, $variables, WATCHDOG_NOTICE, 'link');
    }
  }

}

/**
 * Simple class to provide connection information for Magento API.
 */
class MagentoConnection {

  function __construct() {
    $this->api_username = variable_get('magento_api_username');
    $this->api_password = variable_get('magento_api_password');
    $this->use_http_authentication = variable_get('magento_use_http_authentication', FALSE);
    $this->http_login = variable_get('magento_http_authentication_login');
    $this->http_password = variable_get('magento_http_authentication_password');
    $this->soap_url = variable_get('magento_store_url');

    if ($this->use_http_authentication) {
      // Until PHP Bug 27777 is fixed we have to pass the login via the URL.
      // When we do remove this we will need to add options in the constructor
      // to pass the login information.
      // @see https://bugs.php.net/bug.php?id=27777
      preg_match_all('/(http:\/\/)(.+)/', $this->soap_url, $matches);
      $protocol = $matches[1][0];
      $uri = $matches[2][0];
      $this->soap_url = "{$protocol}{$this->http_login}:{$this->http_password}@{$uri}";
    }
  }

}
