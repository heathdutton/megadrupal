<?php
/**
 * @file
 * Check functions and functions to parse the server response for the
 * Yandex.Webmaster module.
 */

/**
 * Returns Yandex.Webmaster domain name.
 */
function yandex_webmaster_domain_name($subdomain = 'webmaster') {
  $domain = variable_get('yandex_webmaster_domain', 'com');
  return 'https://' . $subdomain . '.yandex.' . $domain . '/';
}

/**
 * Helper function to make authorized GET requests.
 */
function yandex_webmaster_http_request($url) {
  $options = array(
    'method' => 'GET',
    'headers' => array('Authorization' => 'OAuth ' . yandex_services_auth_info()),
  );
  $result = drupal_http_request($url, $options);
  if ($result->code == 200) {
    return $result->data;
  }
  else {
    watchdog('yandex_webmaster', 'Yandex.Webmaster request to %url failed with code %code and error message "%error".',
      array('%url' => $url, '%code' => $result->code, '%error' => isset($result->error) ? $result->error : 'empty'), WATCHDOG_WARNING);
  }
  return FALSE;
}

/**
 * Returns URL of hosts list resource.
 * http://api.yandex.com/webmaster/doc/dg/tasks/how-to-get-service-document.xml
 * https://tech.yandex.ru/webmaster/doc/dg/tasks/how-to-get-service-document-docpage/
 */
function yandex_webmaster_hosts_list_resource() {
  $url = drupal_static(__FUNCTION__, NULL);
  if (!isset($url)) {
    $url = FALSE;
    if ($data = yandex_webmaster_http_request(yandex_webmaster_domain_name() . 'api/v2')) {
      $service = yandex_webmaster_parse_xml($data);
      $url = current($service->workspace->collection->link->attributes()->href);
    }
  }
  return $url;
}

/**
 * Get Yandex.API hosts list.
 * http://api.yandex.com/webmaster/doc/dg/reference/hosts.xml
 * https://tech.yandex.ru/webmaster/doc/dg/reference/hosts-docpage/
 */
function yandex_webmaster_get_hosts($reset = FALSE) {
  if (!$reset) {
    $cache = cache_get('yandex_webmaster_hosts_list');
    if (!empty($cache->data)) {
      $list = $cache->data;
      return $list;
    }
  }

  $list = array();
  if ($data = yandex_webmaster_http_request(yandex_webmaster_hosts_list_resource())) {
    $xml = yandex_webmaster_parse_xml($data);
    $hosts = $xml->xpath('//hostlist/host');
    foreach ($hosts as $host) {
      // Some values can be empty if host is not in Yandex index.
      $href = current($host->attributes()->href);
      $components = explode('/', $href);
      $host_id = array_pop($components);
      $list[$host_id]['href'] = $href;
      $list[$host_id]['name'] = current($host->name);
      $list[$host_id]['verification'] = current($host->verification->attributes()->state);
      $list[$host_id]['crawling'] = isset($host->crawling) ? current($host->crawling->attributes()->state) : '';
      $list[$host_id]['virused'] = isset($host->virused) ? current($host->virused) : 'false';
      $list[$host_id]['last-access'] = isset($host->{'last-access'}) ? strtotime(current($host->{'last-access'})) : t('Never');
      $list[$host_id]['tcy'] = isset($host->tcy) ? current($host->tcy) : '';
      $list[$host_id]['url-count'] = isset($host->{'url-count'}) ? current($host->{'url-count'}) : '';
      $list[$host_id]['index-count'] = isset($host->{'index-count'}) ? current($host->{'index-count'}) : '';
    }
  }

  cache_set('yandex_webmaster_hosts_list', $list, 'cache', CACHE_TEMPORARY);
  return $list;
}

/**
 * Get current host ID.
 */
function yandex_webmaster_get_host_id() {
  $hosts = yandex_webmaster_get_hosts();
  $current_host = yandex_webmaster_idna_convert_operation($_SERVER['HTTP_HOST'], 'decode');
  foreach ($hosts as $host_id => $host) {
    // Check for same hostnames and hostnames without 'www' prefix.
    if ($host['name'] == $current_host || str_replace('www.', '', $current_host) == str_replace('www.', '', $host['name'])) {
      return $host_id;
    }
  }
  return -1;
}

/**
 * Returns URLs of host resources.
 * http://api.yandex.com/webmaster/doc/dg/reference/hosts-id.xml
 * https://tech.yandex.ru/webmaster/doc/dg/reference/hosts-id-docpage/
 */
function yandex_webmaster_host_resources($host_id, $resource_type = NULL) {
  $resources = drupal_static(__FUNCTION__, NULL);
  if (!isset($resources[$host_id])) {
    $hosts = yandex_webmaster_get_hosts();
    if (!isset($hosts[$host_id])) {
      return FALSE;
    }
    if ($data = yandex_webmaster_http_request($hosts[$host_id]['href'])) {
      $xml = yandex_webmaster_parse_xml($data);
      $links = $xml->xpath('//host/link');
      foreach ($links as $link) {
        $resources[$host_id][current($link->attributes()->rel)] = current($link->attributes()->href);
      }
    }
  }

  if (isset($resource_type)) {
    return (isset($resources[$host_id][$resource_type]) ? $resources[$host_id][$resource_type] : FALSE);
  }
  return $resources[$host_id];
}

/**
 * Get Yandex.Webmaster host uin.
 *
 * @param $host_id
 *   Yandex.API host ID.
 *
 * @return string|boolean
 *   Yandex.Webmaster host uin.
 */
function yandex_webmaster_get_host_uin($host_id) {
  if ($data = yandex_webmaster_http_request(yandex_webmaster_host_resources($host_id, 'verify-host'))) {
    preg_match("/<uin>(.*?)<\/uin>/", $data, $matches);
    if (isset($matches[1]) && $matches[1] != '') {
      variable_set('yandex_webmaster_uin', $matches[1]);
      return $matches[1];
    }
  }
  return FALSE;
}

/**
 * Get Yandex.API host stats.
 *
 * @param $host_id
 *   Yandex.API host ID.
 *
 * @return array
 *   Host stats.
 */
function yandex_webmaster_get_host_stats($host_id = 0) {
  $stats = array();
  if ($host_id == 0) {
    $host_id = variable_get('yandex_webmaster_basic_host_id', 0);
    if ($host_id == 0) {
      $stats['error'] = t('Basic host was not selected.');
      return $stats;
    }
  }

  if ($data = yandex_webmaster_http_request(yandex_webmaster_host_resources($host_id, 'host-information'))) {
    $host = yandex_webmaster_parse_xml($data);
    // Some values can be empty if host is not in Yandex index.
    $stats['name'] = current($host->name);
    $stats['verification'] = current($host->verification->attributes()->state);
    $stats['verification_details'] = current($host->verification->details);
    $stats['crawling'] = isset($host->crawling) ? current($host->crawling->attributes()->state) : '';
    $stats['crawling_details'] = current($host->crawling->details);
    $stats['virused'] = current($host->virused);
    $stats['last-access'] = isset($host->{'last-access'}) ? strtotime(current($host->{'last-access'})) : t('Never');
    $stats['tcy'] = current($host->tcy);
    $stats['url-count'] = current($host->{'url-count'});
    $stats['url-errors'] = current($host->{'url-errors'});
    $stats['index-count'] = current($host->{'index-count'});
    $stats['internal-links-count'] = current($host->{'internal-links-count'});
    $stats['links-count'] = current($host->{'links-count'});
  }

  return $stats;
}

/**
 * Get default Yandex.API query.
 *
 * @param $query
 *   Query value, like 'tops' or 'indexed'.
 *
 * @param $host_id
 *   Yandex.API host ID.
 *
 * @return array
 *   Result array.
 */
function yandex_webmaster_get_query($query, $host_id = 0) {
  $stats = array();
  if ($host_id == 0) {
    $host_id = variable_get('yandex_webmaster_basic_host_id', 0);
    if ($host_id == 0) {
      $stats['error'] = t('Basic host was not selected.');
      return $stats;
    }
  }

  if ($data = yandex_webmaster_http_request(yandex_webmaster_host_resources($host_id, $query))) {
    $stats['result'] = $data;
  }

  return $stats;
}

/**
 * Return Yandex branding form element.
 *
 * @param $show_agreements_link
 *   Boolean value to show agreements link.
 *
 * @return array
 *   Form element.
 */
function yandex_webmaster_branding_item($show_agreements_link = FALSE) {
  global $language;
  $agreements = $show_agreements_link ? '<br/>' . l(t('Read user agreements'), 'http://legal.yandex.ru/webmaster_api/', array('attributes' => array('target' => '_blank'))) : '';
  return array(
    '#type' => 'item',
    '#markup' => '<div class="branding branding-' . $language->language . '">'
      . t('Data provided by !link service.', array(
        '!link' => l(t('Yandex.Webmaster'), yandex_webmaster_domain_name(), array(
          'attributes' => array(
            'target' => '_blank',
            'title' => t('Yandex.Webmaster'),
          ),
        ))
      ))
      . $agreements
      . '</div>',
    '#weight' => 100,
  );
}

/**
 * Return site selector form element.
 */
function yandex_webmaster_selector_item() {
  $hosts = yandex_webmaster_get_hosts();
  $options = array();
  foreach ($hosts as $host_id => $host) {
    if ($host['crawling'] == 'INDEXED') {
      $options[$host_id] = $host['name'];
    }
  }
  return array(
    '#tree' => TRUE,
    'options' => array(
      '#title' => t('Select site'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => variable_get('yandex_webmaster_basic_host_id', 0),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Update'),
    ),
  );
}

/**
 * Utility function to work with cyrillic domain names.
 */
function yandex_webmaster_idna_convert_operation($host, $operation = 'decode') {
  static $included;
  if (!isset($included) && _yandex_webmaster_idna_convert_path()) {
    // Load idna_convert class for correct work with domain-names in punycode.
    include_once _yandex_webmaster_idna_convert_path();
    $included = TRUE;
  }
  else {
    $included = FALSE;
  }

  if ($included) {
    $idna = new idna_convert();
    $host = $idna->$operation($host);
  }
  return $host;
}

/**
 * Returns the path of idna_convert class.
 */
function _yandex_webmaster_idna_convert_path() {
  if (module_exists('libraries') && file_exists(libraries_get_path('idna_convert') . '/idna_convert.class.php')) {
    return libraries_get_path('idna_convert') . '/idna_convert.class.php';
  }
  elseif (file_exists('sites/all/libraries/idna_convert/idna_convert.class.php')) {
    return 'sites/all/libraries/idna_convert/idna_convert.class.php';
  }
  return FALSE;
}

/**
 * Utility function to safely parse XML.
 */
function yandex_webmaster_parse_xml($xmlstring) {
  $xml_internal_errors = libxml_use_internal_errors(TRUE);
  // We should handle exceptions here as the above error handling doesn't help with it.
  try {
    $data = new SimpleXMLElement($xmlstring);
  }
  catch (Exception $e) {
    watchdog_exception('yandex_webmaster', $e);
    $data = FALSE;
  }

  if ($errors = libxml_get_errors()) {
    foreach ($errors as $error) {
      watchdog('yandex_webmaster', 'Error parsing xml string %xmlstring with code %code and message %message.',
        array('%xmlstring' => $xmlstring, '%code' => $error->code, '%message' => $error->message), WATCHDOG_WARNING);
    }
    $data = FALSE;
  }

  libxml_clear_errors();
  libxml_use_internal_errors($xml_internal_errors);
  return $data;
}

/**
 * Get Yandex.API status description.
 *
 * @param $state
 *   Yandex.API state code.
 *
 * @param $context
 *   State code context for the correct interpretation of the states with
 *   the same code.
 *
 * @return string
 *   Full description of Yandex.API state code.
 */
function yandex_webmaster_state_description($state, $context = '') {
  switch ($state) {
    // Verification state.
    case 'IN_PROGRESS':
      return t('Verification in progress.');
    case 'NEVER_VERIFIED':
      return t('The site was never verified.');
    case 'VERIFICATION_FAILED':
      return t('Site verification has failed.');
    case 'VERIFIED':
      return t('Verified.');
    case 'WAITING':
      if ($context == 'verification') {
        return t('The site is in queue for verification.');
      }
      elseif ($context == 'index') {
        return t('The site expects the index.');
      }
      else {
        return t('The site is in queue.');
      }
    // Indexing state.
    case 'INDEXED':
      return t('Site was indexed.');
    case 'NOT_INDEXED':
      return t('Site was not indexed.');
    // Verification types.
    case 'AUTO':
      return t('Automatic verification.');
    case 'DNS_RECORD':
      return t('Adding TXT parameter in DNS-record of the site.');
    case 'HTML_FILE':
      return t('HTML file in the site root directory.');
    case 'MANUAL':
      return t('Manual verification.');
    case 'META_TAG':
      return t('Meta-tag in header of the front page.');
    case 'PDD':
      return t('Verification by Yandex.Mail for domains.');
    case 'TXT_FILE':
      return t('Text file in the site root directory.');
    case 'PDD_EXTERNAL':
      return t('Verification by external Yandex.Mail for domains.');
    case 'DELEGATION':
      return t('Access rights was delegated from another user.');
    case 'WHOIS':
      return t('Verification by information from the WHOIS service.');
    default:
      return $state;
  }
}

/**
 * Get Yandex.API code description.
 *
 * @param $state
 *   Yandex.API code.
 *
 * @return string
 *   Full description of Yandex.API code.
 */
function yandex_webmaster_code_description($code) {
  switch ($code) {
    // HTTP-code (redirect).
    case '300':
      return t('HTTP Status: Multiple Choices (300)');
    case '301':
      return t('HTTP Status: Moved Permanently (301)');
    case '302':
      return t('HTTP Status: Found (Moved Temporarily) (302)');
    case '303':
      return t('HTTP Status: See Other (303)');
    case '304':
      return t('HTTP Status: Not Modified (304)');
    case '305':
      return t('HTTP Status: Use Proxy (305)');
    case '307':
      return t('HTTP Status: Temporary Redirect (307)');
    // HTTP-code (client).
    case '400':
      return t('HTTP Status: Bad Request (400)');
    case '401':
      return t('HTTP Status: Unauthorized (401)');
    case '402':
      return t('HTTP Status: Payment Required (402)');
    case '403':
      return t('HTTP Status: Forbidden (403)');
    case '404':
      return t('HTTP Status: File Not Found (404)');
    case '405':
      return t('HTTP Status: Method Not Allowed (405)');
    case '406':
      return t('HTTP Status: Not Acceptable (406)');
    case '407':
      return t('HTTP Status: Proxy Authentication Required (407)');
    case '408':
      return t('HTTP Status: Request Timeout (408)');
    case '409':
      return t('HTTP Status: Conflict (409)');
    case '410':
      return t('HTTP Status: Gone (410)');
    case '411':
      return t('HTTP Status: Length Required  (411)');
    case '412':
      return t('HTTP Status: Precondition Failed (412)');
    case '413':
      return t('HTTP Status: Request Entity Too Large (413)');
    case '414':
      return t('HTTP Status: Request-URL Too Long (414)');
    case '415':
      return t('HTTP Status: Unsupported Media Type (415)');
    case '416':
      return t('HTTP Status: Requested Range Not Satisfiable (416)');
    case '417':
      return t('HTTP Status: Expectation Failed (417)');
    case '422':
      return t('HTTP Status: Unprocessable Entity (422)');
    case '423':
      return t('HTTP Status: Locked (423)');
    case '424':
      return t('HTTP Status: Failed Dependency (424)');
    case '426':
      return t('HTTP Status: Upgrade Required (426)');
    // HTTP-code (server).
    case '500':
      return t('HTTP Status: Internal Server Error (500)');
    case '501':
      return t('HTTP Status: Not Implemented (501)');
    case '502':
      return t('HTTP Status: Bad Gateway (502)');
    case '503':
      return t('HTTP Status: Bad Gateway (503)');
    case '504':
      return t('HTTP Status: Gateway Timeout (504)');
    case '505':
      return t('HTTP Status: HTTP Version Not Supported (505)');
    case '507':
      return t('HTTP Status: Insufficient Space to Store Resource (507)');
    case '510':
      return t('HTTP Status: Not Extended (510)');
    // Page load errors.
    case '1001':
      return t('Connection was closed');
    case '1002':
      return t('Document is too large');
    case '1003':
      return t('Document was denied by robots.txt');
    case '1004':
      return t('Document address does not correspond to the HTTP standard');
    case '1005':
      return t('Unsupported document format');
    case '1006':
      return t('DNS error');
    case '1007':
      return t('Status code does not correspond to standard HTTP');
    case '1008':
      return t('Wrong HTTP header');
    case '1010':
      return t('Unable to connect to the server');
    case '1013':
      return t('Wrong message length');
    case '1014':
      return t('Wrong encoding');
    case '1019':
      return t('Wrong number of data was submitted');
    case '1020':
      return t('HTTP header length exceeds the limit');
    case '1021':
      return t('Address length exceeds the limit');
    // Page parsing errors.
    case '2004':
      return t('Document contains meta-tag "refresh"');
    case '2005':
      return t('Document contains meta-tag "noindex"');
    case '2006':
      return t('Wrong encoding');
    case '2007':
      return t('This document is the server log');
    case '2010':
      return t('Wrong document format');
    case '2011':
      return t('Unable to detect encoding');
    case '2012':
      return t('Unsupported language');
    case '2014':
      return t('Document doesn\'t contains text');
    case '2016':
      return t('Too many links');
    case '2020':
      return t('Error extracting');
    case '2024':
      return t('Empty server response');
    case '2025':
      return t('Document is not canonical');
    default:
      return t('Error code #') . $code;
  }
}
