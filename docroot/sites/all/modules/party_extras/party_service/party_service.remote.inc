<?php
/**
 * @file
 * Class for handling interactions with remote services.
 */

/**
 * Interface
 */
interface PartyRemoteHandlerInterface {

  /**
   * Perform a GET without specifying a resource id.
   */
  public function getAll();

  /**
   * Perform a GET on a particular resource.
   */
  public function get($id);

  /**
   * Post a Party.
   */
  public function post($party);

}

/**
 * Base class.
 */
class PartyRemoteHandlerBase implements PartyRemoteHandlerInterface {

  /**
   * The remote config entity.
   */
  protected $remote = NULL;

  /**
   * Get a new handler instance.
   *
   * @param Entity $remote
   *   The remote configuation entity.
   */
  public static function create($remote) {
    return new PartyRemoteHandlerBase($remote);
  }

  /**
   * Construct a new handler.
   */
  public function __construct($remote) {
    $this->remote = $remote;
  }

  /**
   * {@inheritdoc}
   */
  public function getAll() {}

  /**
   * {@inheritdoc}
   */
  public function get($id) {}

  /**
   * {@inheritdoc}
   */
  public function post($party) {
    // Prepare record
    $record = $this->formatRecord($party);
    $content = json_encode($record);

    // Get the URL ready.
    $url = $this->generateUrl();

    // Send request
    $response = $this->sendRequest('POST', $url, $content);
    return $response;
  }

  /**
   * Format the content into the object this service expects.
   *
   * @param Party $party
   *   The party object.
   *
   * @return stdClass
   *   A simple object ready for serializing.
   */
  protected function formatRecord($party) {
    return PartyServiceFormatterBase::createFromEntity($this->remote)->convertPartyToRecord($party);
  }

  /**
   * Generate the url for this remote.
   */
  protected function generateUrl($id = NULL) {
    $url = $this->remote->url;
    if (!empty($id)) {
      $url .= "/{$id}";
    }
    return $url;
  }

  /**
   * Get the default headers for this url.
   */
  protected function defaultHeaders() {
    $headers = array();
    if (!empty($this->remote->authentication['api_key'])) {
      $headers['X-Api-Key'] = $this->remote->authentication['api_key'];
    }

    return $headers;
  }

  /**
   * Make a request.
   *
   * @return Array
   *   The response in an array with at least the keys status and content.
   */
  protected function sendRequest($method, $url, $content = '', $headers = array()) {
    $headers += $this->defaultHeaders();
  }

}

/**
 * A Curl Based Implementation of the Remote System.
 */
class PartyRemoteHandlerCURL extends PartyRemoteHandlerBase {

  /**
   * {@inheritdoc}
   */
  public static function create($remote) {
    return new PartyRemoteHandlerCURL($remote);
  }

  /**
   * {@inheritdoc}
   */
  protected function sendRequest($method, $url, $content = '', $headers = array()) {
    $headers += $this->defaultHeaders();

    // Convert headers into the right format for CURL
    $formatted_headers = array();
    foreach ($headers as $header => $value) {
      $formatted_headers[] = "{$header}: {$value}";
    }

    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $method);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $formatted_headers);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $content);

    $response = curl_exec($ch);
    $error = curl_error($ch);
    $result = array(
      'header' => '',
      'content' => '',
      'curl_error' => '',
      'status' => '',
      'last_url' => '',
    );

    if ($error != "") {
      $result['curl_error'] = $error;
    }
    else {
      $header_size = curl_getinfo($ch, CURLINFO_HEADER_SIZE);
      $result['header'] = substr($response, 0, $header_size);
      $result['content'] = substr($response, $header_size);
      $result['status'] = curl_getinfo($ch,CURLINFO_HTTP_CODE);
      $result['last_url'] = curl_getinfo($ch,CURLINFO_EFFECTIVE_URL);
    }

    return $result;
  }

}
