<?php

 /**
  * @file
  *
  * Code for the logic for displaying the form for selecting a company
  */
class Fonecta {

  /**
   * Fonecta interface message codes
   */
  const MESSAGE_CODE_ERROR_TOO_MANY_RESULTS = 'ER9100';
  const MESSAGE_CODE_ERROR_NOTHING_FOUND = 'ER9090';
  const MESSAGE_CODE_SUCCESS = 'OK0000';

  /**
   * Interface configuration
   *
   * @var array
   */
  public $configuration;

  /*
   * Initialize client
   */
  public function __construct() {

    $this->configuration['name_search']['service_name_id'] = variable_get('fonecta_service_name_id');
    $this->configuration['name_search']['service_category_id'] = variable_get('fonecta_service_category_id');
    $this->configuration['name_search']['service_class_id'] = variable_get('fonecta_service_class_id');
    $this->configuration['name_search']['origin_country'] = variable_get('fonecta_origin_country');
    $this->configuration['name_search']['customer_type'] = variable_get('fonecta_customer_type');
    $this->configuration['connection']['username'] = variable_get('fonecta_username');
    $this->configuration['connection']['password'] = str_replace('"', '', variable_get('fonecta_password'));
    $this->configuration['connection']['service_base_uri'] = variable_get('fonecta_base_url');
    $this->configuration['connection']['zend_path'] = variable_get('fonecta_zend_path');
  }

  /**
   * Query Fonecta interface for $name and filter results by record
   * customer_type=B
   *
   * @param string Name to search
   * @return array:
   *    'key'           'value'
   *    ------------------------------------------------------
   *    header          response HEADER SXO
   *    content_header  response CONTENT_HEADER SXO
   *    content         response CONTENT SXO or empty
   */
  public function getCompanyList($name) {

    $response_str = $this->getNameSearchResponse($name);

    if ($response_str === FALSE) return FALSE;

    $response_sxo = simplexml_load_string($response_str);

    $result['header'] = $response_sxo->HEADER;
    $result['content_header'] = $response_sxo->CONTENT_HEADER;

    if (!empty($response_sxo->CONTENT)) {
      foreach ($response_sxo->CONTENT->ENTRY as $entry) {

        // If address publicity not set street address cannot be shown
        if (!((string)$entry->ADDRESS_PUBLICITY === 'X')) {
          unset($entry->STREET_ADDRESS);
        }

        // Filter by business type entry to remove person type entries from results
        if ((string)$entry->CUSTOMER_TYPE === 'B') {
          $result['content'][] = $entry;
        }

      }
    }

    return $result;
  }

  /**
   * Query fonecta interface by name
   *
   * @param string $name Name to search
   * @return string $response Http response body xml
   */
  public function getNameSearchResponse($name) {

    $requestParameters = $this->configuration['name_search'];
    $requestParameters['subs_name'] = $name;

    $requestUri = $this->configuration['connection']['service_base_uri'] . '?';
    foreach ($requestParameters as $key => $value) {
      $requestUri .= $key . '=' . urlencode($value) . '&';
    }
    $requestUri = trim($requestUri, '&');

    $username = $this->configuration['connection']['username'];
    $password = $this->configuration['connection']['password'];

    $context = stream_context_create(array(
      'http' => array(
        'header'  => "Authorization: Basic " . base64_encode("$username:$password"),
      )
    ));

    $data = file_get_contents($requestUri, false, $context);

    return $data;
  }
}
