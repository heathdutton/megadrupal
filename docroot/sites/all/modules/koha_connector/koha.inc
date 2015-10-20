<?php
/**
 * @file
 * Contains the Koha class.
 *
 * Koha connector is an OPAC connector for Koha
 */

class Koha implements OpacConnector {

  protected $connectorId = 'Koha';
  protected $connectorName = 'OPAC connector for Koha';
  protected $connectorConf;
  public $host;
  protected $servicePath = '/cgi-bin/koha/ilsdi.pl';
  protected $restServicePath = '/cgi-bin/koha/rest.pl';
  protected $config;

  /**
   * Constructor.
   */
  public function __construct() {
    $module_path = drupal_get_path('module', 'koha_connector');
    $local_conf_filepath = "$module_path/koha.conf.local";
    if (file_exists($local_conf_filepath)) {
      $this->connectorConf = $local_conf_filepath;
    }
    else {
      $this->connectorConf = "$module_path/koha.conf";
    }
  }

  /**
   * Implements OpacConnector::has().
   */
  public function has($feature) {
    $has = FALSE;

    switch ($feature) {
      case 'issues_history':
      case 'suggestions':
        $has = TRUE;
    }

    return $has;
  }

  /**
   * Returns the identity of the connector.
   *
   * @return ARRAY
   *   - id: must be exactely the file name of the connector without extention.
   *   - name: can be whatever you want
   */
  public function identity() {
    return array('id' => $this->connectorId, 'name' => t($this->connectorName));
  }

  /**
   * Return configuration for a given section with translated labels.
   */
  protected function getConfig($section) {
    if (!isset($this->config)) {
      $this->config = drupal_parse_info_file($this->connectorConf);
    }
    $config = $this->config[$section];
    $keys_to_translate = array('label', 'description');
    foreach ($config as &$field) {
      foreach ($keys_to_translate as $key) {
        if (array_key_exists($key, $field)) {
          $field[$key] = t($field[$key], array(), array('context' => 'libraries'));
        }
      }
    }
    return $config;
  }

  /**
   * Returns available fields for patrons.
   */
  public function patronFields() {
    return $this->getConfig('patronFields');
  }

  /**
   * Returns available fields for items.
   */
  public function itemFields() {
    return $this->getConfig('itemFields');
  }

  /**
   * Returns available fields for biblios.
   */
  public function biblioFields() {
    return $this->getConfig('biblioFields');
  }

  /**
   * Returns available fields for authorities.
   */
  public function authFields() {
    return $this->getConfig('authFields');
  }

  /**
   * Returns available fields for patron checkouts.
   *
   * Note: remote_field is only useful internally.
   */
  public function patronCheckoutsFields() {
    $conf = $this->getConfig('patronCheckoutsFields');
    return array_merge($conf, array(
      // Those fields are required by OPAC module.
      'title' => array(
        'remote_field' => 'title',
        'label' => t('Title'),
      ),
      'date' => array(
        'remote_field' => 'issuedate',
        'label' => t('Issue date'),
        'type' => 'date',
      ),
      'duedate' => array(
        'remote_field' => 'date_due',
        'label' => t('Due date'),
        'type' => 'date',
      ),
    ));
  }

  /**
   * Implements OpacConnector::patronIssuesHistoryFields().
   */
  public function patronIssuesHistoryFields() {
    $conf = $this->getConfig('patronIssuesHistoryFields');
    return array_merge($conf, array(
      // Those fields are required by OPAC module.
      'title' => array(
        'remote_field' => 'title',
        'label' => t('Title'),
      ),
      'date' => array(
        'remote_field' => 'issuedate',
        'label' => t('Issue date'),
        'type' => 'date',
      ),
      'returndate' => array(
        'remote_field' => 'returndate',
        'label' => t('Return date'),
        'type' => 'date',
      ),
    ));
  }

  /**
   * Returns available fields for patron holds.
   *
   * Note: remote_field is only useful internally.
   */
  public function patronHoldsFields() {
    $conf = $this->getConfig('patronHoldsFields');
    return array_merge($conf, array(
      // Those fields are required by OPAC module.
      'title' => array(
        'remote_field' => 'title',
        'label' => t('Title'),
      ),
      'date' => array(
        'remote_field' => 'reservedate',
        'label' => t('Reserve date'),
        'type' => 'date',
      ),
      'status' => array(
        'label' => t('Status'),
      ),
    ));
  }

  /**
   * Change the pass in ILS by webservice.
   *
   * @param string $user_name
   *   User login.
   *
   * @param string $new_password
   *   New password.
   */
  public function changePassword($user_name, $new_password) {

    module_load_include('inc', 'opac', "includes/opac.harvest");

    $url = $this->host . "/" . $this->restServicePath . "/auth/change_password";

    $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
    $data    = "user_name=$user_name&new_password=$new_password";
    $options = array(
      'headers' => $headers,
      'method' => 'PUT',
      'data' => $data,
    );

    $result = $this->httpRequest($url, $options, TRUE);

    // Did it work?
    $data = drupal_json_decode($result->data);
    return $data[0]['success'];
  }

  /**
   * Create a user in ILS by webservice.
   *
   * @param array $data
   *   Everything needed for a user creation as a key/value array.
   */
  public function createUser($data) {

    $url = $this->host . "/" . $this->restServicePath . "/user";
    $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
    $query = "data=" . drupal_json_encode($data);
    $options = array(
      'headers' => $headers,
      'method' => 'POST',
      'data' => $query,
    );

    $result = $this->httpRequest($url, $options, TRUE);

    // Did it work?
    $data = drupal_json_decode($result->data);
    return $data['borrowernumber'];
  }

  /**
   * Modify an existing user in ILS.
   *
   * @param string $user_name
   *   User name
   *
   * @param array $data
   *   An associative array where keys are names of fields to modify and values
   *   are new values for those fields.
   *
   * @return array
   *   An associative array with the following keys:
   *   - success: A boolean that indicates if modification succeeded or not.
   *   - modified_fields: An associative array where keys are names of modified
   *     fields and values are new values for those fields.
   */
  public function modifyUser($user_name, $data) {
    $url = $this->host . '/' . $this->restServicePath . "/user/$user_name";
    $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
    $options = array(
      'method' => 'PUT',
      'headers' => $headers,
      'data' => 'data=' . drupal_json_encode($data),
    );

    $result = $this->httpRequest($url, $options, TRUE);
    if (isset($result)) {
      return drupal_json_decode($result->data);
    }

    return array('success' => FALSE);
  }

  /**
   * Delete an existing user from ILS.
   *
   * @param string $user_name
   *   User name.
   */
  public function deleteUser($user_name) {
    $url = $this->host . '/' . $this->restServicePath . "/user/$user_name";
    $options = array(
      'method' => 'DELETE',
    );
    $result = $this->httpRequest($url, $options);
    if (isset($result)) {
      $response = drupal_json_decode($result->data);
      return $response['deleted'];
    }

    return FALSE;
  }

  /**
   * Does the given login exists in the ILS?
   *
   * @param array $login
   *   The login of the user
   */
  public function loginExists($login) {
    $url = $this->host . "/" . $this->restServicePath . "/user/$login/exists";
    $result = drupal_http_request($url);
    $data = drupal_json_decode($result->data);
    return $data['exist'];
  }

  /**
   * Returns informations about the webservices used by the connector.
   *
   * @return array
   *   Informations about the webservices used by the connector, as follows:
   *   array (
   *     0 =>
   *     array (
   *       // Webservice name, mandatory
   *       'name' => 'RESTWebService',
   *
   *       // Webservice url, mandatory
   *       'url' => 'http://example.com/rest/informations',
   *
   *       // HTTP Response code, mandatory
   *       'code' => '200',
   *
   *       // Additional informations about the webservice, as key/values,
   *       // optional.
   *       'infos' =>
   *       array (
   *         'key1' => 'value1',
   *         'key2' => 'value2',
   *         ...
   *       )
   *
   *       // Raw response from the webservice, optional
   *       'raw' => 'rawresponse'
   *     ),
   *     1 =>
   *     array (
   *     // Webservice name...
   *     ...
   *     )
   *     ...
   *   )
   */
  public function getInfos() {
    module_load_include('inc', 'opac', "includes/opac.harvest");

    // Testing REST.
    $url = $this->host . "/" . $this->restServicePath . "/informations";
    $result = drupal_http_request($url);
    $data[0]['name'] = "REST";
    $data[0]['url'] = $url;
    $data[0]['code'] = $result->code;
    $response = drupal_json_decode($result->data);
    // If we got a response.
    if ($response) {
      // If we got a 'OK' status, it means we can display additional
      // informations.
      if ($result->code == '200') {
        $data[0]['infos'] = $response[0];
      }
      // If not, we display the raw response.
      else {
        $data[0]['raw'] = $response[0];
      }
    }

    // Testing ILSDI.
    $url = $this->host . "/" . $this->servicePath . "/?service=GetRecords&id=1";
    $result = drupal_http_request($url);
    $data[1]['name'] = "ILS-DI";
    $data[1]['url'] = $url;
    $data[1]['code'] = $result->code;
    // As ILS-DI currently returns 200 even if it's forbidden, let's handle
    // this differently: we send the raw xml response, so the user can check
    // it manually.
    $response = $result->data;
    $data[1]['raw'] = $response;

    return $data;

  }

  /**
   * Perform an authentification request with ILS.
   *
   * @param string $login
   *   User login.
   *
   * @param string $pass
   *   User password.
   *
   * @return bool
   *   Patron id if authentification succeeded or FALSE.
   */
  public function authenticateUser($login, $pass) {
    $url = $this->host . $this->servicePath . "?service=AuthenticatePatron&username=$login&password=$pass";
    $xml = @simplexml_load_file($url);

    if (isset($xml->id)) {
      return (int) $xml->id;
    }
    else {
      return 0;
    }
  }

  /**
   * Perform a patron lookup with ILS.
   *
   * Looks up a patron in the ILS by an identifier,
   * and returns the ILS identifier for that patron, aka the patron identifier.
   *
   * @param string $login
   *   User login.
   *
   * @param string $type
   *   Login type
   *
   * @return bool
   *   Patron id if authentification succeeded or FALSE.
   */
  public function lookupUser($login, $type = 'userid') {
    $url = $this->host . $this->servicePath . "?service=LookupPatron&id=$login&id_type=$type";
    $xml = @simplexml_load_file($url);

    if (isset($xml->id)) {
      return (int) $xml->id;
    }
    else {
      return 0;
    }
  }


  /**
   * Get marc data for a given record from ILS.
   *
   * @param string $id
   *   Record ILS identifier.
   *
   * @return array
   *   an array with marc fields.
   */
  public function marc($id) {
    $url = $this->host . $this->servicePath . '?service=GetRecords&id=' . $id;
    $xml = @simplexml_load_file($url);

    if ($xml->record->code == "RecordNotFound") {
      return '';
    }
    $record = @simplexml_load_string($xml->record->marcxml);
    $marc = $this->parseMarcSubfields($record);
    if (!count($marc)) {
      return '';
    }
    return $marc;
  }

  /**
   * Get marc data for a given record from ILS.
   *
   * @param array $ids
   *   List of record identifiers.
   *
   * @param array $wanted_fields
   *   Fields list we want to retrieve.
   *
   * @return array
   *   Records list that match $ids with wanted fields.
   *   Returning a single negative value means the records could not be retrieved:
   *    -1 for an unknown cause.
   *    -2 for a disabled webservice.
   */
  public function getBiblios($ids, $wanted_fields) {
    module_load_include('inc', 'koha_connector', 'includes/plugin');

    $bnums = implode('+', $ids);
    $url = $this->host . $this->servicePath . '?service=GetRecords&id=' . $bnums;
    $xml = @simplexml_load_file($url);

    $available_fields = $this->biblioFields();

    $bibs = array();
    foreach ($ids as $id) {
      $bibs[$id] = array();
    }

    // If we do not have an xml file, we couldn't retrieve the biblios, so let's abort.
    if (!$xml) {
      return -1;
    }

    // If ILS-Di is disabled, let's abort.
    if ($xml->code == 'NotAllowed') {
      return -2;
    }

    foreach ($xml->record as $record) {
      $bib = array();
      if ($record->code == "RecordNotFound") {
        continue;
      }
      $identifier = (int) $record->biblionumber;

      $xml2 = @simplexml_load_string($record->marcxml);
      $bib_info_marc = $this->parseMarcSubfields($xml2);

      if (!count($bib_info_marc)) {
        continue;
      }

      $bib = $this->getWrapper($wanted_fields, $available_fields, $bib_info_marc);

      // Title information.
      $bib['title'] = '';
      $title = $this->get(array('value' => '200$a'), $bib_info_marc);
      $bib['title'] = $title[0];
      if (!$bib['title']) {
        $bib['title'] = ' ';
      }
      if (drupal_strlen($bib['title']) > 255) {
        $bib['title'] = drupal_substr($bib['title'], 0, 255);
      }

      $bibs[$identifier] = $bib;
    }
    return $bibs;
  }

  /**
   * Transform MARC data from ILS into a Drupal-node suitable structure.
   *
   * If a plugin is defined for a given field, it will be called.
   * If not, the standard "get" function will be called.
   *
   * @param array $wanted_fields
   *   List of fields we want to get
   *
   * @param array $available_fields
   *   List of fields that are available
   *
   * @return array
   *   Suitable structure for creating a drupal node
   */
  protected function getWrapper($wanted_fields, $available_fields, $info_marc) {

    $record = array();

    // Wanted fields.
    foreach ($wanted_fields as $fieldname) {

      // For every koha field wanted for a given drupal field.
      if (array_key_exists($fieldname, $available_fields)) {
        foreach ($available_fields[$fieldname]['get'] as $get) {

          // If we have a plugin, we call it.
          if (isset($get['plugin'])) {
            $plugin = koha_connector_plugin_get($get['plugin']);
            koha_connector_plugin_require_file($plugin);
            if ($plugin && function_exists($plugin['callback'])) {

              // If we alreay have values for this given field, we add them.
              if (array_key_exists($fieldname, $record) && is_array($record[$fieldname])) {
                foreach ($plugin['callback']($info_marc, $get) as $value) {
                  array_push($record[$fieldname], $value);
                }
              }
              else {
                $record[$fieldname] = $plugin['callback']($info_marc, $get);
              }
            }
          }
          else {
            // Else, we call the regular get function.
            // If we alreay have values for this given field, we add them.
            if (array_key_exists($fieldname, $record) && is_array($record[$fieldname])) {
              foreach ($this->get($get, $info_marc) as $value) {
                array_push($record[$fieldname], $value);
              }
            }
            else {
              $record[$fieldname] = $this->get($get, $info_marc);
            }

            if (!count($record[$fieldname])) {
              unset($record[$fieldname]);
            }
          }
        }
      }
    }
    return $record;
  }

  /**
   * Get marc data for a given authority record from ILS.
   *
   * @param array $ids
   *   List of record identifiers.
   *
   * @param array $wanted_fields
   *   Fields list we want to retrieve.
   *
   * @return array
   *   Records list that match $ids with wanted fields
   */
  public function getAuthorities($ids, $wanted_fields = array()) {
    module_load_include('inc', 'koha_connector', 'includes/plugin');

    $ids = implode('+', $ids);
    $url = $this->host . $this->servicePath . '?service=GetAuthorityRecords&id=' . $ids;
    $xml = @simplexml_load_file($url);

    $available_fields = $this->authFields();
    if (count($wanted_fields) == 0) {
      $wanted_fields = array_keys($available_fields);
    }

    $auths = array();
    foreach ($xml->record as $record) {
      $auth = array();
      if ($record->code == "RecordNotFound") {
        continue;
      }
      $xml2 = @simplexml_load_string($record);
      $auth_info_marc = $this->parseMarcSubfields($xml2);
      if (!count($auth_info_marc)) {
        continue;
      }

      $auth = $this->getWrapper($wanted_fields, $available_fields, $auth_info_marc);

      $identifier = $auth_info_marc['controlfields']['001'];
      $auths[$identifier] = $auth;
    }
    return $auths;
  }

  /**
   * Function itemsStatus checks items insformations.
   *
   * Retrieve data about existing items for a given record.
   *
   * @param string $id
   *   The record identifier for which we want the items informations
   *
   * @param array $wanted_fields
   *   Fields list we want to retrieve.
   *
   * @param string $user_id
   *   Patron id if you want to know if items are holdable by this patron.
   *
   * @return array:
   *   Items list with wanted fields + avail and holdable fields.
   */
  public function itemsStatus($id, $wanted_fields, $user_id = NULL) {
    $url = $this->host . $this->restServicePath . "/biblio/$id/items?reserves=1";
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'GET',
    );
    $result = $this->httpRequest($url, $options, TRUE);
    if (!$result) {
      return array('avail' => 0, 'total' => 0, 'items' => array());
    }

    $items = json_decode($result->data);

    if ($user_id) {
      $url = $this->host . $this->restServicePath .
        "/biblio/$id/items_holdable_status?borrowernumber=" . $user_id;
      $options = array(
        'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
        'method' => 'GET',
      );
      $result = $this->httpRequest($url, $options, TRUE);
      if ($result) {
        $holdable = json_decode($result->data, TRUE);
      }
    }

    $available_fields = $this->itemFields();
    // Must we have to parse more_subfields_xml ?
    $parse_more_subfields_xml = FALSE;
    foreach ($wanted_fields as $field) {
      if (isset($available_fields[$field]['subfield'])) {
        $parse_more_subfields_xml = TRUE;
        break;
      }
    }

    $avail_items_count = 0;
    $total_items_count = 0;
    $items_array = array();
    foreach ($items as $item) {
      $itemnumber = (int) $item->itemnumber;
      $items_array[$itemnumber] = array();

      $item_info_subfields = NULL;
      if ($parse_more_subfields_xml && $item->more_subfields_xml) {
        $more_subfields_xml = @simplexml_load_string($item->more_subfields_xml);
        $item_info_marc = $this->parseMarcSubfields($more_subfields_xml->record);
        // more_subfields_xml never contains more than one 999 field.
        $item_info_subfields = $item_info_marc['datafields']['999']['0']['subfields'];
      }

      foreach ($wanted_fields as $field) {
        if (isset($available_fields[$field]['subfield'])) {
          $subfield = $available_fields[$field]['subfield'];
          if (isset($item_info_subfields) && isset($item_info_subfields[$subfield])) {
            // Subfields are never repeated in more_subfields_xml.
            $items_array[$itemnumber][$field] = (string) $item_info_subfields[$subfield][0];
          }
          else {
            $items_array[$itemnumber][$field] = '';
          }
        }
        else {
          $items_array[$itemnumber][$field] = (string) $item->{$field};
        }
      }

      // Returns date_due as timestamp.
      if (isset($items_array[$itemnumber]['date_due'])) {
        $items_array[$itemnumber]['date_due'] = strtotime($items_array[$itemnumber]['date_due']);
      }

      $items_array[$itemnumber]['canhold'] = (isset($holdable) && $holdable[$itemnumber]['is_holdable']) ? TRUE : FALSE;

      $item_waiting = FALSE;
      foreach ($item->reserves as $reserve) {
        if ($reserve->priority == 0 && $reserve->found == 'W') {
          $item_waiting = TRUE;
          break;
        }
      }

      if (!$item->onloan && !$item_waiting) {
        $items_array[$itemnumber]['avail'] = TRUE;
        $avail_items_count++;
      }
      else {
        $items_array[$itemnumber]['avail'] = FALSE;
      }
      $total_items_count++;
    }
    $status = array(
      'avail' => $avail_items_count,
      'total' => $total_items_count,
      'items' => $items_array,
    );
    return $status;
  }

  /**
   * Checks patron information.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @param array $wanted_fields
   *   Fields list we want to retrieve.
   *
   * @return array
   *   Patron information with wanted fields
   */
  public function patronInfo($patron_id, $wanted_fields) {
    $url = $this->host . $this->servicePath . '?service=GetPatronInfo&patron_id=' . $patron_id;
    $xml = @simplexml_load_file($url);

    $patron = array();
    foreach ($wanted_fields as $field) {
      $patron[$field] = (string) $xml->{$field};
    }

    return $patron;
  }

  /**
   * Checks patron checkouts information.
   *
   * date and duedate Fields must be a timestamp.
   *
   * @param string $user_id
   *   Patron username.
   *
   * @return array
   *   Checkouts list or FALSE.
   */
  public function patronCheckouts($user_id) {
    if (!$user_id) {
      return FALSE;
    }

    $url = $this->host . $this->restServicePath . "/user/byid/$user_id/issues";
    $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
    $result = drupal_http_request($url, array(
      'headers' => $headers,
      'method' => 'GET',
    ));
    if ($result->code != 200) {
      $error = t('Error retrieving patron checkouts.', array(), array('context' => 'libraries'));
      drupal_set_message(check_plain($error), 'error');
      return array();
    }

    $issues = json_decode($result->data);
    $checkouts = array();
    foreach ($issues as $issue) {
      // Make dates fields a timestamp.
      $date = strtotime($issue->issuedate);
      $duedate = strtotime($issue->date_due);

      $checkout = array(
        // Those fields are required by OPAC module but we don't want them to
        // be displayable, so they are here instead of in patronCheckoutsFields.
        'recordid' => $issue->biblionumber,
        'itemid' => $issue->itemnumber,
        'renewable' => $issue->renewable,
      );
      if (!$issue->renewable && property_exists($issue, 'reasons_not_renewable')) {
        switch ($issue->reasons_not_renewable) {
          case 'on_reserve':
            $checkout['reasons_not_renewable'] = t('Item is on hold', array(), array('context' => 'libraries'));
            break;

          case 'too_many':
            $checkout['reasons_not_renewable'] = t('Too many renewals', array(), array('context' => 'libraries'));
            break;

          default:
            $checkout['reasons_not_renewable'] = t("Unknown");

        }
      }
      foreach ($this->patronCheckoutsFields() as $key => $field) {
        if (array_key_exists('remote_field', $field) && property_exists($issue, $field['remote_field'])) {
          if (array_key_exists('type', $field) && $field['type'] == 'date') {
            $checkout[$key] = strtotime($issue->{$field['remote_field']});
          }
          else {
            $checkout[$key] = $issue->{$field['remote_field']};
          }
        }
      }
      $checkouts[] = $checkout;
    }
    return $checkouts;
  }

  /**
   * Checks patron holds information.
   *
   * @param string $user_id
   *   Patron id.
   *
   * @return array
   *   Holds list or FALSE.
   */
  public function patronHolds($user_id) {
    if (!$user_id) {
      return FALSE;
    }
    $url = $this->host . $this->restServicePath . "/user/byid/$user_id/holds";
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'GET',
    );
    $result = $this->httpRequest($url, $options, TRUE);
    if (!$result) {
      return array();
    }
    $holds = json_decode($result->data);

    $response = array();
    foreach ($holds as $hold) {
      $r = array(
        // Those fields are required by OPAC module but we don't want them to
        // be displayable, so they are here instead of in patronHoldsFields.
        'recordid' => $hold->biblionumber,
        'cancelid' => $hold->hold_id,
        'status' => t('On hold', array(), array('context' => 'libraries')),
      );
      if ($hold->found && (string) $hold->found == "W") {
        $r['status'] = t('Waiting to be pulled', array(), array('context' => 'libraries'));
      }
      foreach ($this->patronHoldsFields() as $key => $field) {
        if (array_key_exists('remote_field', $field) && property_exists($hold, $field['remote_field'])) {
          if (array_key_exists('type', $field) && $field['type'] == 'date') {
            $r[$key] = strtotime($hold->{$field['remote_field']});
          }
          else {
            $r[$key] = $hold->{$field['remote_field']};
          }
        }
      }
      $response[] = $r;
    }
    return $response;
  }

  /**
   * Performs a loan renewal.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @param string $itemid
   *   Item identifier to renew.
   *
   * @return bool
   *   TRUE for success or FALSE
   */
  public function renewItem($patron_id, $itemid) {
    $result = array();
    if (!$patron_id || !$itemid) {
      $result['success'] = 0;
      $result['message'] = t('Bad patron or item identifier', array(), array('context' => 'libraries'));
      return $result;
    }

    $url = $this->host . $this->servicePath . '?service=RenewLoan&patron_id=' . $patron_id . '&item_id=' . $itemid;
    $xml = @simplexml_load_file($url);

    if ($xml->error) {
      $result['success'] = 0;
      $result['message'] = (string) $xml->error;
    }
    $result['success'] = 1;

    return $result;
  }

  /**
   * Cancel holds for a given patron.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @param string $itemid
   *   Item identifier for which cancel hold.
   *
   * @return bool
   *   TRUE for success or FALSE.
   */
  public function cancelHold($patron_id, $itemid) {
    if (!$patron_id || !$itemid) {
      return FALSE;
    }

    $url = $this->host . $this->servicePath . '?service=CancelHold&patron_id=' . $patron_id . '&item_id=' . $itemid;
    $xml = @simplexml_load_file($url);

    if ($xml->code == 'Canceled') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Return a form to place an hold.
   *
   * Form is configurable in koha.conf file in the Patron hold form section.
   * Must not provide any submit button.
   *
   * @param string $patron_id
   *   Patron identifier (required).
   *
   * @param array $record
   *   Contains recordid, record_title and nid (required).
   *
   * @param string $itemid
   *   Item identifier we want to place an hold (optional).
   *
   * @return array
   *   A drupal form.
   */
  public function placeHoldForm($patron_id, $record, $itemid = NULL) {
    $form = array();
    $conf = $this->getConfig('patronHoldForm');

    if ($conf['pickup_location']['enabled'] == 1) {
      $url = $this->host . $this->restServicePath . "/branches";
      $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
      $result = drupal_http_request($url, array(
        'headers' => $headers,
        'method' => 'GET',
      ));

      // Build branch options.
      $branch_options = array('none' => t('None'));
      if ($result->code != 200) {
        $error = t('Error retrieving branches.', array(), array('context' => 'libraries'));
        drupal_set_message(check_plain($error), 'error');
      }
      else {
        $branches = json_decode($result->data);
        foreach ($branches as $branch) {
          $branch_options[$branch->code] = $branch->name;
        }
      }

      $form['pickup_location'] = array(
        '#type' => 'select',
        '#title' => t('Pickup location', array(), array('context' => 'libraries')),
        '#options' => $branch_options,
        '#default_value' => 'none',
        '#description' => t('Set this to <em>None</em> to let your library choose the pickup location.'),
      );
    }
    return $form;
  }

  /**
   * Place hold for a given patron.
   *
   * @param array $values
   *   The form submitted values containing:
   *    - opacuserid: Patron identifier (required),
   *    - recordid: record identifier to hold (required),
   *    - itemid: item identifier to hold (optional),
   *
   * @return array
   *   success key and message key.
   */
  public function placeHold($values) {
    if (!isset($values['opacuserid']) || !isset($values['recordid'])) {
      return array(
        'success' => 0,
        'message' => t('Missing elements'),
      );
    }

    $url = $this->host . $this->servicePath;
    if (isset($values['itemid']) && $values['itemid']) {
      $url .= '?service=HoldItem&patron_id=' . $values['opacuserid'] . '&bib_id=' . $values['recordid'];
      $url .= '&item_id=' . $values['itemid'];
    }
    else {
      $url .= '?service=HoldTitle&patron_id=' . $values['opacuserid'] . '&bib_id=' . $values['recordid'];
      $url .= '&request_location=127.0.0.1';
    }

    if (isset($values['pickup_location']) && $values['pickup_location'] != 'none') {
      $url .= '&pickup_location=' . $values['pickup_location'];
    }
    $xml = @simplexml_load_file($url);
    $result = array();
    if (isset($xml->pickup_location)) {
      $pickup = (string) $xml->pickup_location;
      $result['success'] = 1;
      $result['message'] = t('Items should be picked up at @pickup.', array('@pickup' => $pickup), array('context' => 'libraries'));
    }
    else {
      $result['success'] = 0;
      $result['message'] = t('Unable to hold this item. @url', array('@url' => $url), array('context' => 'libraries'));
    }
    return $result;
  }

  /**
   * Check if a biblio is holdable by a patron.
   *
   * @param int $record_id
   *   ILS identifier of record
   *
   * @param int $patron_id
   *   ILS identifier of patron
   *
   * @return int
   *   TRUE if biblio is holdable, FALSE otherwise.
   */
  public function biblioIsHoldable($record_id, $patron_id) {
    $url = $this->host . $this->restServicePath . "/biblio/$record_id/holdable?borrowernumber=$patron_id";
    $options = array(
      'method' => 'GET',
    );
    $result = $this->httpRequest($url, $options, TRUE);
    if ($result) {
      $response = json_decode($result->data);
      return $response->is_holdable;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Return a registration form.
   *
   * @param object $user
   *   Drupal user object.
   *
   * @return array.
   *   a drupal form.
   */
  public function patronRegistrationForm($user) {
    $form = array();

    $form['patron_identity'] = array(
      '#type' => 'fieldset',
      '#title' => t('Patron identity'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['patron_identity']['opac_surname'] = array(
      '#type' => 'textfield',
      '#title' => t('Surname'),
      '#default_value' => '',
      '#size' => 25,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
    $form['patron_identity']['opac_firstname'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#default_value' => '',
      '#size' => 25,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
    $form['patron_identity']['birth_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Date of birth'),
      '#date_format' => 'd/m/Y',
      '#default_value' => '',
    );
    $form['patron_contact'] = array(
      '#type' => 'fieldset',
      '#title' => t('Address / Contact'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['patron_contact']['opac_street_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Street number'),
      '#default_value' => '',
      '#size' => 5,
      '#required' => TRUE,
    );
    $form['patron_contact']['opac_address'] = array(
      '#type' => 'textfield',
      '#title' => t('Address'),
      '#default_value' => '',
      '#size' => 60,
      '#required' => TRUE,
    );
    $form['patron_contact']['opac_zip'] = array(
      '#type' => 'textfield',
      '#title' => t('Zip/Postal code'),
      '#default_value' => '',
      '#size' => 10,
      '#required' => TRUE,
    );
    $form['patron_contact']['opac_city'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#default_value' => '',
      '#size' => 10,
      '#required' => TRUE,
    );
    $form['patron_contact']['opac_mail'] = array(
      '#type' => 'textfield',
      '#title' => t('email'),
      '#default_value' => $user->mail,
      '#size' => 35,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
    $form['opac_login'] = array(
      '#type' => 'textfield',
      '#title' => t('Login'),
      '#default_value' => $user->name,
      '#size' => 20,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
    $form['opac_pass'] = array(
      '#type' => 'password_confirm',
      '#size' => 25,
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Implements OpacConnector::patronIssuesHistory().
   */
  public function patronIssuesHistory($patron_id) {
    $response = array();

    $url = $this->host . '/' . $this->restServicePath . "/user/byid/$patron_id/issues_history";
    $result = $this->httpRequest($url, array(), TRUE);
    if ($result) {
      $issues = json_decode($result->data);
      foreach ($issues as $issue) {
        $r = array();
        $r['recordid'] = $issue->biblionumber;
        foreach ($this->patronIssuesHistoryFields() as $key => $field) {
          if (array_key_exists('remote_field', $field) && property_exists($issue, $field['remote_field'])) {
            if (array_key_exists('type', $field) && $field['type'] == 'date') {
              $r[$key] = strtotime($issue->{$field['remote_field']});
            }
            else {
              $r[$key] = $issue->{$field['remote_field']};
            }
          }
        }
        $response[] = $r;
      }
    }

    return $response;
  }

  /**
   * Return purchase suggestions.
   */
  public function getSuggestions($patron_id) {
    $response = array();

    $url = $this->host . '/' . $this->restServicePath . "/suggestions?suggestedby=$patron_id";
    $result = $this->httpRequest($url, array(), TRUE);
    if ($result) {
      $suggestions = json_decode($result->data);
      foreach ($suggestions as $suggestion) {
        $r = array();
        $r['suggestionid'] = $suggestion->suggestionid;
        $r['recordid'] = $suggestion->biblionumber;
        $r['title'] = $suggestion->title;
        $r['author'] = $suggestion->author;
        $r['publication_year'] = $suggestion->publicationyear;
        $r['library'] = $suggestion->branchcode;
        $r['date'] = $suggestion->date;
        $r['notes'] = $suggestion->note;
        $r['status'] = $suggestion->STATUS;
        $r['reason'] = $suggestion->reason;
        $response[] = $r;
      }
    }

    return $response;
  }

  /**
   * Delete an existing suggestion.
   */
  public function deleteSuggestion($suggestionid) {
    $url = $this->host . '/' . $this->restServicePath . "/suggestions/$suggestionid";
    $options = array(
      'method' => 'DELETE',
    );
    $result = $this->httpRequest($url, $options, TRUE);
    if ($result) {
      $response = json_decode($result->data);
    }

    if (isset($response->success) && $response->success) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Retrieves Rest Service Path.
   */
  public function getRestServicePath() {
    return $this->restServicePath;
  }

  /**
   * Retrieves patron_id for a given cardnumber.
   *
   * @param string $cardnum
   *   Patron card number.
   *
   * @return string.
   *   Patrin identifier.
   */
  protected function getPatronId($cardnum) {
    $url = $this->host . $this->servicePath . '?service=LookupPatron&id=' . $cardnum . '&id_type=cardnumber';
    $xml = @simplexml_load_file($url);

    return $xml->id;
  }

  /**
   * Create a purchase suggestion in ILS by webservice.
   *
   * @param array $values
   *   Everything needed for a suggestion creation as a key/value array.
   */
  public function createSuggestion($values) {
    $data = array(
      'suggestedby' => isset($_SESSION['opac_user']['opac_user_id']) ? $_SESSION['opac_user']['opac_user_id'] : 0,
      'suggesteddate' => date('Y-m-d'),
      'STATUS' => 'ASKED',
      'branchcode' => isset($values['opac_suggestion_branchcode']) ? $values['opac_suggestion_branchcode'] : '',
      'title' => $values['opac_suggestion_title'],
      'author' => $values['opac_suggestion_author'],
      'publicationyear' => $values['opac_suggestion_publicationyear'],
      'isbn' => $values['opac_suggestion_isbn'],
      'publishercode' => $values['opac_suggestion_publisher'],
      'collectiontitle' => $values['opac_suggestion_collection'],
      'place' => $values['opac_suggestion_publisherplace'],
      'itemtype' => isset($values['opac_suggestion_doctype']) ? $values['opac_suggestion_doctype'] : '',
      'note' => $values['opac_suggestion_note'],
    );

    $url = $this->host . "/" . $this->restServicePath . "/suggestions";
    $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
    $query = "data=" . urlencode(drupal_json_encode($data));
    $options = array(
      'headers' => $headers,
      'method' => 'POST',
      'data' => $query,
    );

    $result = $this->httpRequest($url, $options, TRUE);

    // Did it work?
    $data = drupal_json_decode($result->data);
    if (isset($data['suggestionid'])) {
      return 'success';
    }
    else {
      return $data['error'];
    }
  }

  /**
   * Parse marc subfields.
   *
   * @param array $marcxml
   *   Marc tags and subfields values.
   *
   * @return array
   *   An array with marc subfields
   */
  protected function parseMarcSubfields($marcxml) {
    $marc = array();

    // Leader.
    $marc['leader'] = (string) $marcxml->leader;

    // Control field.
    foreach ($marcxml->controlfield as $controlfield) {
      $tag = (string) $controlfield->attributes()->tag;
      $data = (string) $controlfield;
      $marc['controlfields'][$tag] = $data;
    }

    // Data field.
    foreach ($marcxml->datafield as $datafield) {
      $tag = (string) $datafield->attributes()->tag;
      $subfields = array();
      foreach ($datafield->subfield as $subfield) {
        $code = trim((string) $subfield->attributes()->code);
        $data = trim((string) $subfield);
        $subfields[][$code] = $data;
      }
      $marc['datafields'][$tag][] = array('subfields' => $subfields);
    }
    return $marc;
  }


  /**
   * Return values for a given tag and (optionally) subfield code.
   *
   * @param array $get
   *   The field configuration
   *
   * @param array $from
   *   Marc data of the current record.
   *
   * @return array
   *   an array with marc subfields values
   */
  protected function get($get, $from) {
    $values = array();
    list($tag, $subfield) = array_pad(explode('$', $get['value']), 2, NULL);

    if (isset($from['controlfields'][$tag])) {
      // Return control field.
      return array(0 => $from['controlfields'][$tag]);
    }

    if (!isset($from['datafields'][$tag])) {
      // Return empty array.
      return $values;
    }

    // Data field.
    $values = $from['datafields'][$tag];

    $tmp = array();
    foreach ($values as $field) {
      if (isset($field['subfields'])) {
        foreach ($field['subfields'] as $subf) {
          foreach ($subf as $key => $value) {
            // If we are dealing with a subfield.
            if ($subfield) {
              // We only add it if it is this one.
              if ("$key" == "$subfield") {
                $tmp[] = $value;
              }
            }
            else {
              // If we are dealing with a field, we add every subfield.
              $tmp[] = $value;
            }
          }
        }
      }
    }
    $values = $tmp;
    return $values;
  }

  /**
   * Parse a date.
   *
   * @param string $date
   *   Date string.
   *
   * @return string
   *   A timestamp.
   */
  protected function dateToTimestamp($date) {
    $reg = "/([0-9].*)-([0-9].*)-([0-9].*)/";
    preg_match_all($reg, $date, $matches);
    $time = mktime(0, 0, 0, $matches[2][0], $matches[3][0], $matches[1][0]);
    return $time;
  }

  /**
   * Make a HTTP request with some error checks.
   *
   * @param string $url
   *   URL to fetch.
   *
   * @param array $options
   *   Options to pass to drupal_http_request().
   *
   * @param bool $show_errors
   *   If TRUE, prints error messages with drupal_set_message().
   *
   * @return object
   *   Object returned by drupal_http_request() if there is no errors.
   *   NULL otherwise.
   */
  protected function httpRequest($url, $options, $show_errors = FALSE) {
    $result = drupal_http_request($url, $options);
    if ($result->code != 200) {
      if ($show_errors) {
        drupal_set_message(t('@url returned bad status', array('@url' => $url))
          . ': ' . $result->code . ' ' . $result->error, 'error', FALSE);
      }
      return NULL;
    }

    return $result;
  }
}
