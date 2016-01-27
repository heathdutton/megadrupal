<?php
/**
 * @file
 * Class to handle ISGweb SOAP requests and responses.
 */

class ISGwebWrapper {
  private $wsdlRoot;
  private $wsdlKeys;
  private $uiRoot;
  public  $errors;

  public function __construct(
    $wsdlRoot,
    $wsdlKeys = array(
      'Authentication' => NULL,
      'DataAccess' => NULL
    ),
    $uiRoot
  ) {
    $this->wsdlRoot = trim($wsdlRoot, '/') . '/';
    $this->wsdlKeys = $wsdlKeys;
    $this->uiRoot = trim($uiRoot, '/') . '/';
    $this->errors = array();
  }

  /**
   * Authentication web service methods
   */
  public function authenticateUser($username, $password) {
    $response = $this->call('Authentication', 'AuthenticateUser', array(
      'username' => $username,
      'password' => $password
    ));

    if ($response) {
      $response = $response['User']['@attributes'];

      // Authorize the issued token in ISGweb
      $response = $this->authenticateToken($response['TOKEN']);
    }

    return $response;
  }

  public function authenticateToken($token) {
    $response = $this->call('Authentication', 'AuthenticateToken', array(
      'token' => $token
    ));

    if (!empty($response)) {
      $response = $response['User']['@attributes'];
    }

    return $response;
  }

  public function deleteUserSession($token) {
    $response = $this->call('Authentication', 'DeleteUserSession', array(
      'token' => $token
    ));

    if (!empty($response)) {
      $response = TRUE;
    }

    return $response;
  }

  /**
   * DataAccess web service methods
   */
  public function executeStoredProcedure($sproc, $args) {
    $response = $this->call('DataAccess', 'ExecuteStoredProcedure', array(
      'name' => $sproc,
      'parameters' => $args
    ));

    return $response;
  }

  /**
   * UI helper methods
   */
  public function getLink($link_key, $token, $returnURL, $is_iframe = FALSE) {
    $links = array(
      'profile_view' => 'Profile/ViewProfile.aspx',
      'profile_edit' => 'Profile/EditProfile.aspx',
      'profile_register' => 'Profile/CreateNewUser.aspx',
      'profile_password' => 'LogIn/RetrievePassword.aspx',
      'iframe_js' => 'javascripts/iframe.js',
    );

    if (array_key_exists($link_key, $links)) {
      $format  = $this->uiRoot . $links[$link_key];
      $query  =
      $params  = array();

      if (!empty($token)) {
        $query[]  = 'Token=%s';
        $params[]  = $token;
      }
      if (!empty($returnURL))  {
        $query[]   = 'ReturnPage=%s';
        $params[]  = $returnURL;
      }
      if (!empty($query)) {
        $format .= '?' . implode('&', $query);
      }

      $path = vsprintf($format, $params);

      if ($is_iframe) {
        drupal_add_js($this->uiRoot . $links['iframe_js'], array(
          'type' => 'external',
          'scope' => 'header'
        ));

        return '<iframe src="' . $path . '" isgwebsite="1" name="ISGwebContainer" id="ISGwebContainer" marginwidth="1" marginheight="0" frameborder="0" vspace="0" hspace="0" scrolling="no" style="width:100%; height:800px;"></iframe>';
      }

      return $path;
    }

    return NULL;
  }

  /**
   * Internal utility methods
   */
  private function call($service, $method, $params) {
    ini_set('memory_limit', '1024M');
    ini_set('max_execution_time', 18000);
    ini_set('default_socket_timeout', ini_get('max_execution_time'));
    ini_set('soap.wsdl_cache_enabled', 1);
    ini_set('soap.wsdl_cache_dir', realpath(conf_path() . '/files'));
    ini_set('soap.wsdl_cache_ttl', 86400);
    drupal_set_time_limit(0);

    // Set SOAP request
    $params['securityPassword']   = $this->wsdlKeys[$service];
    $request                      = array('parameters' => $params);
    $response                     = NULL;

    // Get SOAP response
    try {
      $client = new SoapClient($this->wsdlRoot . $service . '.asmx?wsdl', array(
        'cache_wsdl' => WSDL_CACHE_DISK,
        'connection_timeout' => ini_get('max_execution_time'),
        'compression' => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
        'trace' => TRUE,
        'exceptions' => TRUE,
      ));

      $response = $client->__soapCall($method, $request);
      $response = (string) $client->__getLastResponse();

      // A note on why we're not using the $client->__soapCall() response:
      //   The PHP SOAP client was choking on the ~85K records that were
      //   being returned in the SOAP response. It was able to pull back
      //   the raw data, but failed consistently when there were more
      //   than a few thousand records involved. As such, we're directly
      //   processing the raw XML below.
    }
    catch (SoapFault $e1) {
      $this->errors[] = $e1->getMessage();
      return FALSE;
    }
    catch (Exception $e2) {
      $this->errors[] = $e2->getMessage();
      return FALSE;
    }

    // Process response
    $data = NULL;

    // Some ISGweb methods return strings instead of XML
    if (strpos($response, '<') == 0) {
      $values    =
      $map       = array();
      $parser    = xml_parser_create();

      // Change encoding string to UTF8, change root element case so
      // simplexml_load_string doesn't spit out "Extra content at the
      // end of the document" error
      $response  = preg_replace(array('/encoding="UTF-16"/i', '/iBridge/i'), array('encoding="UTF-8"', 'ibridge'), $response);

      libxml_use_internal_errors(TRUE);
      xml_parse_into_struct($parser, $response, $values, $map);
      xml_parser_free($parser);

      $map_ix  = $map[strtoupper($method . 'Result')][0];
      $obj     = trim($values[$map_ix]['value']);
      $obj     = simplexml_load_string($obj, 'SimpleXMLElement', LIBXML_ERR_FATAL | LIBXML_PARSEHUGE);
      $error   = libxml_get_errors();

      // Check for XML parsing errors
      if (!empty($error)) {
        foreach ($error as $e) {
          $this->errors[] = $e;
        }
        libxml_clear_errors();
        return FALSE;
      }

      $data = $this->objectToArray($obj);

      // Check for ISGweb errors (e.g. invalid data input,
      // failure of service, etc.)
      if (array_key_exists('Errors', $data)) {
        $error = $data['Errors'];
        foreach ($error as $e) {
          $this->errors[] = $e['@attributes']['Description'];
        }
        return FALSE;
      }
    }
    else {
      $data = $response;
    }

    return $data;
  }

  private function objectToArray($arrObjData, $arrSkipIndices = array()) {
    $arrData = array();

    // if input is object, convert into array
    if (is_object($arrObjData)) {
      $arrObjData = get_object_vars($arrObjData);
    }

    if (is_array($arrObjData)) {
      foreach ($arrObjData as $index => $value) {
        if (is_object($value) || is_array($value)) {
          $value = $this->objectToArray($value, $arrSkipIndices);
        }
        if (in_array($index, $arrSkipIndices)) {
          continue;
        }
        $arrData[$index] = $value;
      }
    }
    return $arrData;
  }

}

