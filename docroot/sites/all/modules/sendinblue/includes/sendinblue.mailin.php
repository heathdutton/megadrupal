<?php
/**
 * @file
 * Rest class file.
 */

/**
 * Sendinblue REST client.
 */
class SendinblueMailin {
  public $apiKey;
  public $baseUrl;
  public $curlOpts = array();

  /**
   * Constructor.
   *
   * @param string $base_url
   *   A request url of api.
   * @param string $api_key
   *   A access key of api.
   */
  public function __construct($base_url, $api_key) {
    if (!function_exists('curl_init')) {
      $msg = 'SendinBlue requires CURL module';
      watchdog('sendinblue', $msg, NULL, WATCHDOG_ERROR);
      return;
    }
    $this->baseUrl = $base_url;
    $this->apiKey = $api_key;
  }

  /**
   * Do CURL request with authorization.
   *
   * @param string $resource
   *   A request action of api.
   * @param string $method
   *   A method of curl request.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   An associate array with respond data.
   */
  private function doRequest($resource, $method, $input) {
    if (!function_exists('curl_init')) {
      $msg = 'SendinBlue requires CURL module';
      watchdog('sendinblue', $msg, NULL, WATCHDOG_ERROR);
      return NULL;
    }
    $called_url = $this->baseUrl . "/" . $resource;
    $ch = curl_init($called_url);
    $auth_header = 'api-key:' . $this->apiKey;
    $content_header = "Content-Type:application/json";
    //if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
      // Windows only over-ride because of our api server.
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    //}
    curl_setopt($ch, CURLOPT_HTTPHEADER, array($auth_header, $content_header));
    //curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $method);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $input);
    $data = curl_exec($ch);
    if (curl_errno($ch)) {
      watchdog('sendinblue', 'Curl error: @error', array('@error' => curl_error($ch)), WATCHDOG_ERROR);
    }
    curl_close($ch);
    return drupal_json_decode($data);
  }

  /**
   * Do CURL request directly into sendinblue.
   *
   * @param array $data
   *  A data of curl request.
   * @return array
   *   An associate array with respond data.
   */
  private function doRequestDirect($data) {
    if (!function_exists('curl_init')) {
      $msg = 'SendinBlue requires CURL module';
      watchdog('sendinblue', $msg, NULL, WATCHDOG_ERROR);
      return NULL;
    }
    $url = 'http://ws.mailin.fr/';
    $ch = curl_init();
    $paramData = '';
    $data['source'] = 'Drupal';
    if (is_array($data))
      foreach ($data as $key => $value) {
        $paramData .= $key . '=' . urlencode($value) . '&';
      }
    else {
      $paramData = $data;
    }
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:'));
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $paramData);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_URL, $url);
    $data = curl_exec($ch);
    curl_close($ch);
    return $data;
  }

  /**
   * Get Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function get($resource, $input) {
    return $this->doRequest($resource, "GET", $input);
  }

  /**
   * Put Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function put($resource, $input) {
    return $this->doRequest($resource, "PUT", $input);
  }

  /**
   * Post Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function post($resource, $input) {
    return $this->doRequest($resource, "POST", $input);
  }

  /**
   * Delete Request of API.
   *
   * @param string $resource
   *   A request action.
   * @param string $input
   *   A data of curl request.
   *
   * @return array
   *   A respond data.
   */
  public function delete($resource, $input) {
    return $this->doRequest($resource, "DELETE", $input);
  }

  /**
   * Get the details of an account.
   *
   * @return array
   *   An array of account detail.
   */
  public function getAccount() {
    return $this->get("account", "");
  }

  /**
   * Get campaigns by type.
   *
   * @param string $type
   *   A campaign type.
   *
   * @return array
   *   An array of campaigns.
   */
  public function getCampaigns($type) {
    return $this->get("campaign/detailsv2", drupal_json_encode(array("type" => $type)));
  }

  /**
   * Get campaign by id.
   *
   * @param string $id
   *   A campaign identification.
   *
   * @return array
   *   An array of campaigns.
   */
  public function getCampaign($id) {
    return $this->get("campaign/" . $id . "/detailsv2", "");
  }

  /**
   * Get lists of an account.
   *
   * @return array
   *   An array of all lists.
   */
  public function getLists() {
    return $this->get("list", "");
  }

  /**
   * Get list by id.
   *
   * @param string $id
   *   A list identification.
   *
   * @return array
   *   An array of lists.
   */
  public function getList($id) {
    return $this->get("list/" . $id, "");
  }

  /**
   * Send email via sendinblue.
   *
   * @param string $to
   *   A recipe email address.
   * @param string $subject
   *   A subject of email.
   * @param string $from
   *   A sender email address.
   * @param string $html
   *   A html body of email content.
   * @param string $text
   *   A text body of email content.
   * @param array $cc
   *   A cc address.
   * @param array $bcc
   *   A bcc address.
   * @param string $replyto
   *   A reply address.
   * @param array $attachment
   *   A attachment information.
   * @param array $headers
   *   A header of email.
   *
   * @return array
   *   An array of response code.
   */
  public function sendEmail($to, $subject, $from, $html, $text, $cc = array(), $bcc = array(), $replyto = '', $attachment = array(), $headers = array()) {
    return $this->post("email", drupal_json_encode(
      array(
        "cc" => $cc,
        "text" => $text,
        "bcc" => $bcc,
        "replyto" => $replyto,
        "html" => $html,
        "to" => $to,
        "attachment" => $attachment,
        "from" => $from,
        "subject" => $subject,
        "headers" => $headers)));
  }

  /**
   * Get user by email.
   *
   * @param string $email
   *   An email address.
   *
   * @return array
   *   An array of response code.
   */
  public function getUser($email) {
    return $this->get("user/" . $email, "");
  }

  /**
   * Create and update user.
   *
   * @param string $email
   *   An email address of user.
   * @param array $attributes
   *   An attributes to update.
   * @param array $blacklisted
   *   An array of black user.
   * @param string $listid
   *   A new listid.
   * @param string $listid_unlink
   *   A link unlink.
   *
   * @return array
   *   A response code.
   */
  public function createUpdateUser($email, $attributes = array(), $blacklisted = array(), $listid = '', $listid_unlink = '') {
    return $this->post("user/createdituser", drupal_json_encode(
      array(
        "email" => $email,
        "attributes" => $attributes,
        "blacklisted" => $blacklisted,
        "listid" => $listid,
        "listid_unlink" => $listid_unlink)));
  }

  /**
   * Get attribute by type.
   *
   * @param string $type
   *   A type.
   *
   * @return array
   *   An array of attributes.
   */
  public function getAttribute($type) {
    return $this->get("attribute/" . $type, "");
  }

  /**
   * Get attribute by type.
   *
   * @return array
   *   An array of attributes.
   */
  public function getAttributes() {
    return $this->get("attribute/", "");
  }

  /**
   * Get senders.
   *
   * @param array $option
   *   A option information.
   *
   * @return array
   *   A sender details.
   */
  public function getSenders($option = array()) {
    return $this->get("advanced", drupal_json_encode(array("option" => $option)));
  }

  /**
   * Get the access token.
   *
   * @return array
   *   An access token information.
   */
  public function getAccessTokens() {
    return $this->get("account/token", "");
  }

  /**
   * Delete access token.
   *
   * @param string $key
   *   An access token.
   *
   * @return array
   *   A response code.
   */
  public function deleteToken($key) {
    return $this->post("account/deletetoken", drupal_json_encode(array("token" => $key)));
  }

  /**
   * Get the details of smtp.
   *
   * @return array
   *   A smtp details.
   */
  public function getSmtpDetails() {
    return $this->get("account/smtpdetail", "");
  }

  /**
   * Display all users of list.
   *
   * @param array $listids
   *   An array of lists.
   * @param int $page
   *   A page number.
   * @param int $page_limit
   *   A page limit per one page.
   *
   * @return array
   *   An array of users.
   */
  public function displayListUsers($listids = array(), $page = 0, $page_limit = 50) {
    return $this->post("list/display", drupal_json_encode(
      array(
        "listids" => $listids,
        "page" => $page,
        "page_limit" => $page_limit)));
  }

  /**
   * Add the Partner's name in sendinblue.
   */
  public function partnerDrupal() {
    $data = array();
    $data['key'] = $this->apiKey;
    $data['webaction'] = 'MAILIN-PARTNER';
    $data['partner'] = 'DRUPAL';
    $this->doRequestDirect($data);
  }
}
