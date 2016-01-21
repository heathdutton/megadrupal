<?php

/**
 * @file
 * Provides the VirusTotal API service class.
 *
 * This is based on the guidelines of VirusTotal API
 * https://www.virustotal.com/documentation/public-api/
 */

/**
 * Request was valid and could be processed successfully.
 */
define('VIRUSTOTAL_RESULT_OK', 1);

/**
 * The searched item could not be found.
 */
define('VIRUSTOTAL_RESULT_NOT_FOUND', 0);

/**
 * Used API key was not valid.
 */
define('VIRUSTOTAL_RESULT_INVALID_KEY', -1);

/**
 * Requested item is still queued.
 */
define('VIRUSTOTAL_RESULT_QUEUED', -2);

/**
 * cURL HTTP request failed.
 */
define('VIRUSTOTAL_RESULT_HTTP_ERROR', -10);

/**
 * API function names.
 */
define('VIRUSTOTAL_FUNC_SCAN_FILE', 'file/scan');
define('VIRUSTOTAL_FUNC_GET_FILE_REPORT', 'file/report');
define('VIRUSTOTAL_FUNC_SCAN_URL', 'url/scan');
define('VIRUSTOTAL_FUNC_GET_URL_REPORT', 'url/report');
define('VIRUSTOTAL_FUNC_MAKE_COMMENT', 'comments/put');

/**
 * Class for VirusTotal API.
 */
class VirusTotal {

  /**
   * VirusTotal API URL
   *
   * In order to use the VirusTotal API we need to send HTTP requests to
   * the following URL.
   *
   * @var string
   */
  protected $apiUrl = 'https://www.virustotal.com/vtapi/v2/';

  /**
   * The required API Key to access the VirusTotal API.
   *
   * Once you have a valid VirusTotal Community account, you will find your
   * personal API key on the profile of your account (sign in and click on
   * the API key tab). This key is all you need to use VirusTotal's API.
   *
   * @var string
   */
  protected $apiKey = NULL;

  /**
   * Constructor for VirusTotal API class.
   *
   * @param string $api_key
   *   The required API Key to access the VirusTotal API.
   */
  public function __construct($api_key = NULL) {
    // Override configurated API Key if given.
    $this->apiKey = (empty($api_key) ? variable_get('virustotal_api_key', '') : $api_key);
  }

  /**
   * Sends a File to VirusTotal service to queue it for scanning.
   *
   * The VirusTotal API allows you to send files. Before performing your
   * submissions we encourage you to retrieve the latest report on the
   * files, if it is recent enough you might want to save time and bandwidth
   * by making use of it.
   *
   * @param object $fobj
   *   File Object of the local file you want to send.
   *
   * @return array
   *   An array containing the following key/value pairs:
   *   - response_code: Response status code.
   *   - verbose_msg: Versbose message regarding response code.
   *   Following keys will only be given on success:
   *   - resource: VirusTotal resource ID (same as sha256).
   *   - scan_id: Resource ID + unique scan id.
   *   - permalink: Permanent URL to scan report.
   *   - sha256: SHA2 hash of file sent.
   */
  public function scanFile($fobj) {
    // Make sure its a valid instance.
    if (!is_object($fobj)) {
      return FALSE;
    }

    // Try to get real path of file.
    if ($wrapper = file_stream_wrapper_get_instance_by_uri($fobj->uri)) {
      $real_path = $wrapper->realpath();
    }
    else {
      return FALSE;
    }

    // Set HTTP multipart file information, so cURL can load and send it.
    return ($this->apiRequest(VIRUSTOTAL_FUNC_SCAN_FILE,
      array('file' => '@' . $real_path . ';type=' . $fobj->filemime . ';filename=' . $fobj->filename)
    ));
  }

  /**
   * Tries to retrieve a file scan report.
   *
   * @param string $resource
   *   A md5/sha1/sha256 hash will retrieve the most recent report on a
   *   given sample. You may also specify a permalink identifier
   *   (sha256-timestamp (Scan ID) as returned by the file upload API)
   *   to access a specific report. You can also specify a CSV list made
   *   up of a combination of hashes and scan_ids (up to 4 items with the
   *   standard request rate), this allows you to perform a batch request
   *   with one single call.
   *
   * @return array
   *   An array containing the following key/value pairs of scan report:
   *   - response_code: Response status code.
   *   - verbose_msg: Versbose message regarding response code.
   *   Following keys will only be given on success:
   *   - resource: VirusTotal resource ID (same as sha256).
   *   - scan_id: Resource ID + unique scan id.
   *   - permalink: Permanent URL to scan report.
   *   - md5: MD5 hash of file sent.
   *   - sha1: SHA1 hash of file sent.
   *   - sha256: SHA2 hash of file sent.
   *   - scan_date: Date of scan.
   *   - positives: Number of positive detections.
   *   - total: Number of AV's used.
   *   - scans: Array containing a list of AV results.
   *     Each key is the name of the AV software used:
   *     - detected: Boolean, whether something was detected.
   *     - version: AV version used.
   *     - result: Result type found.
   *     - update: Timestamp of last update.
   */
  public function getFileReport($resource) {
    if (!empty($resource)) {
      return ($this->apiRequest(VIRUSTOTAL_FUNC_GET_FILE_REPORT, array('resource' => $resource)));
    }
  }

  /**
   * Sends an URL to VirusTotal service and queue it for scanning.
   *
   * URLs can also be submitted for scanning. Once again, before performing
   * your submission we encourage you to retrieve the latest report on the
   * URL, if it is recent enough you might want to save time and bandwidth
   * by making use of it.
   *
   * @param string $url
   *   The string of the URL that should be scanned.
   *
   * @return array
   *   An array containing the following key/value pairs:
   *   - response_code: Response status code.
   *   - verbose_msg: Versbose message regarding response code.
   *   Following keys will only be given on success:
   *   - scan_date: Date of scan.
   *   - scan_id: Resource ID + unique scan id.
   *   - permalink: Permanent URL to scan report.
   *   - url: Same as the sent one.
   */
  public function scanUrl($url) {
    if (!empty($url)) {
      return ($this->apiRequest(VIRUSTOTAL_FUNC_SCAN_URL, array('url' => $url)));
    }
  }

  /**
   * Tries to retrieve a URL scan report.
   *
   * @param string $resource
   *   A URL will retrieve the most recent report on the given URL.
   *   You may also specify a scan_id (sha256-timestamp as returned by the URL
   *   submission API) to access a specific report. At the same time, you can
   *   specify a CSV list made up of a combination of hashes and scan_ids so as
   *   to perform a batch request with one single call (up to 4 resources per
   *   call with the standard request rate).
   *
   * @return array
   *   An array containing the following key/value pairs of scan report:
   *   - response_code: Response status code.
   *   - verbose_msg: Versbose message regarding response code.
   *   Following keys will only be given on success:
   *   - resource: VirusTotal resource ID (same as sha256).
   *   - scan_id: Resource ID + unique scan id.
   *   - permalink: Permanent URL to scan report.
   *   - url: Same as the sent one.
   *   - scan_date: Date of scan.
   *   - positives: Number of positive detections.
   *   - total: Number of scanners used.
   *   - scans: Array containing a list of scan results.
   *     Each key is the name of the scanning software used:
   *     - detected: Boolean, whether something was detected.
   *     - result: Result type found.
   */
  public function getUrlReport($resource) {
    if (!empty($resource)) {
      return ($this->apiRequest(VIRUSTOTAL_FUNC_GET_URL_REPORT, array('resource' => $resource)));
    }
  }

  /**
   * Creates a comment on a file or URL report.
   *
   * @param string $resource
   *   Either a md5/sha1/sha256 hash of the file you want to review or the URL
   *   itself that you want to comment on.
   * @param string $comment
   *   The actual review, you can tag it using the "#" twitter-like syntax
   *   (e.g. #disinfection #zbot) and reference users using the "@" syntax
   *   (e.g. @EmilianoMartinez).
   *
   * @return array
   *   An array containing the following key/value pairs:
   *   - response_code: Response status code.
   *   - verbose_msg: Versbose message regarding response code.
   */
  public function makeComment($resource, $comment) {
    if (!empty($resource) && !empty($comment)) {
      return ($this->apiRequest(VIRUSTOTAL_FUNC_MAKE_COMMENT,
        array(
          'resource' => $resource,
          'comment' => $comment,
        ))
      );
    }
  }

  /**
   * Sends the API request and interpretes the returned data.
   *
   * @param string $function
   *   API specific function name (used in URL).
   * @param array $data
   *   An array containing request data as name/value pairs.
   *
   * @return array
   *   An array containing decoded json data.
   *   At least the following keys:
   *   - response_code: Response status code.
   *   - verbose_msg: Versbose message regarding response code.
   */
  protected function apiRequest($function, $data = array()) {

    // Allow modules to make changes on post data.
    drupal_alter('virustotal_query', $data, $function);

    // Initialise cURL.
    $curl_handle = curl_init($this->apiUrl . $function);

    $curl_options = array(
      // Set POST data.
      CURLOPT_POST => TRUE,
      // Add key to HTTP POST data.
      CURLOPT_POSTFIELDS => $data + array('apikey' => $this->apiKey),
      // Results will be returned and not displayed.
      CURLOPT_RETURNTRANSFER => TRUE,
      // Forbid reuse so no quota can be wasted.
      CURLOPT_FORBID_REUSE => TRUE,
      // This is a fix for the Curl library to prevent
      // Expect: 100-continue headers in POST requests.
      CURLOPT_HTTPHEADER => array('Expect:'),
    );

    // Set cURL options.
    curl_setopt_array($curl_handle, $curl_options);

    // Send the HTTP POST request with cURL.
    $json_data = curl_exec($curl_handle);

    // Check for cURL errors.
    if (curl_errno($curl_handle)) {
      $result_data = array('response_code' => VIRUSTOTAL_RESULT_HTTP_ERROR);
    }
    else {
      // Decode returned json data to an array.
      $result_data = drupal_json_decode($json_data);
    }

    // Close cURL handle.
    curl_close($curl_handle);

    // Log the result by watchdog.
    watchdog('virustotal', check_plain(@$result_data['verbose_msg']));

    // Allow modules to react on query result.
    drupal_alter('virustotal_result', $result_data, $function);

    return $result_data;
  }
}
