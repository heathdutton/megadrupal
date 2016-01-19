<?php

/**
 * Class SourcePointAsset.
 */
class SourcePointAsset {
  /**
   * The Sourcepoint Service URL.
   */
  const SERVICE_URL_BASE = 'https://api.sourcepoint.com/script';

  /**
   * The API Key.
   */
  protected $apiKey;

  /**
   * The Service Url.
   */
  protected $serviceUrl;

  /**
   * Initial setup.
   */
  public function __construct($apiKey, $method) {
    if (empty($apiKey) || empty($method)) {
      throw new Exception('Missing arguments in SourcePointAsset.');
    }
    $this->apiKey = $apiKey;
    $this->serviceUrl = self::SERVICE_URL_BASE . "?delivery=$method";
  }

  public function determine_asset() {
    if (!function_exists('curl_init')) {
      throw new Exception('php-curl is required.');
    }

    $header = array(
      'Content-type: text/xml',
      "Authorization: Token token=$this->apiKey",
    );

    // Curl init.
    $ch = curl_init($this->serviceUrl);
    curl_setopt_array($ch, array(
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLINFO_HEADER_OUT => TRUE,
      CURLOPT_HTTPHEADER => $header,
    ));

    // Curl exec.
    $data = curl_exec($ch);

    // Check for Auth error (returned as JSON).
    $data_json = drupal_json_decode($data);
    if (!empty($data_json)) {
      throw new Exception('Error(s): ' . PHP_EOL . implode(', ', $data_json));
    }

    // Check for Curl errors.
    if (curl_errno($ch)) {
      throw new Exception('Curl Request Error:' . curl_error($ch));
    }

    // Validate retrieved data.
    if (!$this->isValidScript($data)) {
      throw new Exception('Retrieved script doesn\'t seem to be valid.');
    }

    $this->virginData = $data;
    $this->encodedData = $this->encodeAsset($data);

    return $this;
  }

  /**
   * Validate retrieved script.
   */
  private function isValidScript($data) {
    return preg_match('~<script.*</script>~Usmi', $data);
  }

  private function encodeAsset($data = '') {
    return base64_encode($data);
  }
}
