<?php
/**
 * @file
 * Class file for Readytalk.
 */

/**
 * Class file for handling ReadyTalk Api items.
 */
class ReadyTalk {

  private $telephone_number;
  private $access_code;
  private $pass_code;
  private $creds;
  public $timeout = 5;
  public $api_url = 'https://cc.readytalk.com/api/1.3/';

  /**
   * Default constructor.
   *
   * @param string $telephone_number
   *   Conference telephone number.
   * @param string $access_code
   *   Readytalk access code.
   * @param string $pass_code
   *   Readytalk pass code.
   */
  public function __construct($telephone_number, $access_code, $pass_code) {
    $this->telephone_number = $telephone_number;
    $this->access_code = $access_code;
    $this->pass_code = $pass_code;
    $this->creds = $telephone_number . ':' . $access_code . ':' . $pass_code;
  }

  /**
   * Gets meeing info from a user.
   *
   * @return array $info
   *   Returns id, title, and startDate of Meeting.
   */
  public function get_meetings_info() {
    $info = array();

    // Build url.
    $url = 'https://' . $this->creds . '@cc.readytalk.com/api/1.3/svc/rs/meetings';

    // Get xml result from ReadyTalk.
    $data = $this->_curl($url);

    try {
      $simpleXml = @new SimpleXMLElement($data);
    }
    catch (Exception $e) {
      drupal_set_message(t('Could not get meeting info from ReadyTalk.'), 'error');
      return FALSE;
    }

    $count = 1;
    foreach ($simpleXml->meeting as $meeting) {
      $info[$count]['id'] = (string) $meeting->id;
      $info[$count]['title'] = (string) $meeting->title;
      $info[$count]['startDate'] = (string) $meeting->startDate;
      $count++;
    }

    return $info;
  }

  /**
   * Get meeting info for a certain meeting.
   *
   * @param string $id
   *   Meeting id.
   *
   * @return object $simpleXml
   *   The SimpleXMLElement.
   */
  public function get_meeting_info($id) {
    $result = $this->_api_request('GET', 'svc/rs/meetings/' . $id);

    if (isset($result->data)) {
      $simpleXml = @new SimpleXMLElement($result->data);
      return $simpleXml;
    }

    return FALSE;
  }

  /**
   * Curl functionality to be used to access ReadyTalk urls.
   *
   * @param string $url
   *   The api url.
   *
   * @return string $data;
   *   The string (xml) returned from Readytalk.
   */
  private function _curl($url) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $this->timeout);
    $data = curl_exec($ch);
    curl_close($ch);

    return $data;
  }

  /**
   * Add user into meeting.
   *
   * @param array $data
   *   An array of ReadyTalk field names => form values.
   */
  public function add_meeting_user($data = array()) {
    $result = $this->_api_request('POST', 'svc/rs/registrations', $data);

    if (isset($result->code)) {
      switch ($result->code) {
        case 200:
          // Yay this one is good :).
          break;

        default:
          // Get Error message from xml response from ReadyTalk.
          $simpleXml = @new SimpleXMLElement($result->data);
          $error_code = (string) $simpleXml->error->code;
          $error_message = (string) $simpleXml->error->message;

          watchdog('readytalk', '%error_message  Error Code:%error_code.', array('%error_message' => $error_message, '%error_code' => $error_code));
          break;

      }
    }
    else {
      watchdog('readytalk', 'Was not able to connect to Readytalk');
    }
  }

  /**
   * Api Request.
   *
   * @param string $method
   *   GET or POST.
   * @param string $endpoint
   *   The url path endpoint for the api (Don't include the leading and
   *   trailing slashes).
   *
   * @return object $return
   *   See drupal_http_request().
   */
  private function _api_request($method, $endpoint, $data = array()) {
    $options = array(
      'method' => $method,
      'timeout' => $this->timeout,
      'headers' => array(
        'Authorization' => 'Basic ' . base64_encode($this->creds),
      ),
    );

    $url = url($this->api_url . $endpoint, array('query' => $data, 'external' => TRUE));
    $result = drupal_http_request($url, $options);

    return $result;
  }
}
