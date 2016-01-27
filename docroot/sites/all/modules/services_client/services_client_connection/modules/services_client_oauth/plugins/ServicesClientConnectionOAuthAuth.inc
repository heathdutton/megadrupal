<?php

/**
 * OAuth authentication support
 */
class ServicesClientConnectionOAuthAuth extends ServicesClientConnectionAuth {

  /**
   * Last request
   *
   * @var ServicesClientOAuthRequest
   */
  protected $request;

  public function configForm(&$form, &$form_state) {
    $form['add_token'] = array(
      '#type' => 'item',
      '#markup' => t('Authorize !connection or', array(
        '!connection' => l(t('connection'), 'services_client_oauth/request', array('query' => array(
          'connection_name' => $this->connection->name,
        ))),
      ))
    );

    $form['container'] = array(
      '#type' => 'fieldset',
      '#title' => empty($this->config) ? t('Enter keys manually') : t('oAuth Keys'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($this->config),
      '#tree' => FALSE
    );

    $form['container']['consumer_key'] = array(
      '#title' => t('Consumer Key'),
      '#type' => 'textfield',
      '#default_value' => !empty($this->config['consumer_key']) ? $this->config['consumer_key'] : '',
      '#required' => TRUE,
    );

    $form['container']['consumer_secret'] = array(
      '#title' => t('Consumer Secret'),
      '#type' => 'textfield',
      '#default_value' => !empty($this->config['consumer_secret']) ? $this->config['consumer_secret'] : '',
      '#required' => TRUE,
    );

    $form['container']['access_key'] = array(
      '#title' => t('Access Key'),
      '#type' => 'textfield',
      '#default_value' => !empty($this->config['access_key']) ? $this->config['access_key'] : '',
      '#description' => t('Access key is optional. If you want to use 2 legged oauth leave access key and secret empty.')
    );

    $form['container']['access_secret'] = array(
      '#title' => t('Access Secret'),
      '#type' => 'textfield',
      '#default_value' => !empty($this->config['access_secret']) ? $this->config['access_secret'] : '',
    );

    $form['container']['force_port'] = array(
      '#title' => t('Force Port'),
      '#type' => 'textfield',
      '#default_value' => isset($this->config['force_port']) ? $this->config['force_port'] : '',
      '#description' => t('If required force port in URL which is used for calculating signature. I.e. 80 for https connection.')
    );
  }

  public function configFormValidate(&$form, &$form_state) {
    if (!empty($form_state['values']['access_key']) && empty($form_state['values']['access_secret'])) {
      form_set_error('access_secret', 'Access Secret must be provided when using Access Key');
    }
    if (!empty($form_state['values']['access_secret']) && empty($form_state['values']['access_key'])) {
      form_set_error('access_key', 'Access Key must be provided when using Access Secret');
    }
  }

  /**
   * Implements configFormSubmit().
   */
  public function configFormSubmit(&$form, &$form_state) {
    parent::configFormSubmit($form, $form_state);

    $form_state['config']['consumer_key'] = $form_state['values']['consumer_key'];
    $form_state['config']['consumer_secret'] = $form_state['values']['consumer_secret'];
    $form_state['config']['access_key'] = $form_state['values']['access_key'];
    $form_state['config']['access_secret'] = $form_state['values']['access_secret'];
    $form_state['config']['force_port'] = $form_state['values']['force_port'];
  }

  /**
   * Implements prepareRequest().
   *
   * @param ServicesClientConnectionHttpRequest $request
   */
  public function prepareRequest(ServicesClientConnectionHttpRequest &$request) {
    parent::prepareRequest($request);

    if ($this->isAuthorized()) {
      // By default authenticate request before its sent to request plugin
      $this->sign($request);
    }
  }

  /**
   * Check if current plugin configuration has been authorized
   */
  protected function isAuthorized() {
    $keys = array('consumer_key', 'consumer_secret');
    foreach ($keys as $key) {
      if (!isset($this->config[$key]) || empty($this->config[$key])) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Authenticate request
   *
   * @param ServicesClientConnectionHttpRequest $request
   */
  protected function sign(ServicesClientConnectionHttpRequest &$request) {
    // Build params that should be signed
    $params = $this->getRequestParams($request);

    // Get consumer and access token if configured
    $consumer = new DrupalOAuthConsumer($this->config['consumer_key'], $this->config['consumer_secret']);
    // Token doesn't have to be provided for 2legged auth
    $token = NULL;
    if (!empty($this->config['access_key']) && !empty($this->config['access_secret'])) {
      $token = new DrupalOAuthToken($this->config['access_key'], $this->config['access_secret'], $consumer);
    }

    // Create request
    $req = ServicesClientOAuthRequest::from_consumer_and_token($consumer, $token, $request->http_method, $request->url, $params);

    // For special use cases we may need to force port to URL when calculating signature
    if (isset($this->config['force_port']) && !empty($this->config['force_port'])) {
      $req->force_port = $this->config['force_port'];
    }
    $signature_method = DrupalOAuthClient::signatureMethod();

    $req->sign_request($signature_method, $consumer, $token);

    $header = explode(':', $req->to_header());
    $request->http_headers[$header[0]] = trim($header[1]);

    // Store latest OAuthRequest
    $this->request = $req;
  }

  /**
   * Determine which params should be included in signing
   *
   * @param ServicesClientConnectionHttpRequest $request
   */
  protected function getRequestParams(ServicesClientConnectionHttpRequest &$request) {
    $params = NULL;

    // If GET request take any $req->data params
    if ($request->http_method == 'GET' && is_array($request->data) && !empty($request->data)) {
      $params = $request->data;
    }

    // Try to guess whether POST request contains data encoded by http_build_query
    // which could be assigned to signature on remote site.
    if (
      $request->http_method == 'POST' &&
      is_array($request->data_raw) &&
      !empty($request->data_raw) &&
      isset($request->http_headers['Content-Type']) &&
      $request->http_headers['Content-Type'] == 'application/x-www-form-urlencoded'
    ) {
      // If post URL is comming with Query
      $data = $request->data;
      if (!empty($request->query)) {
        if ($data) {
          $data .= '&';
        }
        $data .= http_build_query($request->query, NULL, '&');
      }

      $params = OAuthUtil::parse_parameters($data);
      if (count($params) == 1) {
        $params = array_filter($params);
      }
    }

    // Flatten array keys
    if (is_array($params) && !empty($params)) {
      $params = $this->flattenParamKeys($params);
    }

    return $params;
  }

  /**
   * Flatten array keys as they would be sent to in HTTP query.
   *
   * array('body' => array('und' => array(0 => array('value' => 'val'))))
   *
   * will get converted to:
   *
   * array('body[und][0][value]' => 'val');
   *
   *
   * @param array $params
   *   HTTP query params
   * @return array
   *   Array of params with flattened keys structure
   */
  protected function flattenParamKeys($params) {
    $string = http_build_query($params, NULL, '&');
    $rows = explode('&', $string);
    $out = array();
    foreach ($rows as $row) {
      list($key, $val) = explode('=', $row);
      $out[urldecode($key)] = urldecode($val);
    }
    return $out;
  }
}
