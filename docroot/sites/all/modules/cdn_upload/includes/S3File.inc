<?php
/**
 * @file
 * Contains S3File
 */
class S3File {

  private $client;
  private $config;
  private $bucket;
  private $maxSize;

  /**
   * Construct the S3Interface
   */
  public function __construct($bucket, $maxSize = 15000000) {
    $this->config = awssdk_service_load('s3')->config;
    $this->client = awssdk_get_client('s3');
    $this->bucket = $bucket;
    $this->maxSize = $maxSize;
  }

  /**
   * Provided a URL, returns a key.
   * @param $url
   *
   * @return mixed
   */
  public function getKeyFromURL($url) {
    $matches = array();
    preg_match('/\/([^\/]*)$/', $url, $matches);
    return $matches[1];
  }

  /**
   * Returns the UUID provided the url of the file.
   * @param $url
   *
   * @return mixed
   */
  public function getUUIDFromURL($url) {
    $matches = array();
    preg_match('/\/([^\/\.]*)[^\/]*$/', $url, $matches);
    return $matches[1];
  }

  /**
   * Gets the info for the item.
   * @param $url
   */
  public function getInfo($url) {
    return $this->client->getObject(array(
      'Bucket' => $this->bucket,
      'Key' => $this->getKeyFromURL($url)
    ));
  }

  /**
   * Delete the item provided by this bucket and key.
   *
   * @param string $key
   *   The item within S3 to delete.
   */
  public function delete($key) {
    $this->client->deleteObject(array(
      'Bucket' => $this->bucket,
      'Key' => $key,
    ));
  }

  /**
   * Verify if the file is valid.
   *
   * @param string $uuid
   *   The uuid of the file that was created in S3.
   * @param string $key
   *   The key for this S3 upload.
   *
   * @return array
   *   The repsonse for the verification.
   */
  public function verify($uuid, $key) {

    // Get the url of the file uploaded.
    $url = $this->getURL($key);

    // Get the temporary URL for this file.
    $tempURL = $this->getTempURL($key);

    // Check to see if the file exists.
    if ($uuid && $url) {

      // Return the file details.
      return array(
        'bucket' => $this->bucket,
        'url' => $url,
        'tempURL' => $tempURL,
        'uuid' => $uuid,
        'key' => $key
      );
    }
    else {

      // Return that an error occured.
      return array('error' => t('File not found.'));
    }
  }

  /**
   * Encode a string as a base64 sha1.
   *
   * @param string $content
   *   The content you wish to encode.
   *
   * @return string
   *   The base64 encoded sha1 version of this string.
   */
  private function encode($content) {
    return base64_encode(hash_hmac(
      'sha1',
      $content,
      $this->config['access']['secret'],
      true
    ));
  }

  /**
   * Sign a request.
   *
   * @return array
   *   The response for the signature.
   */
  public function sign() {

    // Get the response body and encode it as json.
    $responseBody = file_get_contents('php://input');
    $content = json_decode($responseBody, true);

    // Check if the content has headers.
    if (!empty($content["headers"])) {

      // Check to see if the headers are valid.
      if ($this->isValidRequest($content["headers"])) {

        // Return the signature.
        return array('signature' => $this->encode($content["headers"]));
      }
    }

    // Now check to see if the policy is valid.
    else if ($this->isPolicyValid($content)) {

      // Encode the body as the policy.
      $encodedPolicy = base64_encode($responseBody);

      // Return the response for the encoded policy.
      return array(
        'policy' => $encodedPolicy,
        'signature' => $this->encode($encodedPolicy),
      );
    }

    // Return invalid if we get to this point.
    return array("invalid" => true);
  }

  /**
   * Get a URL for the file that was uploaded.
   *
   * @param string $key
   *   The key for the S3 file that was created.
   *
   * @return mixed
   */
  protected function getURL($key) {
    return $this->client->getObjectUrl($this->bucket, $key);
  }

  /**
   * Get a temporary URL for the file that was uploaded.
   *
   * @param string $key
   *   The key for the S3 file that was created.
   *
   * @return mixed
   */
  protected function getTempURL($key) {
    $url = "{$this->bucket}/{$key}";
    $request = $this->client->get($url);
    return $this->client->createPresignedUrl($request, '+15 minutes');
  }

  /**
   * Checks to see if the request is valid.
   *
   * @param string $headers
   *   A string of the headers for the request.
   *
   * @return bool
   *   If this request is valid or not.
   */
  private function isValidRequest($headers) {
    $pattern = '/\/' . $this->bucket . '\/.+$/';
    preg_match($pattern, $headers, $matches);
    return count($matches) > 0;
  }

  /**
   * Check to see if a policy is valid.
   *
   * @param array $policy
   *   The request body as a json parsed array.
   *
   * @return bool
   *   If the policy is valid or not.
   */
  private function isPolicyValid($policy) {
    $conditions = $policy["conditions"];
    $bucket = null;
    $parsedMaxSize = null;
    for ($i = 0; $i < count($conditions); ++$i) {
      $condition = $conditions[$i];

      if (isset($condition["bucket"])) {
        $bucket = $condition["bucket"];
      }
      else if (isset($condition[0]) && $condition[0] == "content-length-range") {
        $parsedMaxSize = $condition[2];
      }
    }

    // See if the policy is valid.
    return ($bucket == $this->bucket && $parsedMaxSize == (string)$this->maxSize);
  }
}
