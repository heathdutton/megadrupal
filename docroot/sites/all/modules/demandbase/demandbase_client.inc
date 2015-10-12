<?php

/**
 * @file
 * Serve data from demandbase.
 * Need demandbase key for retrieval of visitor data
 */

class demandbase_client {
  public  $dbkey; //JB 12/20/2013 - don't name it $key PHP thinks $this->key is a function
  public  $url;
  public  $result;
  public  $method;
  public  $ip_address;
  public  $fetch_field;
  public  $code;
  public  $data;
  public  $admin_ip_address;
  public  $session_ip_address;
  public  $activate_ip_address;
  public  $visitor_ip_address;
  public  $activate_visitor_ip_address;
  public  $activate_admin_ip_address;
  public  $error;
  public  $processed_data;
  public function __construct() {
    // Pick up default visitor ip address.
    $this->ip_address = ip_address();
    $this->visitor_ip_address = $this->ip_address;

    // IP address setting for Admin.
    $this->admin_ip_address = trim(variable_get('demandbase_admin_ip_address', '208.92.40.133'));
    $this->activate_admin_ip_address = intval(variable_get('demandbase_activate_admin_ip_address', '0'));

    //set demandbase session ip address
    $this->session_ip_address = (isset($_SESSION['demandbase_session_ip'])) ? $_SESSION['demandbase_session_ip'] : '';

    //set the url and key for demandbase api
    $this->url = trim(variable_get('demandbase_url', 'http://api.demandbase.com/api/v2/ip.json'));
    $this->dbkey = trim(variable_get('demandbase_key', ''));
    $this->method = 'POST';
    // Pick up admin specified ip address.
    if ($this->admin_ip_address != '' && $this->activate_admin_ip_address) {
      $this->ip_address = $this->admin_ip_address;
      //print "i am in admin condition";
    }

     // Pick up visitor specified ip address
    $this->activate_visitor_ip_address = intval(variable_get('demandbase_activate_visitor_ip_address', '0'));
    if ($this->session_ip_address != '' && $this->activate_visitor_ip_address) {
      $this->ip_address = $this->session_ip_address;
    }

  }

  public function __get($name) {
    return $this->$name;
  }

  public function __set($name, $value) {
    $this->$name = $value;
  }

  /**
   * Fetch data from the demandbase based on the ip address set or passed
   */
  public function processData($ip_address = '') {
    $ip = $this->ip_address;
    if (!empty($ip_address)) {
      $ip = $ip_address;
    }

    $options = array(
      // 'data' must be formatted as a query string,
      // see https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_http_request/7
      'data' => 'key='.$this->dbkey.'&query='.$ip,
      'timeout' => 5.0,
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded')
    );
    //echo '<pre>',$this->url,'</pre>';
    //echo '<pre>',$this->dbkey,'</pre>';
    //echo '<pre>',$this->dbkey,'</pre>';

    $result = drupal_http_request($this->url, $options); //JB 12/20/2013 - POST returns 'unauthorized' use GET instead (similar to JS method)
    $this->code = json_decode($result->code);
    //echo '<pre>REQUEST: ',$result->request,'</pre>';
    //echo '<pre>HEADERS: ',print_r(array_values($result->headers)),'</pre>';
    //echo '<pre>Status: ',$this->code,'</pre>';
    //echo '<pre>Data: ',$result->data,'</pre>';
    $data = array();
    if ($this->code == '200') {
      $data_object = json_decode($result->data);
      $this->data = $data_object; //save raw data to this->data
      if (is_array($data_object) || is_object($data_object)) {
       foreach ($data_object as $key => $value) {
        if (is_object($value)) {
          $child_data = $this->demanbase_process_object_data($value, $key);
          foreach ($child_data as $child_key => $child_value) {
           $data[$child_key] = $child_value;
         }
         continue;
       }
            // if (empty($value)) {
            //   continue;
            // }
       $data[$key] = $value;
     }
   }
    $this->processed_data = $data;// save flattened data to this->result
  }
  if ($this->code != '200') {
    $this->data = $result->data;//JB 12/20/2013 - $result->data is not always an json, sometimes it is a plaintext

    if(json_decode($result->data)) {//JB 12/20/2013 - If it is json, decode it and set it to $this->data
      $this->data = $result->data;
    }

    if(is_array($this->data) || is_object($this->data)) {
      foreach ($this->data as $key => $value) {
        $this->processed_data[$key] = $value;
      }
    } else {  //JB 12/20/2013 - If not json/array then just tell me the error message
      $this->processed_data['result_data'] = $this->data;
    }

    $this->processed_data['ip'] = $ip;
    $this->error = $result->error;  //JB 12/20/2013 - Same with $result->error, not always json

    if(json_decode($result->error)) {
      $this->data = $result->error;
    }
  }
}

  /**
   *  converting object to array
   */
  function demanbase_process_object_data($data, $mainkey) {
    $items = array();
    foreach ($data as $key => $value) {
      //$field_name = str_ireplace('_', ' ', $key);
      // if (empty($value)) {
      //   continue;
      // }
      $items[$mainkey . '_' . $key] =  $value;
    }
    return $items;
  }
  /**
   * Retrieve visitor information from demandbase
   * @param String $field
   *   return this field
   * @param String $ip
   *   process based on the ip information passed
   */
  public function fetch_data($field = 'ip', $ip = '') {
    $this->processed_data = array();
    $this->processData($ip);//send request to demandbase and process recieved
    if (isset($this->processed_data) && is_array($this->processed_data)) {
      return isset($this->processed_data[$field]) ? $this->processed_data[$field] : '';
    }
    return isset($this->processed_data[$field])? $this->processed_data[$field] : '';

  }


  /**
   * Match the subject against a set of regex patterns.
   * also handles negation through the use
   * of the ~ character.
   *
   * @param mixed $subject
   *   The subject string or an array of strings to be matched.
   * @param array $patterns
   *   An array of patterns. Any patterns that begin with ~ are considered
   *   negative or excluded conditions.
   * @param boolean $marketing_alias
   *   Whether the given subject should be matched as a Drupal marketing_alias.
   *   If TRUE,
   *   '<front>' will be replaced with the site frontpage when matching against
   *   $patterns.
   */
  public function match($subject, $patterns, $marketing_alias = FALSE) {
    static $regexps;
    $match = FALSE;
    $positives = $negatives = 0;
    $subject = !is_array($subject) ? array($subject) : $subject;
    foreach ($patterns as $pattern) {
      $pattern = drupal_strtolower($pattern);
      if (strpos($pattern, '~') === 0) {
        $negate = TRUE;
        $negatives++;
      }
      else {
        $negate = FALSE;
        $positives++;
      }
      $pattern = ltrim($pattern, '~');
      if (!isset($regexps[$pattern])) {
        if ($marketing_alias) {
          $regexps[$pattern] = '/^(' . preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/', '/(^|\|)\\\\<front\\\\>($|\|)/'), array('|', '.*', '\1' . preg_quote(variable_get('site_frontpage', 'node'), '/') . '\2'), preg_quote($pattern, '/')) . ')$/';
        }
        else {
          $regexps[$pattern] = '/^(' . preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/'), array('|', '.*'), preg_quote($pattern, '/')) . ')$/';
        }
      }
      foreach ($subject as $value) {
        if (preg_match($regexps[$pattern], $value)) {
          if ($negate) {
            return FALSE;
          }
          $match = TRUE;
        }
      }
    }
    // If there are **only** negative conditions and we've gotten this far none
    // we actually have a match.
    if ($positives === 0 && $negatives) {
      return TRUE;
    }
    return $match;
  }

}
