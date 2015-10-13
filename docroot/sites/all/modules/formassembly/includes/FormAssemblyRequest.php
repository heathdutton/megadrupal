<?php

/**
 * @file
 * Authorizes the current site and handles API requests to FormAssembly.
 */
class FormAssemblyRequest {

  private $apiHost;
  private $clientId;
  private $clientSecret;
  private $authEndpoint = 'oauth/login';
  private $tokenEndpoint = 'oauth/access_token';
  private $authCallback = 'admin/config/services/formassembly/authorize';
  private $returnUrl;
  private $token;
  private $forms = array();
  private $page = 1;
  private $last_hash = NULL;
  private $current_hash = NULL;
  private $responses = array();

  /**
   * Initialize a new FormAssemblyRequest object.
   *
   * @param string $client_id
   *   The client ID required for authentication.
   *
   * @param string $client_secret
   *   The client secret required for authentication.
   *
   * @param string $endpoint
   *   The url of the api endpoint.
   */
  public function __construct($client_id, $client_secret = '', $endpoint = '') {
    $api_mode = variable_get('formassembly_endpoint', FALSE);
    if ($api_mode) {
      switch ($api_mode) {
        case 'dev':
          $this->apiHost = 'https://developer.formassembly.com';
          break;

        case 'pro':
          $this->apiHost = 'https://app.formassembly.com';
          break;

        case 'ent':
          $this->apiHost = 'https://' . variable_get('formassembly_instance') . '.tfaforms.net';
          break;

      }
      $this->clientId = $client_id;
      $this->clientSecret = $client_secret;
      $this->authCallback = 'admin/config/services/formassembly/authorize';

      if (!empty($_SERVER['HTTPS'])) {
        $protocol = 'https://';
      }
      else {
        $protocol = 'http://';
      }
      $this->returnUrl = $protocol . $_SERVER['SERVER_NAME'] . '/' . $this->authCallback;
    }
    else {
      watchdog('FormAssembly', 'Connection Failure: API Endpoint must be configured', WATCHDOG_ERROR);
    }
  }

  /**
   * Redirect the user so to authorize the application using OAuth2.0.
   *
   * @return bool|mixed
   *   Returns an access token if the user has already authorized.
   */
  public function authorize() {
    $token = $this->getToken();
    if ($token) {
      return $token;
    }

    $params = array(
      'type' => 'web',
      'client_id' => $this->clientId,
      'redirect_uri' => $this->returnUrl,
      'response_type' => 'code',
    );

    $auth_uri = $this->apiHost . '/' . $this->authEndpoint . '?' . drupal_http_build_query($params);
    drupal_goto($auth_uri);
    return NULL;
  }

  /**
   * Retrieve an active access_token from the database or request a new one.
   *
   * @param string $code
   *   A hash passed into $_GET['code'] after the user has been redirected back
   *   from the auth portal. Needed for new token requests.
   *
   * @return bool|null
   *   An access token if available or FALSE if something has gone wrong.
   */
  public function getToken($code = '') {
    // Retrieve a previously generated token.
    $token = variable_get('formassembly_oauth_access_token', '');
    if (!empty($token)) {
      return $token;
    }

    // Something went wrong - maybe they ended up at this page by accident?
    if (empty($code)) {
      return FALSE;
    }

    // Prepare to request a new access_token.
    $data = array(
      "grant_type" => "authorization_code",
      "type" => "web_server",
      "client_id" => $this->clientId,
      "client_secret" => $this->clientSecret,
      "redirect_uri" => $this->returnUrl,
      "code" => $code,
    );

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $this->apiHost . '/' . $this->tokenEndpoint);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    $response = curl_exec($ch);
    unset($ch);

    $response = json_decode($response);
    if (isset($response->access_token)) {
      $this->setToken($response->access_token);
      variable_set('formassembly_oauth_access_token', $this->token);
      return $this->token;
    }

    watchdog('formassembly', 'Could not retrieve access token.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  /**
   * Request all form data from FormAssembly and perform CRUD ops as needed.
   */
  public function syncForms() {
    $forms_by_faid = array();
    $fa_form_controller = entity_get_controller('fa_form');
    foreach ($this->forms as $form_item) {
      $forms_by_faid[$form_item['Form']['id']] = array(
        'faid' => $form_item['Form']['id'],
        'name' => filter_xss(decode_entities($form_item['Form']['name'])),
        'modified' => date('U', strtotime($form_item['Form']['modified'])),
      );
    }
    // Update forms that have changed since the last sync.
    foreach ($forms_by_faid as $form_data) {
      // Load an existing fa_id entity if one matches $form_data->id or
      // create a new entity otherwise.
      $fa_form_search = $fa_form_controller->loadByProperties(array('faid' => $form_data['faid']));
      // The search returns data as an array keyed by eid or an empty array if
      // no match. There should only be one item - faid is a unique key so pop
      // the first item off the array or we get NULL if the array was empty...
      $found_form = array_shift($fa_form_search);
      if ($found_form != NULL) {
        // Update forms that have changed since the last sync.
        if ($found_form->getModified() < $form_data['modified']) {
          // Update the title and modified date stored.
          $found_form->updateData($form_data);
          $fa_form_controller->save($found_form);
        }
      }
      else {
        $new_fa_form = $fa_form_controller->create($form_data);
        $fa_form_controller->save($new_fa_form);
      }
      // Now check for fa_forms that are no longer in $forms_by_faid.
      // First pull the faids currently stored...
      $stored = $fa_form_controller->loadPropertySet('faid');
      // Now check those against the faids from the rest request...
      foreach ($stored as $record) {
        if (!array_key_exists($record->faid, $forms_by_faid)) {
          // The faid of the current record is not a key of $forms_by_faid,
          // so the form is no longer in FormAssembly.
          $eids_to_delete[] = $record->eid;
        }
      }
      if (isset($eids_to_delete)) {
        formassembly_delete_multiple($eids_to_delete);
      }
    }
    watchdog('formassembly', 'Form sync complete.', array(), WATCHDOG_NOTICE);
  }

  /**
   * Set an access token to use in the current request.
   *
   * @param string $token
   *   A valid access_token as returned by FA's API.
   */
  public function setToken($token) {
    $this->token = $token;
  }

  /**
   * Get data about available forms.  If admin index is in use, get a single page of data.
   *
   * @param string $endpoint
   *   The api endpoint to make the request against.
   * @param boolean $is_admin
   *   Flag indicating if $endpoint is an admin index
   *
   * @return array
   *   An array of objects representing FormAssembly forms.
   */
  public function getForms($endpoint = 'api_v1/forms/index.json', $is_admin = FALSE) {
    //prepare a processing array for forms
    $finished = false;
    if ($is_admin) {
      $request_uri = $this->apiHost . '/' . $endpoint . '?access_token=' . urlencode($this->token) . '&show=50&page=' . $this->page;
      $response = drupal_http_request($request_uri);
      // @todo add more exception processing on http code
      // If we don't get JSON on the first try there is an error
      // on subsequent pages FormAssembly indicates that the last page could be empty
      if ($this->page == 1 && !$this->isJson($response->data)) {
        throw new Exception($response->data);
      }
      //increment flags
      if (!empty($this->current_hash)) {
        $this->last_hash= $this->current_hash;
      }
      // hash the request so we can easily see if it has changed
      $this->current_hash = md5($response->data);
      //We store the hashes in the object to keep all the properties of the request together
      // but the boolean expression below is easier to read without also checking for empty object properties.
      $last_hash = $this->last_hash;
      $current_hash = $this->current_hash;
      if (!(empty($response->data) || $last_hash === $current_hash)) {
        // observed behavior from formassembly is if n pages are needed to iterate the admin index
        // page n+1 returns the same response as page n
        // but formassembly advises this could change to page n+1 returns empty. Neither of these responses should
        // be processed and we are finished.
        // Otherwise, process response
        $this->responses[] = drupal_json_decode($response->data);
        // increment page
        ++$this->page;
      }
      else {
        $finished = TRUE;
      }
    }
    else {
      $request_uri = $this->apiHost . '/' . $endpoint . '?access_token=' . urlencode($this->token);
      $response = drupal_http_request($request_uri);
      // @todo add more exception processing on http code
      if (!$this->isJson($response->data)) {
        throw new Exception($response->data);
      }
      $this->responses[] = drupal_json_decode($response->data);
      $finished = TRUE;
    }
    return $finished;
  }

  /**
   * process responses property into forms array
   */
  public function processResponses() {
    if (!empty($this->responses)) {
      $forms = array();
      foreach ($this->responses as $response_array) {
        if (!empty($response_array['Forms'])) {
          $forms = array_merge($forms, $response_array['Forms']);
        }
        if (!empty($response_array['Category'])) {
          $this->extractCategories($forms, $response_array['Category']);
        }
      }
      $this->forms = $forms;
    }
  }

  /**
   * @param array $forms
   *   the processing array for forms
   * @param array $Category
   *    a Category array - may contain additional Category arrays
   */
  public function extractCategories(array &$forms, array $Category) {
    foreach ($Category as $formset) {
      if (!empty($formset['Category'])) {
        $this->extractCategories($forms, $formset['Category']);
      }
      if (!empty($formset['Forms'])) {
        $forms = array_merge($forms, $formset['Forms']);

      }
    }
  }

  /**
   * Retrieve the HTML for a FormAssembly form.
   *
   * The FA API recognizes query parameters passed on the rest URL and will
   * use them to pre-fill fields in the returned form markup.  Here we fold
   * in parameters configured via formassembly_form() and expose the hook
   * hook_formassembly_form_params_alter(&$params) to allow modules to modify
   * the passed parameter list.
   *
   * @param $entity
   *   Entity form object.
   *
   * @return string
   *   HTML representation of the form.
   */
  public function getFormMarkup($entity) {

    // Add configured query params passed to FA API.
    $entity_wrapped = entity_metadata_wrapper('fa_form', $entity);
    $query_params = $entity_wrapped->fa_query_params->value();
    $params = array();
    if (!empty($query_params)) {
      $params = unserialize($query_params);
    }

    // Expose hook_formassembly_form_params_alter().
    drupal_alter('formassembly_form_params', $params);

    // Replace any tokens found in the parameter pair values.
    foreach ($params as $key => $value) {
      $params[$key] = token_replace($value);
    }

    // Make FA rest call and return form markup.
    $request_uri = url($this->apiHost . '/rest/forms/view/' . $entity->faid, array('query' => $params));
    $response = drupal_http_request($request_uri);
    return $response->data;
  }

  /**
   * Retrieve the HTML for a FormAssembly next path using a returned tfa_next value.
   *
   * @param string $tfa_next
   *   The urlencoded parameter from of FormAssembly.
   *
   * @return string
   *   HTML returned by the query
   */
  public function getNextForm($tfa_next) {
    $query_path = urldecode($tfa_next);
    $request_uri = $this->apiHost . '/rest/' . $query_path;
    $response = drupal_http_request($request_uri);
    return $response->data;
  }

  /**
   * Retrieve the current request page value.
   *
   * @return int
   */
  public function getPage() {
    return $this->page;
  }

  /**
   * Helper function check if a string is JSON format.
   *
   * @param string $string
   *   The string to check.
   *
   * @return bool
   *   True if $string is JSON.
   */
  protected function isJson($string) {
    json_decode($string);
    return (json_last_error() == JSON_ERROR_NONE);
  }

}
