<?php

/**
 * @file
 */

/**
 * Class AmoRestApi.
 */
class AmoRestApi {
  /**
   * URL fro RestAPI
   */
  const URL = 'https://%s.amocrm.ru/private/api/v2/json/';

  /**
   * Auth URL fro RestAPI
   */
  const AUTH_URL = 'https://%s.amocrm.ru/private/api/';

  /**
   * HTTP methods
   */
  const METHOD_GET = 'GET';
  const METHOD_POST = 'POST';

  /**
   * Login access to API.
   */
  protected $login;

  /**
   * Secret key.
   */
  protected $key;

  /**
   * Sub-domain.
   */
  protected $subDomain;

  /**
   * Curl instance
   */
  protected $curl;

  /**
   * Current account info
   */
  protected $accountInfo;

  /**
   * Accounts custom fields
   */
  protected $customFields;

  /**
   * Accounts leads statues info
   */
  protected $leadsStatuses;

  /**
   * Class constructor.
   *
   * @param string $subDomain
   * @param string $email
   * @param string $key
   *
   * @access public
   *
   * @throws /Exception
   */
  public function __construct($subDomain, $email, $key) {
    $this->subDomain = $subDomain;
    $this->login = $email;
    $this->key = $key;
    $url = $this->getUrl('auth.php?type=json', self::AUTH_URL);

    $auth = $this->curlRequest($url, self::METHOD_POST, array(
      'USER_LOGIN' => $email,
      'USER_HASH' => $key,
    ));

    if ($auth['auth'] !== TRUE) {
      throw new Exception('Authorization error.');
    }
  }

  /**
   * Class destructor.
   */
  public function __destruct() {
    curl_close($this->curl);
  }

  /**
   * Get Account Info.
   *
   * @return array
   *
   * @access public
   */
  public function getAccountInfo() {
    $result = array();

    if ($this->accountInfo) {
      return $this->accountInfo;
    }

    $url = $this->getUrl('accounts/current');
    $request = $this->curlRequest($url);

    if (is_array($request) && isset($request['account'])) {
      $this->accountInfo = $request['account'];
      $result = $this->accountInfo;
    }

    return $result;
  }

  /**
   * Get Contacts List.
   *
   * @param int $limitRows
   * @param int $limitOffset
   * @param array $ids
   * @param string $query
   * @param string $responsible
   * @param \DateTime $dateModified
   *
   * @return array
   *
   * @access public
   */
  public function getContactsList($limitRows = NULL, $limitOffset = NULL, array $ids = array(), $query = '', $responsible = '', \DateTime $dateModified = NULL) {
    $headers = array();
    $parameters = array();
    $url = $this->getUrl('contacts/list');

    if ($dateModified) {
      $headers = array('if-modified-since: ' . $dateModified->format('D, d M Y H:i:s'));
    }

    if (!is_null($limitRows)) {
      $parameters['limit_rows'] = $limitRows;

      if (!is_null($limitOffset)) {
        $parameters['limit_offset'] = $limitOffset;
      }
    }

    if ($ids) {
      $parameters['id'] = $ids;
    }

    if ($query) {
      $parameters['query'] = $query;
    }

    if ($responsible) {
      $parameters['responsible_user_id'] = $responsible;
    }

    return $this->curlRequest(
      $url,
      self::METHOD_GET,
      $parameters ? http_build_query($parameters) : NULL,
      $headers
    );
  }

  /**
   * Get Contacts Links.
   *
   * @param int $limitRows
   * @param int $limitOffset
   * @param array $contacts
   * @param array $leads
   * @param \DateTime $dateModified
   *
   * @return array
   *
   * @access public
   */
  public function getContactsLinks($limitRows = NULL, $limitOffset = NULL, array $contacts = array(), array $leads = array(), \DateTime $dateModified = NULL) {
    $headers = array();
    $parameters = array();
    $url = $this->getUrl('contacts/links');

    if ($dateModified) {
      $headers = array('if-modified-since: ' . $dateModified->format('D, d M Y H:i:s'));
    }

    if (!is_null($limitRows)) {
      $parameters['limit_rows'] = $limitRows;

      if (!is_null($limitOffset)) {
        $parameters['limit_offset'] = $limitOffset;
      }
    }

    if ($contacts) {
      $parameters['contacts_link'] = $contacts;
    }
    elseif ($leads) {
      $parameters['deals_link'] = $leads;
    }

    return $this->curlRequest(
      $url,
      self::METHOD_GET,
      $parameters ? http_build_query($parameters) : NULL,
      $headers
    );
  }

  /**
   * Get Leads List.
   *
   * @param int $limitRows
   * @param int $limitOffset
   * @param array $ids
   * @param string $query
   * @param string $responsible
   * @param array $status
   * @param \DateTime $dateModified
   *
   * @return array
   *
   * @access public
   */
  public function getLeadsList($limitRows = NULL, $limitOffset = NULL, array $ids = array(), $query = '', $responsible = '', array $status = array(), \DateTime $dateModified = NULL) {
    $headers = array();
    $parameters = array();
    $url = $this->getUrl('leads/list');

    if ($dateModified) {
      $headers = array('if-modified-since: ' . $dateModified->format('D, d M Y H:i:s'));
    }

    if ($limitRows) {
      $parameters['limit_rows'] = $limitRows;

      if (!is_null($limitOffset)) {
        $parameters['limit_offset'] = $limitOffset;
      }
    }

    if ($ids) {
      $parameters['id'] = $ids;
    }

    if ($query) {
      $parameters['query'] = $query;
    }

    if ($responsible) {
      $parameters['responsible_user_id'] = $responsible;
    }

    if ($status) {
      $parameters['status'] = $status;
    }

    return $this->curlRequest(
      $url,
      self::METHOD_GET,
      $parameters ? http_build_query($parameters) : NULL,
      $headers
    );
  }

  /**
   * Get list of possible leads statuses.
   *
   * @return mixed
   */
  public function getLeadsStatuses() {
    if ($this->leadsStatuses) {
      return $this->leadsStatuses;
    }

    $account = $this->getAccountInfo();
    $this->leadsStatuses = $account['leads_statuses'];

    return $this->leadsStatuses;
  }

  /**
   * Get Company List.
   *
   * @param int $limitRows
   * @param int $limitOffset
   * @param array $ids
   * @param string $query
   * @param string $responsible
   * @param \DateTime $dateModified
   *
   * @return array
   *
   * @access public
   */
  public function getCompanyList($limitRows = NULL, $limitOffset = NULL, array $ids = array(), $query = '', $responsible = '', \DateTime $dateModified = NULL) {
    $headers = array();
    $parameters = array();
    $url = $this->getUrl('company/list');

    if ($dateModified) {
      $headers = array('if-modified-since: ' . $dateModified->format('D, d M Y H:i:s'));
    }

    if ($limitRows) {
      $parameters['limit_rows'] = $limitRows;

      if (!is_null($limitOffset)) {
        $parameters['limit_offset'] = $limitOffset;
      }
    }

    if ($ids) {
      $parameters['id'] = $ids;
    }

    if ($query) {
      $parameters['query'] = $query;
    }

    if ($responsible) {
      $parameters['responsible_user_id'] = $responsible;
    }

    return $this->curlRequest(
      $url,
      self::METHOD_GET,
      $parameters ? http_build_query($parameters) : NULL,
      $headers
    );
  }

  /**
   * Get Tasks List.
   *
   * @param int $limitRows
   * @param int $limitOffset
   * @param array $ids
   * @param string $query
   * @param string $responsible
   * @param string $type
   * @param \DateTime $dateModified
   *
   * @return array
   *
   * @access public
   */
  public function getTasksList($limitRows = NULL, $limitOffset = NULL, array $ids = array(), $query = '', $responsible = '', $type = '', \DateTime $dateModified = NULL) {
    $headers = array();
    $parameters = array();
    $url = $this->getUrl('tasks/list');

    if ($dateModified) {
      $headers = array('if-modified-since: ' . $dateModified->format('D, d M Y H:i:s'));
    }

    if ($limitRows) {
      $parameters['limit_rows'] = $limitRows;

      if (!is_null($limitOffset)) {
        $parameters['limit_offset'] = $limitOffset;
      }
    }

    if ($ids) {
      $parameters['id'] = $ids;
    }

    if ($query) {
      $parameters['query'] = $query;
    }

    if ($responsible) {
      $parameters['responsible_user_id'] = $responsible;
    }

    if ($type) {
      $parameters['type'] = $type;
    }

    return $this->curlRequest(
      $url,
      self::METHOD_GET,
      count($parameters) > 0 ? http_build_query($parameters) : NULL,
      $headers
    );
  }

  /**
   * Get Notes List.
   *
   * @param int $limitRows
   * @param int $limitOffset
   * @param array $ids
   * @param string $element_id
   * @param string $type
   * @param \DateTime $dateModified
   *
   * @return array
   *
   * @access public
   */
  public function getNotesList($limitRows = NULL, $limitOffset = NULL, array $ids = array(), $element_id = '', $type = '', \DateTime $dateModified = NULL) {
    $headers = array();
    $parameters = array();
    $url = $this->getUrl('notes/list');

    if ($dateModified) {
      $headers = array('if-modified-since: ' . $dateModified->format('D, d M Y H:i:s'));
    }

    if ($limitRows) {
      $parameters['limit_rows'] = $limitRows;

      if (!is_null($limitOffset)) {
        $parameters['limit_offset'] = $limitOffset;
      }
    }

    if ($ids) {
      $parameters['id'] = $ids;
    }

    if ($element_id) {
      $parameters['element_id'] = $element_id;
    }

    if ($type) {
      $parameters['type'] = $type;
    }

    return $this->curlRequest(
      $url,
      self::METHOD_GET,
      $parameters ? http_build_query($parameters) : NULL,
      $headers
    );
  }

  /**
   * Set Fields.
   *
   * @param array $fields
   *
   * @return array
   *
   * @access public
   */
  public function setFields(array $fields = array()) {
    if (!$fields) {
      return array();
    }

    // Prepare request.
    $request['request']['fields'] = $fields;
    $requestJson = drupal_json_encode($request);
    $headers = array('Content-Type: application/json');
    $url = $this->getUrl('fields/set');

    return $this->curlRequest($url, self::METHOD_POST, $requestJson, $headers);
  }

  /**
   * Set Contacts.
   *
   * @param array $contacts
   *
   * @return array
   *
   * @access public
   */
  public function setContacts(array $contacts = array()) {
    if (!$contacts) {
      return array();
    }

    // Prepare request.
    $request['request']['contacts'] = $contacts;
    $requestJson = drupal_json_encode($request);
    $headers = array('Content-Type: application/json');
    $url = $this->getUrl('contacts/set');

    return $this->curlRequest($url, self::METHOD_POST, $requestJson, $headers);
  }

  /**
   * Set Leads.
   *
   * @param array $leads
   * @return array
   * @access public
   */
  public function setLeads(array $leads = array()) {
    $result = array();

    if (!$leads) {
      return $result;
    }

    // Prepare request.
    $request['request']['leads'] = $leads;
    $requestJson = drupal_json_encode($request);
    $headers = array('Content-Type: application/json');
    $url = $this->getUrl('leads/set');

    // Do request.
    $response = $this->curlRequest($url, self::METHOD_POST, $requestJson, $headers);

    // Parse leads ids from response and return along with last modified time.
    if (isset($response['leads']['add']) && is_array($response['leads']['add'])) {
      $added_leads = array();

      foreach ($response['leads']['add'] as $key => $lead_info) {
        $added_leads[$key]['id'] = $lead_info['id'];
        $added_leads[$key]['last_modified'] = $response['server_time'];
      }

      $result = $added_leads;
    }
    elseif (isset($response['leads']['update'])) {
      $result = $response;
    }

    return $result;
  }

  /**
   * Set Companies.
   *
   * @param array $companies
   *
   * @return array
   *
   * @access public
   */
  public function setCompany(array $companies = array()) {
    if (!$companies) {
      return array();
    }

    // Prepare request.
    $request['request']['contacts'] = $companies;
    $requestJson = drupal_json_encode($request);
    $headers = array('Content-Type: application/json');
    $url = $this->getUrl('company/set');

    return $this->curlRequest($url, self::METHOD_POST, $requestJson, $headers);
  }

  /**
   * Set Tasks.
   *
   * @param array $tasks
   *
   * @return array
   *
   * @access public
   */
  public function setTasks(array $tasks = array()) {
    if (!$tasks) {
      return array();
    }

    // Prepare request.
    $request['request']['tasks'] = $tasks;
    $requestJson = json_encode($request);
    $headers = array('Content-Type: application/json');
    $url = $this->getUrl('tasks/set');

    return $this->curlRequest($url, self::METHOD_POST, $requestJson, $headers);
  }

  /**
   * Set Notes.
   *
   * @param array $notes
   *
   * @return array
   *
   * @access public
   */
  public function setNotes(array $notes = array()) {
    if (!$notes) {
      return array();
    }

    // Prepare request.
    $request['request']['notes'] = $notes;
    $requestJson = json_encode($request);
    $headers = array('Content-Type: application/json');
    $url = $this->getUrl('notes/set');

    return $this->curlRequest($url, self::METHOD_POST, $requestJson, $headers);
  }

  /**
   * Execution of the request.
   *
   * @param string $url
   * @param string $method
   * @param string $parameters
   * @param array $headers
   * @param string $cookie
   * @param integer $timeout
   *
   * @return array
   *
   * @access protected
   *
   * @throws \Exception
   */
  protected function curlRequest($url, $method = 'GET', $parameters = '', $headers = array(), $cookie = '/tmp/cookie.txt', $timeout = 30) {
    if ($method == self::METHOD_GET && $parameters) {
      $url .= "?$parameters";
    }

    // Get curl handler or initiate it.
    if (!$this->curl) {
      $this->curl = curl_init();
    }

    // Set general arguments.
    curl_setopt($this->curl, CURLOPT_URL, $url);
    curl_setopt($this->curl, CURLOPT_FAILONERROR, FALSE);
    curl_setopt($this->curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($this->curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($this->curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($this->curl, CURLOPT_TIMEOUT, $timeout);
    curl_setopt($this->curl, CURLOPT_HEADER, FALSE);
    curl_setopt($this->curl, CURLOPT_COOKIEFILE, $cookie);
    curl_setopt($this->curl, CURLOPT_COOKIEJAR, $cookie);

    // Reset some arguments, in order to avoid use some from previous request.
    curl_setopt($this->curl, CURLOPT_POST, FALSE);

    curl_setopt($this->curl, CURLOPT_HTTPHEADER, $headers);

    if ($method == self::METHOD_POST && $parameters) {
      curl_setopt($this->curl, CURLOPT_POST, TRUE);

      // Encode parameters if them already not encoded in json.
      if (!$this->isJson($parameters)) {
        $parameters = http_build_query($parameters);
      }

      curl_setopt($this->curl, CURLOPT_POSTFIELDS, $parameters);
    }

    $response = curl_exec($this->curl);
    $statusCode = curl_getinfo($this->curl, CURLINFO_HTTP_CODE);

    $errno = curl_errno($this->curl);
    $error = curl_error($this->curl);

    if ($errno) {
      throw new Exception($error, $errno);
    }

    $result = drupal_json_decode($response);

    if ($statusCode >= 400) {
      throw new Exception($result['response']['error'], $statusCode);
    }

    return isset($result['response']) ? $result['response'] : array();
  }

  /**
   * Check if passed argument is JSON.
   *
   * @param string $string
   *
   * @return bool
   */
  protected function isJson($string) {
    if (!is_string($string)) {
      return FALSE;
    }

    drupal_json_decode($string);
    return (json_last_error() == JSON_ERROR_NONE);
  }

  /**
   * Generate url for request.
   *
   * @param string $path
   * @param string $base
   *
   * @return string
   */
  protected function getUrl($path, $base = '') {
    $base = $base ? $base : self::URL;
    return sprintf($base . $path, $this->subDomain);
  }

  /**
   * Get accounts custom fields and store in self::customFields.
   *
   * @return mixed
   */
  protected function getCustomFields() {
    if ($this->customFields) {
      return $this->customFields;
    }

    $account = $this->getAccountInfo();
    $this->customFields = $account['custom_fields'];

    return $this->customFields;
  }

  /**
   * Getting custom fields id.
   *
   * @param string $fieldName
   * @param string $fieldSection
   *   Possible values contacts or companies
   *
   * @return mixed
   */
  public function getCustomFieldID($fieldName, $fieldSection = 'contacts') {
    $customFields = $this->getCustomFields();

    if (is_array($customFields) && isset($customFields[$fieldSection]) && is_array($customFields[$fieldSection])) {
      foreach ($customFields[$fieldSection] as $customFieldDetails) {
        if ($fieldName === $customFieldDetails['code']) {
          return $customFieldDetails['id'];
        }
      }
    }

    return NULL;
  }
}
