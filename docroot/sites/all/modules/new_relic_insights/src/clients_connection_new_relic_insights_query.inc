<?php
/**
 * @file
 * Contains classes for Client connections handlers.
 */

/**
 * Class for New Relic Insights client query connections.
 */
class clients_connection_new_relic_insights_query extends clients_connection_base implements ClientsRemoteEntityInterface {

  /**
   * Base endpoint for New Relic Insights queries.
   * @var string
   */
  protected $endpoint_base = 'https://insights-api.newrelic.com/v1/accounts';

  /**
   * Declare an array of properties which should be treated as credentials.
   *
   * This lets the credentials storage plugin know which configuration
   * properties to take care of.
   *
   * @return
   *  A flat array of property names.
   */
  function credentialsProperties() {
    return array('account', 'key');
  }

  /**
   * Extra form elements specific to a class's edit form.
   *
   * @param $form_state
   *  The form state from the main form, which you probably don't need anyway.
   *
   * @see clients_connection_form()
   * @see clients_connection_form_submit()
   */
  function connectionSettingsFormAlter(&$form, &$form_state) {
    $form['endpoint']['#description'] = t('Remote service URL e.g. http://mysite.com/service-endpoint');

    // Custom credential properties.
    $form['credentials']['account'] = array(
      '#type' => 'textfield',
      '#title' => t('Account number'),
      '#size' => 24,
      '#maxlength' => 24,
      '#attributes' => array('autocomplete' => 'off'),
      '#description' => t('The account number against which queries will be run.'),
      '#required' => TRUE,
    );
    $form['credentials']['key'] = array(
      '#type' => 'textfield',
      '#title' => t('Query key'),
      '#size' => 32,
      '#maxlength' => 64,
      '#attributes' => array('autocomplete' => 'off'),
      '#description' => t('A query key associated with the account number above.'),
      '#required' => TRUE,
    );

    // Always provide the endpoint and lock it.
    $form['endpoint']['#type'] = 'hidden';
    $form['endpoint']['#default_value'] = $this->endpoint_base;

    // Do not allow credential storage to be configurable.
    $form['credentials']['credentials_storage']['#type'] = 'hidden';
    $form['credentials']['credentials_storage']['#default_value'] = 'variables';
  }

  /**
   * Submit handler for saving/updating connections of this class.
   *
   * @see clients_connection_form_submit()
   */
  function connectionSettingsForm_submit($form, &$form_state) {

    $endpoint = implode('/', array($this->endpoint_base, $form_state['values']['credentials']['account']));

    // Update the endpoint with the account number, append /query.
    $form_state['values']['endpoint'] = $endpoint;
    $form_state['input']['endpoint'] = $endpoint;
    $form_state['clients_connection']->endpoint = $endpoint;

    parent::connectionSettingsForm_submit($form, $form_state);
  }

  // ============================================ Remote Entity methods.

  /**
   * {@inheritdoc}
   */
  function remote_entity_load($entity_type, $id) {

  }

  /**
   * {@inheritdoc}
   */
  function remote_entity_save($entity_type, $entity, $remote_properties = array()) {
    // @todo, support INSERT, but not update.
  }

  /**
   * {@inheritdoc}
   */
  function getRemoteEntityQuery() {
    return new InsightRemoteEntityQuery($this);
  }

  /**
   * {@inheritdoc}
   */
  function entity_property_type_map() {
    return array(
      // Defaults provided by Entity API.
      'text' => 'text',
      'integer' => 'integer',
      'decimal' => 'decimal',
      'date' => 'date',
      'token' => 'token',
      'boolean' => 'boolean',
      'uri' => 'uri',
      'entities' => 'entities',
      'entity' => 'entity',
      'struct' => 'struct',
      'list' => 'list',
      // Mapping hook_schema schema types to Entity API schema types.
      'int' => 'integer',
      'varchar' => 'text',
      'serial' => 'int',
      'float' => 'decimal',
      'numeric' => 'integer',
      'blob' => 'text',
    );
  }

  // ============================================ Resource retrieval.

  /**
   * Common helper for reacting to an error from a REST call.
   *
   * Gets the error from the response, logs the error message,
   * and throws an exception, which should be caught by the module making use
   * of the Clients connection API.
   *
   * @param $response
   *  The REST response data, decoded.
   *
   * @throws Exception
   */
  function handleRestError($response) {
    if ($response->code != 200) {
      watchdog('new relic insights entity', 'Error with Insights request. Error was code @code with error "@error" and message "@message".', array(
        '@code'     => $response->code,
        '@error'    => $response->error,
        '@message'  => isset($response->status_message) ? $response->status_message : '(no message)',
      ));

      throw new Exception(t("Clients connection error, got message '@message'.", array(
        '@message' => isset($response->status_message) ? $response->status_message : $response->error,
      )), $response->code);
    }
  }

  /**
   * API function to request a remote resource.
   *
   * @param $path
   *   The path of the remote resource to retrieve.
   * @param $options
   *   A flat array of further parameters for the request. This should contain,
   *   in this order:
   *   - The HTTP method to use.
   *   - (optional) An array of data for the request, such as POST or extra
   *     query parameters.
   *
   * @return
   *  Whatever is returned from the remote site.
   */
  function callMethodArray($path, $options = array()) {
    $http_method = array_shift($options);

    // The data array doesn't have to be present, so we have to fiddle about
    // to make sure we don't pass a NULL for it to makeRequest().
    if (count($options)) {
      $data = array_shift($options);
    }
    else {
      $data = array();
    }

    return $this->makeRequest($path, $http_method, $data);
  }

  /**
   * Make a REST request.
   *
   * Examples:
   * Retrieve a node:
   *  makeRequest('node/NID', 'GET');
   * Update a node:
   *  makeRequest('node/NID', 'POST', $data);
   *
   * @param $resource_path
   *  The path of the resource. Eg, 'node', 'node/1', etc.
   * @param $http_method
   *  The HTTP method. One of 'GET', 'POST', 'PUT', 'DELETE'. For an explanation
   *  of how the HTTP method affects the resource request, see the Services
   *  documentation at http://drupal.org/node/783254.
   * @param $data = array()
   *  (Optional) An array of data to pass to the request.
   *
   * @return
   *  The data from the request response.
   */
  function makeRequest($resource_path, $http_method, $data = array()) {
    $data = http_build_query($data, '', '&');

    // Data should be passed as query parameters for GET requests.
    if ($http_method == 'GET') {
      if (strpos($resource_path, '?') === FALSE) {
        $resource_path .= '?' . $data;
      }
      else {
        $resource_path .= '&' . $data;
      }

      // Do not send the data in the request body.
      $data = '';
    }

    // Load credentials in and apply to headers.
    $this->credentialsLoad();
    $headers = array(
      'Accept' => 'application/json',
      'X-Query-Key' => $this->credentials['key'],
      'User-Agent' => variable_get('new_relic_insights_ua', 'Drupal (+http://drupal.org/)'),
    );

    $options = array(
      'headers' => $headers,
      'method' => $http_method,
      'data' => $data,
    );

    // Perform the query and handle response errors.
    $response = drupal_http_request($this->endpoint . '/' . $resource_path, $options);
    $this->handleRestError($response);

    // Decode and return the data returned from Insights.
    $result = json_decode($response->data);
    return $result;
  }

}
