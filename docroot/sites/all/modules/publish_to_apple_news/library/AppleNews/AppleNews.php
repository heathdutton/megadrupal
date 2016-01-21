<?php

/**
 * @file
 * Wrapper class around the AppleNews API.
 */

/**
 * Class AppleNews
 */

define('APPLE_NEWS_ARTICLE_FORMAT_VERSION', '0.10.13');

class AppleNews {

  protected $api_key;
  protected $api_secret;
  protected $api_host = 'https://u48r14.digitalhub.com';
  protected $api_body;
  public $ch;
  public $debug = true;

  public static $error_map = array(
    "UNAUTHORIZED" => "The Authorization header was not set.",
    "MISSING_DATE" => "The X-Apple-Date header was not set.",
    "INVALID_AUTH_FORMAT" => "The Authorization header parameters were not parseable as key=value pairs separated by ;.",
    "INVALID_AUTH_SCHEME" => "The first part of the Authorization header was not HHMAC.",
    "MISSING_API_KEY_ID" => "The API key ID was the empty string in the Authorization header.",
    "MISSING_SIGNATURE" => "The request signature was the empty string in the Authorization header.",
    "INVALID_API_KEY_FORMAT" => "The API Key ID in the Authorization header was not a UUID.",
    "WRONG_SIGNATURE" => "The signature given by the client did not match the signature computed by the server.",
    "API_KEY_NOT_FOUND" => "The API Key specified in the Authorization header does not exist.",
    "DATE_NOT_RECENT" => "The date specified in X-Apple-Date or Date was not within 5 minutes of the server's time.",
    "INVALID_DATE_FORMAT" => "The date specified in X-Apple-Date or Date was not in ISO 8601 format.",
    "DUPLICATE_AUTH_PARAMETER" => "A parameter was supplied twice in the Authorization header.",
    "DUPLICATE_AUTH_ELEMENT" => "Two Authorization elements with the same scheme were supplied.",
    "NOT_FOUND" => "The URL you tried to access does not exist.",
    "MISSING" => "The article.json part is required and was missing.",
    "INVALID_DOCUMENT" => "The JSON in the Native document (article.json) is invalid. Please test with the Preview Tool first.",
  );


  public function __construct($apikey=null, $apisecret=null) {

    if (!$apikey) {
        throw new AppleNews_Error('You must provide a AppleNews API key');
    }

    if (!$apisecret) {
        throw new AppleNews_Error('You must provide a AppleNews API secret');
    }

    $this->api_key = $apikey;
    $this->api_secret = $apisecret;

    $this->ch = curl_init();
    curl_setopt($this->ch, CURLOPT_USERAGENT, 'AppleNews-PHP/1.0');
    curl_setopt($this->ch, CURLOPT_HEADER, false);
    curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($this->ch, CURLOPT_CONNECTTIMEOUT, 30);

    if ($this->debug) {
      curl_setopt($this->ch, CURLOPT_SSL_VERIFYHOST, 0);
      curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER, 0);
    }
  }

  public function __destruct() {
    if(is_resource($this->ch)) {
        curl_close($this->ch);
    }
  }

  /**
   * Get the article format version the libary is compatible with
   * @return string
   */
  public function getArticleFormatVersion() {
    return APPLE_NEWS_ARTICLE_FORMAT_VERSION;
  }

  /**
   * Get the content for a channel
   * @param string $channel_id
   * @return associative_array containing all content for the channel
   */
  public function getChannel($channel_id) {

    $this->api_body = FALSE;
    $ch = $this->ch;
    curl_setopt($ch, CURLOPT_POST, FALSE);
    $url = $this->api_host . '/channels/' . $channel_id;
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET');
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json', $this->getAuthorizationHeader('GET', $url)));
    curl_setopt($ch, CURLOPT_VERBOSE, $this->debug);

    $response_body = curl_exec($ch);

    $info = curl_getinfo($ch);

    if(curl_error($ch)) {
      throw new AppleNews_Error("API call failed: " . curl_error($ch));
    }

    $result = json_decode($response_body, true);

    if(floor($info['http_code'] / 100) >= 4) {
        throw $this->castErrors($result);
    }

    return $result['data'];

  }

  /**
   * Get the sections for a channel
   * @param string $channel_id
   * @return array sections
   *    string name
   *    array links
   *        string self
   *        string channel
   *    string modifiedAt
   *    bool isDefault
   *    string type
   *    string id
   *    string createdAt
   */
  public function getChannelSections($channel_id) {

    $this->api_body = FALSE;
    $ch = $this->ch;
    curl_setopt($ch, CURLOPT_POST, FALSE);
    $url = $this->api_host . '/channels/' . $channel_id . '/sections';
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET');
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json', $this->getAuthorizationHeader('GET', $url)));
    curl_setopt($ch, CURLOPT_VERBOSE, $this->debug);

    $response_body = curl_exec($ch);

    $info = curl_getinfo($ch);

    if(curl_error($ch)) {
      throw new AppleNews_Error("API call failed: " . curl_error($ch));
    }

    $result = json_decode($response_body, true);

    if(floor($info['http_code'] / 100) >= 4) {
        throw $this->castErrors($result);
    }

    return $result['data'];

  }

  /**
   * Get the content for a section
   * @param string $section_id
   * @return associative_array containing all content for the section
   */
  public function getSection($section_id) {

    $this->api_body = FALSE;
    $ch = $this->ch;
    curl_setopt($ch, CURLOPT_POST, FALSE);
    $url = $this->api_host . '/sections/' . $section_id;
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET');
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json', $this->getAuthorizationHeader('GET', $url)));
    curl_setopt($ch, CURLOPT_VERBOSE, $this->debug);

    $response_body = curl_exec($ch);

    $info = curl_getinfo($ch);

    if(curl_error($ch)) {
      throw new AppleNews_Error("API call failed: " . curl_error($ch));
    }

    $result = json_decode($response_body, true);

    if(floor($info['http_code'] / 100) >= 4) {
        throw $this->castErrors($result);
    }

    return $result['data'];

  }

  /**
   * Get the content for an article
   * @param string $article_id
   * @return associative_array containing all content for the article
   */
  public function getArticle($article_id) {

    $this->api_body = FALSE;

    $ch = $this->ch;
    curl_setopt($ch, CURLOPT_POST, FALSE);
    $url = $this->api_host . '/articles/' . $article_id;
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET');
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json', $this->getAuthorizationHeader('GET', $url)));
    curl_setopt($ch, CURLOPT_VERBOSE, $this->debug);

    $response_body = curl_exec($ch);

    $info = curl_getinfo($ch);

    if(curl_error($ch)) {
      throw new AppleNews_Error("API call failed: " . curl_error($ch));
    }

    $result = json_decode($response_body, true);

    if(floor($info['http_code'] / 100) >= 4) {
        throw $this->castErrors($result);
    }

    return $result['data'];

  }

  /**
   * POST the article to a channel
   * @param string $channel_id
   * @param string $json_string - article.json string; Can be empty when
   *    passing a path to article.json in the $files array
   * @param array $files - array of full paths to files to upload
   * @param array $sections - array of section ids to which this article should be added
   * @param bool $article_id - The article id, to perform an update
   * @param bool $revision - If updating, the revision is required
   * @param bool $is_preview - Whether the article is a preview
   * @return array associative_array
   *    string createdAt
   *    string modifiedAt
   *    string id
   *    string type
   *    array links
   *        string channel
   *        string self
   *    bool isCandidateToBeFeatured
   *    bool isSponsored
   *    bool shareUrl
   */
  public function postArticle($channel_id, $json_string = '', $files = array(), $sections = array(), $article_id = false, $revision = false, $is_preview = false) {
    
    $ch = $this->ch;
    
    if ($article_id) {
      $url = $this->api_host . '/articles/' . $article_id;
    } else {
      $url = $this->api_host . '/channels/' . $channel_id . '/articles';
    }

    $boundary = $this->generateRandomString();

    $this->api_body = "";

    if (!empty($sections) || !empty($revision)) {
      $this->api_body .= $this->_part($boundary, "application/json", "metadata", $this->_metadata($sections, $revision, $is_preview));
    }

    if ($json_string) {
      $this->api_body .= $this->_part($boundary, "application/json", "article.json", $json_string);
    }

    foreach($files as $file_path) {
      $name = basename(urldecode($file_path));
      $name = str_replace(' ', '-', $name);
      
      if (substr($file_path, 0, 1) == '/') {
        // Local file system path
        $byte_string = file_get_contents($file_path);
        $mime = $name == "article.json" ? "application/json" : $this->getFileMimeType($file_path);
      } else {
        // External file URL
        $external_file = $this->getExternalFile($file_path);
        $byte_string = $external_file['data'];
        $mime = $external_file['mime'];
      }
      
      $this->api_body .= $this->_part($boundary, $mime, $name, $byte_string);
    }

    $this->api_body .= "--$boundary--";
    $content_type = 'multipart/form-data; boundary='.$boundary;
    $headers = array(
      'Accept: application/json',
      'Content-Type: '.$content_type,
      $this->getAuthorizationHeader('POST', $url, $content_type)
    );

    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'POST');
    curl_setopt($ch, CURLOPT_POST, true);

    curl_setopt($ch, CURLOPT_POSTFIELDS, $this->api_body);

    curl_setopt($ch, CURLOPT_VERBOSE, $this->debug);


    $response_body = curl_exec($ch);

    $info = curl_getinfo($ch);

    if(curl_error($ch)) {
      throw new AppleNews_Error("API call failed: " . curl_error($ch));
    }

    $result = json_decode($response_body, true);

    if(floor($info['http_code'] / 100) >= 4) {
        throw $this->castErrors($result);
    }

    return $result['data'];
  }

  /**
   * Delete the article
   * @param string $article_id
   * @return bool
   */
  public function deleteArticle($article_id) {

    $this->api_body = FALSE;

    $ch = $this->ch;
    curl_setopt($ch, CURLOPT_POST, FALSE);
    $url = $this->api_host . '/articles/' . $article_id;
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json', $this->getAuthorizationHeader('DELETE', $url)));
    curl_setopt($ch, CURLOPT_VERBOSE, $this->debug);

    $response_body = curl_exec($ch);

    $info = curl_getinfo($ch);

    if(curl_error($ch)) {
      throw new AppleNews_Error("API call failed: " . curl_error($ch));
    }

    $result = json_decode($response_body, true);

    if(floor($info['http_code'] / 100) >= 4) {
        throw $this->castErrors($result);
    }

    return true;

  }

  /**
   * Get the authentication header string
   * @param string $http_method
   * @param string $url
   * @return associative_array containing all content for the channel
   */
  private function getAuthorizationHeader($http_method = 'GET', $url, $content_type = false) {
    /**
     *  Per api security documentation
     */

    /**
     *  1. Generate a sting from :
     *  -HTTP method
     *  -The full URL of the request
     *  -The current date in ISO 8601 format
     *
     *  If the request includes an entity (for POST requests):
     *
     *  -The value of the Content-Type header
     *  -The full content of the entity
     */
    $date = date(DateTime::ISO8601);
    $canonical_request = $http_method . $url . $date;

    if ($this->api_body) {
      $canonical_request .= $content_type . $this->api_body;
    }

    /**
     *  2. Decode the API key's secret from Base64 to raw bytes.
     */
    $secret = base64_decode($this->api_secret);

    /**
     *  3. Create the hash using HMAC SHA-256 over the canonical request with the decoded API key secret.
     */
    $hashed = hash_hmac('sha256', $canonical_request, $secret , true);

    /**
     *  4. Encode the hash with Base64.
     */
    $api_signature = base64_encode($hashed);

    /**
     * 5. Set the Authorization header as:
     *    -Authorization: HHMAC; key=<api-key-id>; signature=<hash>; date=<date>
     */
    return "Authorization: HHMAC; key={$this->api_key}; signature={$api_signature}; date={$date}";
  }

  private function castErrors($result) {
    $errors = is_array($result['errors']) ? $result['errors'] : array();
    $errors_formatted = array();

    foreach ($errors as $error) {
      $errors_formatted[] = (isset(self::$error_map[$error['code']])) ? self::$error_map[$error['code']] : 'We received an unexpected error: ' . json_encode($error);
    }

    $error_message = sizeof($errors_formatted) ? implode("\n", $errors_formatted) : 'We received an unexpected error: ' . json_encode($result);
    throw new AppleNews_Error($error_message);
  }

  private function generateRandomString($length = 32) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
  }

  /**
   * Get the file mime type
   * @param string $file - path to the file
   * @return string - mime type
   */

  private function getFileMimeType($file) {
    if (function_exists("finfo_file")) {
      $finfo = finfo_open(FILEINFO_MIME_TYPE); // return mime type ala mimetype extension
      $mime = finfo_file($finfo, $file);
      finfo_close($finfo);
      return $mime;
    } else if (function_exists("mime_content_type")) {
      return mime_content_type($file);
    } else if (!stristr(ini_get("disable_functions"), "shell_exec")) {
      $file = escapeshellarg($file);
      $mime = shell_exec("file -bi " . $file);
      return $mime;
    } else {
      return false;
    }
  }

  private function _part($boundary, $type, $name, $byte_string) {
    $part = "--$boundary\r\n";
    $part .= "Content-Type: $type\r\n";
    $part .= "Content-Disposition: form-data; name=$name; filename=$name; size=".mb_strlen($byte_string, '8bit')."\r\n";
    $part .= "\r\n";
    $part .= $byte_string;
    $part .= "\r\n";

    return $part;
  }

  /**
   * Add metadata to the article being posted.
   * @param array $section_ids  - An array of section ids to post the article to
   * @param array $revision - If updating the article, supply a revision
   * @param bool $is_preview - Whether the article being posted is a preview
   * @return json
   */
  private function _metadata($section_ids = array(), $revision = FALSE, $is_preview = TRUE) {
    $data = (object) array();
    $json_string = json_encode((object) array("data" => $data));

    $hash = json_decode($json_string);

    if (!empty($section_ids)) {
      //metadata needs sections as urls
      $prepend = $this->api_host . "/sections/";

      foreach($section_ids as $k => $v) {
        $section_ids[$k] = $prepend . $v;
      }

      $hash->data->links = (object) array();
      $hash->data->links->sections = $section_ids;
    }
    
    if (!empty($revision)) {
      $hash->data->revision = $revision;
    }
    
    $hash->data->isPreview = $is_preview;

    return json_encode($hash);
  }
  
  public function getExternalFile($url) {
    $ch = curl_init();
    
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1);
    
    $data = curl_exec($ch);

    if(curl_error($ch)) {
      throw new AppleNews_Error("API call failed: " . curl_error($ch));
    }
    
    $content_type_header = curl_getinfo($ch, CURLINFO_CONTENT_TYPE);
    $content_type_parts = explode(';', $content_type_header);
    $mime = trim($content_type_parts[0]);
    
    curl_close($ch);
    
    return array(
      'mime' => $mime,
      'data' => $data,
    );
  }

}

/**
 * Class AppleNews_Error
 */

class AppleNews_Error extends Exception {}
