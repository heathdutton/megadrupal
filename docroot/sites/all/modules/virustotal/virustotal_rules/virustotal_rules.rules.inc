<?php

/**
 * @file
 * Data types, Actions and events of VirusTotal API Rules integration.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_data_info().
 *
 * Declares meta-data about data types for VirusTotal Rules.
 */
function virustotal_rules_rules_data_info() {
  return array(
    // Data type that represents the API response.
    'virustotal_response' => array(
      'label' => t('VirusTotal API response'),
      'group' => t('VirusTotal'),
      'property info' => _virustotal_rules_rules_response_info(),
      'wrap' => TRUE,
      'token type' => 'virustotal_response',
    ),
    // Data type used for scan reports to represent each AV result.
    'virustotal_scan_result' => array(
      'label' => t('VirusTotal AV result'),
      'group' => t('VirusTotal'),
      'wrap' => TRUE,
      'token type' => 'virustotal_scan_result',
      'property info' => _virustotal_rules_rules_scan_result_info(),
    ),
  );
}

/**
 * Defines property info for VirusTotal API responses.
 */
function _virustotal_rules_rules_response_info() {
  return array(
    'response_code' => array(
      'type' => 'decimal',
      'label' => t('Response status code'),
      'options list' => 'virustotal_rules_response_status_codes',
    ),
    'verbose_msg' => array(
      'type' => 'text',
      'label' => t('Versbose message regarding response code'),
    ),
    'resource' => array(
      'type' => 'text',
      'label' => t('VirusTotal resource ID (same as sha256)'),
    ),
    'scan_id' => array(
      'type' => 'text',
      'label' => t('Resource ID + unique scan id'),
    ),
    'permalink' => array(
      'type' => 'text',
      'label' => t('Permanent URL to scan report'),
    ),
    'md5' => array(
      'type' => 'text',
      'label' => t('MD5 hash of file sent'),
    ),
    'sha1' => array(
      'type' => 'text',
      'label' => t('SHA1 hash of file sent'),
    ),
    'sha256' => array(
      'type' => 'text',
      'label' => t('SHA256 hash of file sent'),
    ),
    'scan_date' => array(
      'type' => 'date',
      'label' => t('Date of scan'),
    ),
    'positives' => array(
      'type' => 'integer',
      'label' => t('Number of positive detections'),
    ),
    'total' => array(
      'type' => 'integer',
      'label' => t('Number of AV´s used'),
    ),
    'scans' => array(
      'type' => 'list<virustotal_scan_result>',
      'label' => t('List of AV results'),
    ),
  );
}

/**
 * Defines property info for VirusTotal AV scan results.
 */
function _virustotal_rules_rules_scan_result_info() {
  return array(
    'av' => array(
      'type' => 'text',
      'label' => ('Name of AV software used'),
    ),
    'detected' => array(
      'type' => 'boolean',
      'label' => ('Boolean, whether something was detected'),
    ),
    'version' => array(
      'type' => 'text',
      'label' => ('AV version used'),
    ),
    'result' => array(
      'type' => 'text',
      'label' => ('Result type found'),
    ),
    'update' => array(
      'type' => 'date',
      'label' => ('Timestamp of last update'),
    ),
  );
}

/**
 * Select list of response status codes.
 */
function virustotal_rules_response_status_codes() {
  // Inlcude VirusTotal API class, otherwise constants won't be defined.
  include_once drupal_get_path('module', 'virustotal') . '/virustotal.class.php';
  return array(
    VIRUSTOTAL_RESULT_OK => t('OK'),
    VIRUSTOTAL_RESULT_NOT_FOUND => t('Not found'),
    VIRUSTOTAL_RESULT_INVALID_KEY => t('Invalid key'),
    VIRUSTOTAL_RESULT_QUEUED => t('Still queued'),
    VIRUSTOTAL_RESULT_HTTP_ERROR => t('HTTP Error'),
  );
}

/**
 * Implements hook_rules_action_info().
 *
 * Declares meta-data about actions for VirusTotal Rules.
 */
function virustotal_rules_rules_action_info() {
  $actions = array();

  // Action to send a file to VirusTotal service.
  $actions['virustotal_rules_action_send_file'] = array(
    'label' => t('Send a File'),
    'group' => t('VirusTotal'),
    'parameter' => array(
      'file' => array(
        'type' => 'file',
        'label' => t('File Object'),
        'description' => t('File Object of the local file you want to send'),
      ),
    ),
    'provides' => array(
      'response' => array(
        'type' => 'virustotal_response',
        'label' => t('File scan response'),
        'description' => t('Contains: response_code, verbose_msg, resource, scan_id, permalink, sha256'),
      ),
    ),
  );

  // Action to send an URL to VirusTotal service.
  $actions['virustotal_rules_action_send_url'] = array(
    'label' => t('Send an URL'),
    'group' => t('VirusTotal'),
    'parameter' => array(
      'url' => array(
        'type' => 'text',
        'label' => t('URL'),
        'description' => t('The URL that should be scanned'),
      ),
    ),
    'provides' => array(
      'response' => array(
        'type' => 'virustotal_response',
        'label' => t('URL scan response'),
        'description' => t('Contains: response_code, verbose_msg, scan_date, scan_id, permalink, url'),
      ),
    ),
  );

  // Action to retrieve a file report from VirusTotal service.
  $actions['virustotal_rules_action_get_file_report'] = array(
    'label' => t('Retrieve a file report'),
    'group' => t('VirusTotal'),
    'parameter' => array(
      'resource' => array(
        'type' => 'text',
        'label' => t('Resource'),
        'description' => t('A md5/sha1/sha256 hash will retrieve the most recent report on a given sample. You may also specify a permalink identifier (sha256-timestamp (Scan ID) as returned by the file upload API) to access a specific report.'),
      ),
    ),
    'provides' => array(
      'response' => array(
        'type' => 'virustotal_response',
        'label' => t('File report response'),
        'description' => t('Contains: response_code, verbose_msg, resource, scan_id, permalink, md5, sha1, sha256, scan_date, positives, total, scans (detected, version, result, update).'),
      ),
    ),
  );

  // Action to retrieve an URL report from VirusTotal service.
  $actions['virustotal_rules_action_get_url_report'] = array(
    'label' => t('Retrieve an URL report'),
    'group' => t('VirusTotal'),
    'parameter' => array(
      'resource' => array(
        'type' => 'text',
        'label' => t('Resource'),
        'description' => t('An URL will retrieve the most recent report on the given URL. You may also specify a permalink identifier (md5-timestamp as returned by the URL submission API) to access a specific report.'),
      ),
    ),
    'provides' => array(
      'response' => array(
        'type' => 'virustotal_response',
        'label' => t('URL report response'),
        'description' => t('Contains: response_code, verbose_msg, resource, scan_id, permalink, scan_date, positives, total, scans (detected, result).'),
      ),
    ),
  );

  // Action to create a comment on a file or URL report.
  $actions['virustotal_rules_action_create_comment'] = array(
    'label' => t('Comment a report'),
    'group' => t('VirusTotal'),
    'parameter' => array(
      'resource' => array(
        'type' => 'text',
        'label' => t('Resource'),
        'description' => t('Either a md5/sha1/sha256 hash of the file you want to review or the URL itself that you want to comment on.'),
      ),
      'comment' => array(
        'type' => 'text',
        'label' => t('Comment'),
        'description' => t('The actual review, you can tag it using the "#" twitter-like syntax (e.g. #disinfection #zbot) and reference users using the "@" syntax (e.g. @EmilianoMartinez).'),
      ),
    ),
    'provides' => array(
      'response' => array(
        'type' => 'virustotal_response',
        'label' => t('URL report response'),
        'description' => t('Contains: response_code, verbose_msg.'),
      ),
    ),
  );

  return $actions;
}

/**
 * The action function for the 'virustotal_rules_action_send_file'.
 * An action to send a file to VirusTotal service.
 */
function virustotal_rules_action_send_file($file) {
  // Submit the file for scanning.
  $vt = new VirusTotal();
  $response = $vt->scanFile($file);

  // Provide response data as expected to rules.
  return array('response' => $response);
}

/**
 * The action function for the 'virustotal_rules_action_send_url'.
 * An action to send an URL to VirusTotal service.
 */
function virustotal_rules_action_send_url($url) {
  // Submit the URL for scanning.
  $vt = new VirusTotal();
  $response = $vt->scanUrl($url);

  // Provide response data as expected to rules.
  return array('response' => $response);
}

/**
 * The action function for the 'virustotal_rules_action_get_file_report'.
 * An action to retrieve a file report from VirusTotal service.
 */
function virustotal_rules_action_get_file_report($resource) {
  // Request a file report.
  $vt = new VirusTotal();
  $response = $vt->getFileReport($resource);

  // We have to provide a correct scan data type here, so we construct it.
  $scans = array();
  foreach ($response['scans'] as $av => $results) {
    $scans[] = $results + array('av' => $av);
  }
  $response['scans'] = $scans;

  // Provide response data to rules.
  return array('response' => $response);
}

/**
 * The action function for the 'virustotal_rules_action_get_url_report'.
 * An action to retrieve an URL report from VirusTotal service.
 */
function virustotal_rules_action_get_url_report($resource) {
  // Request an URL report.
  $vt = new VirusTotal();
  $response = $vt->getUrlReport($resource);

  // We have to provide a correct scan data type here, so we construct it.
  $scans = array();
  foreach ($response['scans'] as $av => $results) {
    $scans[] = $results + array('av' => $av);
  }
  $response['scans'] = $scans;

  // Provide response data to rules.
  return array('response' => $response);
}

/**
 * The action function for the 'virustotal_rules_action_create_comment'.
 */
function virustotal_rules_action_create_comment($resource, $comment) {
  // Create a comment on a file / URL report.
  $vt = new VirusTotal();
  $response = $vt->makeComment($resource, $comment);

  // Provide response data to rules.
  return array('response' => $response);
}

/**
 * Implements hook_rules_event_info().
 *
 * Declares meta-data about events for VirusTotal Rules.
 */
function virustotal_rules_rules_event_info() {
  $events = array();

  // Event that triggers before an API request is send.
  $events['virustotal_rules_event_pre_request'] = array(
    'label' => t('Before sending a request'),
    'group' => t('VirusTotal'),
    'variables' => array(
      'function' => array(
        'type' => 'text',
        'label' => t('API function'),
      ),
    ),
  );

  // Event that triggers after an API request was send.
  $events['virustotal_rules_event_post_request'] = array(
    'label' => t('After a request was sent'),
    'group' => t('VirusTotal'),
    'variables' => array(
      'response' => array(
        'type' => 'virustotal_response',
        'label' => t('API response'),
      ),
      'function' => array(
        'type' => 'text',
        'label' => t('API function'),
      ),
    ),
  );

  return $events;
}

/**
 * @}
 */
