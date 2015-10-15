<?php

/**
 * Class Membersify_ApiResource
 */
abstract class Membersify_ApiResource extends Membersify_Object {
  protected static $object = 'api_resource';

  /**
   * Performs a request against the Membersify API Web Service.
   *
   * @param string $endpoint
   *   The endpoint on the web service.
   * @param array $params
   *   (Optional) An associative array of parameters. Defaults to NULL.
   * @return mixed
   *   The result from the request.
   * @throws Membersify_Error
   */
  public static function request($endpoint, $params = NULL) {
    if (is_null($params)) {
      $params = array();
    }
    $params['timestamp'] = time();
    $content = json_encode($params);
    $hash = hash_hmac('sha256', $content, Membersify::$secret_key);

    $langVersion = phpversion();
    $uname = php_uname();
    $ua = array(
      'lang' => 'php',
      'lang_version' => $langVersion,
      'publisher' => 'membersify',
      'uname' => $uname
    );
    $headers = array(
      'X-Membersify-Client-User-Agent: ' . json_encode($ua),
      'X-Membersify-Site-Key: ' . Membersify::$public_key,
      'X-Membersify-Hash: ' . $hash,
      'X-Membersify-Version: ' . Membersify::$api_version,
    );

    $curl = curl_init();

    $opts = array();
    $opts[CURLOPT_POST] = 1;
    $opts[CURLOPT_POSTFIELDS] = $content;
    $opts[CURLOPT_URL] = Membersify::$api_url . $endpoint . '.json';
    $opts[CURLOPT_RETURNTRANSFER] = true;
    $opts[CURLOPT_CONNECTTIMEOUT] = 30;
    $opts[CURLOPT_TIMEOUT] = 80;
    $opts[CURLOPT_HTTPHEADER] = $headers;

    curl_setopt_array($curl, $opts);
    $rbody = curl_exec($curl);

    $errno = curl_errno($curl);
    if ($errno == CURLE_SSL_CACERT || $errno == CURLE_SSL_PEER_CERTIFICATE || $errno == 77) {
      array_push($headers, 'X-Membersify-Client-Info: {"ca":"using Membersify-supplied CA bundle"}');
      curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
      curl_setopt($curl, CURLOPT_CAINFO,
                  dirname(__FILE__) . '/../data/ca-certificates.crt');
      $rbody = curl_exec($curl);
    }

    if ($rbody === FALSE) {
      $message = curl_error($curl);
      curl_close($curl);
      throw new Membersify_Error("CURL Error: " . $errno . " Message: " . $message);
    }

    $rcode = curl_getinfo($curl, CURLINFO_HTTP_CODE);

    curl_close($curl);

    if ($rcode < 200 || $rcode >= 300) {
      throw new Membersify_Error("Bad code received: " . $rcode . " Message: " . $rbody);
    }

    $response = json_decode($rbody, true);

    return $response;
  }

  /**
   * Sets values in an object.
   *
   * @param mixed $data
   *   An object or associative array of values to set.
   */
  public function setValues($data) {
    foreach ($data as $key => $value) {
      $this->$key = $value;
    }
  }

  /**
   * Retrieves a Membersify object from the server by id.
   *
   * @param string $id
   *   The id of the object.
   * @return mixed
   *   The Membersify object.
   */
  public static function retrieve($id) {
    $response = self::request(static::$object . '/retrieve', array('id' => $id));
    $class = self::get_class_name();
    $instance = new $class();
    $instance->setValues($response);

    return $instance;
  }

  /**
   * @param null $params
   * @return array
   */
  public static function all($params = null){
    $response = self::request(static::$object . '/index', $params);

    $class = self::get_class_name();
    $list = array();
    foreach ($response as $values_array) {
      $instance = new $class();
      $instance->setValues($values_array);
      $list[$instance->id] = $instance;
    }

    return $list;
  }

  /**
   * Creates a new object using the Membersify API.
   *
   * @param null $params
   *   An associative array of parameters to pass to the API request.
   *
   * @return mixed
   *   The new object.
   */
  public static function create($params = null) {
    $response = self::request(static::$object . '/create', $params);
    $class = self::get_class_name();
    $instance = new $class();
    $instance->setValues($response);
    return $instance;
  }

  /**
   * Saves an object using the Membersify API.
   *
   * @return $this
   *   The updated object.
   */
  public function save() {
    $params = $this->__toArray();

    if (count($params) > 0) {
      $response = $this->request($this->object . '/save', $params);
      $this->setValues($response);
    }
    return $this;
  }
}
