<?php

/**
 * @file
 * Implementation of basic interaction with the Blizzard Community Platform API.
 */

/**
 * Base Blizzard API class.
 *
 * This class provides a Drupal interface to the Blizzard Community Platform
 * API, including but not limited to: request and error handling, caching, and
 * locale validation.
 */
abstract class BlizzardApi implements BlizzardApiLocalizationInterface {
  const API_HOST = '//{region}.api.battle.net';
  const API_HOST_CHINA = '//api.battlenet.com.cn';
  
  const URL_HOST = '//{region}.battle.net';
  const URL_HOST_CHINA = '//www.battlenet.com.cn';
  
  const URL_MEDIA_HOST = '//{region}.media.blizzard.com';
  const URL_MEDIA_HOST_CHINA = '//content.battlenet.com.cn';
  
  const REGION_US = 'us';
  const REGION_EU = 'eu';
  const REGION_KR = 'kr';
  const REGION_TW = 'tw';
  const REGION_CN = 'cn';
  const REGION_SEA = 'sea';
  
  const LANGUAGE_DEFAULT   = '';
  const LANGUAGE_CHINESE   = 'zh';
  const LANGUAGE_ENGLISH   = 'en';
  const LANGUAGE_FRENCH    = 'fr';
  const LANGUAGE_GERMAN    = 'de';
  const LANGUAGE_ITALIAN   = 'it';
  const LANGUAGE_KOREAN    = 'ko';
  const LANGUAGE_POLISH    = 'pl';
  const LANGUAGE_PORTUGUES = 'pt';
  const LANGUAGE_RUSSIAN   = 'ru';
  const LANGUAGE_SPANISH   = 'es';
  const LANGUAGE_TURKISH   = 'tr';
  
  /**
   * The application or IP address has been blocked from making further
   * requests. This ban may not be permanent.
   */
  const ERROR_ACCESS_DENIED = -1;
  /**
   * A request was made including application identification information, but
   * either the application key is invalid or missing.
   */
  const ERROR_INVALID_APPLICATION = -2;
  /**
   * The application authorization information was malformed or missing when
   * expected.
   *
   * @deprecated
   */
  const ERROR_INVALID_HEADER = -3;
  /**
   * A request was made to an API resource that requires a higher application
   * permission level.
   *
   * @deprecated
   */
  const ERROR_INVALID_PERMISSIONS = -4;
  /**
   * The request date was outside of a 15 second window from the current GMT time.
   *
   * @deprecated
   */
  const ERROR_INVALID_SIGNATURE = -5;
  /**
   * A request was made to a resource that does not exist.
   */
  const ERROR_NOT_FOUND = -6;
  /**
   * The application or IP has been throttled.
   */
  const ERROR_TOO_MANY_REQUESTS = -7;
  /**
   * There was a server error or equally catastrophic exception preventing the
   * request from being fulfilled.
   */
  const ERROR_SERVER_FAULT = -8;
  
  /**
   * The API resource path (not including the URL host).
   *
   * @var string
   */
  protected $apiPath = '';
  
  /**
   * The maximum time (in seconds) that retrieved data should be stored in the
   * local cache. Defaults to 30 days.
   * 
   * If the server specifies a cache-control directive, this property is ignored.
   * 
   * @var int
   */
  protected $cacheLifetime = 2592000;
  
  /**
   * A prefix used in the cache ID of a stored API response.
   *
   * @var string
   */
  protected $cachePrefix = '';
  
  /**
   * The locale tag to append to the URL query.
   *
   * @var string
   */
  protected $customLocale = '';
  
  /**
   * Data returned from the Battle.net API.
   *
   * @var mixed
   */
  protected $data;
  
  /**
   * A list of fields that will be added to the URL.
   *
   * The value of a key that requires multiple values may also be an array.
   * @code
   * // Example of a single realm field.
   * $fields = array('realms' => 'medivh');
   * // Example of multiple character fields.
   * $fields = array('fields' => array('guild', 'items', 'professions', 'reputations', 'stats'));
   * @endcode
   *
   * @var array
   *
   * @see BlizzardApi::buildUrl()
   */
  protected $fields = array();
  
  /**
   * A supported Battle.net language that data will be retrieved in.
   *
   * @var string
   */
  protected $language;
  
  /**
   * The current state of retrieved data.
   *
   * @var bool
   */
  protected $modified = FALSE;
  
  /**
   * List of placeholders in the API path.
   *
   * @var array
   *
   * @see BlizzardApi::addPlaceholder()
   */
  protected $placeholders = array();
  
  /**
   * A supported Battle.net region that data will be retrieved in.
   *
   * @var string
   */
  protected $region;
  
  /**
   * List of placeholder replacements in the API path.
   *
   * @var array
   *
   * @see BlizzardApi::addPlaceholder()
   */
  protected $replacements = array();
  
  /**
   * The full URL of the request to send.
   *
   * @var string
   *
   * @see BlizzardApi::buildUrl()
   */
  protected $url = '';
  
  /**
   * Constructs a BlizzardApi object.
   *
   * @param string $region
   *   (optional) The Battle.net region to request data from.
   *   Valid regions include:
   *   - us: Americas (default)
   *   - eu: Europe
   *   - kr: Korea
   *   - tw: Taiwan
   *   - cn: China
   * @param string $language
   *   (optional) The language to request data in.
   *   - If not provided, the site default language may be used.
   *   - If the language is not supported, the Battle.net region default will
   *     be used.
   */
  public function __construct($region = BlizzardApi::REGION_US, $language = BlizzardApi::LANGUAGE_DEFAULT) {
    $this->setLocale($region, $language);
    
    if (!extension_loaded('openssl')) {
      throw new BlizzardApiException('Extension \'OpenSSL\' not found');
    }
  }
  
  /**
   * Adds a placeholder and replacement to use when building the API URL. If the
   * placeholder already exists, the replacement will be updated.
   *
   * @param string $placeholder
   *   The value to search for and replace in the URL.
   * @param string $replacement
   *   A string to replace the placeholder with. This value will be encoded for
   *   use in the API URL.
   * 
   * @see BlizzardApi::clearPlaceholders(), BlizzardApi::removePlaceholder()
   */
  protected function addPlaceholder($placeholder, $replacement) {
    $i = array_search($placeholder, $this->placeholders);
    if ($i === FALSE) {
      $this->placeholders[] = $placeholder;
      $this->replacements[] = rawurlencode($replacement);
    }
    else {
      $this->replacements[$i] = rawurlencode($replacement);
    }
    
    return $this->resetProperties();
  }
  
  /**
   * Compiles a full URL to request data from.
   *
   * This method will select a proper URL scheme and host for the selected
   * Battle.net region, then replace any placeholders. An encoded URL query will
   * also be built including any fields and the locale.
   *
   * @throws BlizzardApiException
   */
  protected function buildUrl() {
    $query = array();
    
    // Build the query.
    if (!empty($this->fields)) {
      foreach ($this->fields as $key => $value) {
        // Skip this parameter if there is no value.
        if (!empty($value)) {
          $query[$key] = is_array($value) ? implode(',', array_map('rawurlencode', $value)) : rawurlencode($value);
        }
      }
    }
    
    if (!empty($this->customLocale)) {
      $query['locale'] = $this->customLocale;
    }
    
    $key = variable_get('blizzardapi_public_key', '');
    if (empty($key)) {
      throw new BlizzardApiException('A public key is required to access this resource');
    }
    
    // The api key is now a required query parameter.
    $params = array('apikey=' . rawurlencode($key));
    foreach ($query as $key => $value) {
      $params[] = $key . '=' . $value;
    }
    
    $query = implode('&', $params);
    
    // Construct the URL.
    $this->addPlaceholder('{region}', $this->region);
    
    $this->url = $this->getApiHost() . $this->apiPath;
    if (!empty($query)) {
      // Note: During SimpleTest runs, the host may already have a query if the
      // clean URL feature is disabled.
      $this->url .= (strpos($this->url, '?') === FALSE ? '?' : '&') . $query;
    }
    
    $this->url = str_replace($this->placeholders, $this->replacements, $this->url);
  }
  
  /**
   * Clears all stored placeholders and replacements.
   *
   * @see BlizzardApi::addPlaceholder(), BlizzardApi::removePlaceholder()
   */
  protected function clearPlaceholders() {
    $this->placeholders = array();
    $this->replacements = array();
    return $this->resetProperties();
  }
  
  /**
   * Decompress and then decode a supplied JSON string.
   *
   * @param string $data
   *   A string containing the request data.
   * @param string $encoding
   *   (optional) The encoding format of the supplied data.
   *   Supported formats: gzip.
   *
   * @return mixed
   *   The values encoded in the JSON string as an appropriate PHP value.
   *
   * @throws BlizzardApiJsonException
   */
  protected static function decodeJson($data, $encoding = '') {
    $json = NULL;
    
    switch ($encoding) {
      case 'gzip':
        $json = gzinflate(substr($data, 10, -8));
        break;
      default:
        $json = $data;
    }
    
    $json = json_decode($json, TRUE);
    
    if (is_null($json)) {
      // Since PHP 5.3.0
      // Force a generic message otherwise.
      $code = function_exists('json_last_error') ? json_last_error() : -1000;
      throw new BlizzardApiJsonException($code);
    }
    
    return $json;
  }
  
  /**
   * Find values in a data set that match the specified filter.
   *
   * @param array $data
   *   A list of key-value pairs to look through.
   * @param mixed $key
   *   The key to use when selecting a value.
   * @param mixed $filter
   *   A simple value, array of values, or function callback to use as the filter.
   *
   * @return array
   *   A list of values that match the given filter.
   */
  public static function filterBy($data, $key, $filter) {
    $results = array();
    
    if (empty($data)) {
      return $results;
    }
    
    foreach ($data as $item) {
      if (is_array($filter) && in_array($item[$key], $filter)) {
        $results[] = $item;
      }
      else if ($item[$key] == $filter) {
        $results[] = $item;
      }
      else if (function_exists($filter) && ($filter($item[$key]) == TRUE)) {
        $results[] = $item;
      }
    }
    
    return $results;
  }
  
  /**
   * Returns the appropriate URL host for the given region.
   */
  protected function getApiHost() {
    $host = ($this->region == self::REGION_CN) ? self::API_HOST_CHINA : self::API_HOST;
    return 'https:' . $host;
  }
  
  /**
   * Returns the current API path, this does not include the URL host.
   */
  public function getApiPath() {
    return $this->apiPath;
  }
  
  /**
   * Returns the JSON data as an appropriate PHP type.
   */
  public function getData() {
    return $this->data;
  }
  
  /**
   * Determine an appropriate error code given the supplied reason.
   *
   * Also, would it have killed Blizzard to give us an error code...
   *
   * @param string $reason
   *   The error reason specified in the request's JSON data.
   *
   * @return mixed
   *   An error code specified by this class or FALSE if the reason does not
   *   have an associated code.
   *
   * @link http://support.mashery.com/docs/read/mashery_api/20_reporting/error_codes Error Codes & Responses @endlink
   */
  protected static function getErrorCode($reason) {
    // This list only includes Mashery errors. Those sent by Blizzard are not
    // included and require manual processing.
    $errors = array(
      // "You have not been granted permission to access the requested method or object."
      self::ERROR_ACCESS_DENIED       => 'Forbidden',
      // "The API key associated with your request was not recognized or the signature was incorrect."
      self::ERROR_ACCESS_DENIED       => 'Not Authorized',
      // "The API key you are using to access the Mashery API has not been approved or has been disabled."
      self::ERROR_INVALID_APPLICATION => 'Account Inactive',
      // "The API key you are using has attempted to access the api too many times in one second."
      self::ERROR_TOO_MANY_REQUESTS   => 'Account Over Queries Per Second Limit',
      // "The API key you are using has attempted to access the api too many times in the rate limiting period."
      self::ERROR_TOO_MANY_REQUESTS   => 'Account Over Rate Limit',
      // "The service you have requested is over-capacity."
      self::ERROR_SERVER_FAULT        => 'Rate Limit Exceeded'
    );
    
    // Probably not the fastest method, but it works, for now.
    return array_search($reason, $errors);
  }
  
  /**
   * Determine a proper cache expiration time based on HTTP response headers.
   * If the response does not contain this information, the specified cache
   * lifetime will be used instead.
   *
   * @param array $headers
   *   The API request's HTTP response headers.
   *
   * @return int
   *   A cache expiration timestamp, CACHE_PERMANENT, or CACHE_TEMPORARY.
   */
  protected function getExpiration($headers) {
    if ($this->cacheLifetime == CACHE_PERMANENT || $this->cacheLifetime == CACHE_TEMPORARY) {
      return $this->cacheLifetime;
    }
    
    $expire = REQUEST_TIME + $this->cacheLifetime;
    
    if (isset($headers['expires'])) {
      $expire = strtotime($headers['expires']);
      if ($expire === FALSE) {
        $expire = CACHE_TEMPORARY;
        watchdog('blizzardapi',
          'Unable to get expiration time from %resource. Results will be removed during the next general cache wipe.',
          array('%resource' => $this->url), WATCHDOG_WARNING);
      }
    }
    
    // RFC 2616: the 'max-age' directive overrides the 'Expires' field (section 14.21).
    if (isset($headers['cache-control'])) {
      $cache_control = explode(',', $headers['cache-control']);
      foreach ($cache_control as $directive) {
        // Skip directives with no value like 'no-cache'.
        if (strpos($directive, '=') === FALSE) {
          break;
        }
        list($key, $value) = explode('=', trim($directive), 2);
        if ($key == 'max-age') {
          $expire = REQUEST_TIME + $value;
          break;
        }
      }
    }
    
    return $expire;
  }
  
  /**
   * Returns a list of fields to be added to the URL.
   */
  public function getFields() {
    return $this->fields;
  }
  
  /**
   * Returns the language that the API will request data in.
   */
  public function getLanguage() {
    return $this->language;
  }
  
  /**
   * Returns a list of valid API locales.
   *
   * Caution: Unlike BlizzardApi::getSupportedLocales() the data returned by
   * this method may change depending on the context used during runtime. This
   * is basically a workaround to not having support for late static binding.
   */
  abstract protected function getLocales();
  
  /**
   * Returns the region that the API will request data from.
   */
  public function getRegion() {
    return $this->region;
  }
  
  /**
   * Returns TRUE after sending a request that results in updated data being
   * retrieved.
   */
  public function isModified() {
    return $this->modified;
  }
  
  /**
   * Build the request URL and attempt to get API data from the local cache.
   *
   * @return mixed
   *   The cache object or FALSE if the request was not cached.
   */
  public function loadFromCache() {
    $this->buildUrl();
    
    $cache = cache_get($this->cachePrefix . $this->url, 'cache_blizzardapi');
    if ($cache === FALSE) {
      return FALSE;
    }
    
    $response = $cache->data;
    $encoding = isset($response->headers['content-encoding']) ? $response->headers['content-encoding'] : '';
    $this->data = self::decodeJson($response->data, $encoding);
    $this->prepareData();
    
    return $cache;
  }
  
  /**
   * Extending classes should override this method if they wish to perform any
   * special tasks on the decoded JSON data.
   */
  protected function prepareData() {
    // Do nothing.
  }
  
  /**
   * Removes the first occurance of the placeholder and its associated
   * replacement.
   *
   * @see BlizzardApi::addPlaceholder(), BlizzardApi::clearPlaceholders()
   */
  protected function removePlaceholder($placeholder) {
    $i = array_search($placeholder, $this->placeholders);
    if ($i !== FALSE) {
      unset($this->placeholders[$i], $this->replacements[$i]);
    }
    return $this->resetProperties();
  }
  
  /**
   * Reset all properties that BlizzardApi::sendRequest() may have changed.
   */
  protected function resetProperties() {
    $this->data = NULL;
    $this->modified = FALSE;
    $this->url = '';
    return $this;
  }
  
  /**
   * Requests and caches data from the Blizzard Community Platform API.
   * Authentication and data compression are used in the request when appropriate.
   *
   * @param bool $refresh
   *   (optional) If TRUE, bypass the local cache and attempt to retrieve fresh
   *   data. Defaults to FALSE.
   * @param array $options
   *   (optional) Additional options to pass to drupal_http_request() and/or
   *   the following parameters:
   *   - delay: The delay period before a retry is attempted, in milliseconds.
   *     Defaults to 100ms (1/10th of a second).
   *   - retry: An integer representing how many times to retry a request if
   *     it fails due to a rate limit per second error. Defaults to 3.
   *   
   *   Note: Some headers such as 'If-Modified-Since' and 'Accept-Encoding' may
   *   already be supplied, and cannot be modified.
   *
   * @throws BlizzardApiException, BlizzardApiAuthenticationException,
   *   BlizzardApiAccessDeniedException, BlizzardApiNotFoundException,
   *   BlizzardApiServerException, BlizzardApiServiceException,
   *   BlizzardApiTooManyRequestsException
   */
  public function sendRequest($refresh = FALSE, $options = array()) {
    $headers = array();
    $options += array('headers' => array(), 'method' => 'GET', 'delay' => 100, 'retry' => 3);
    
    // NOTE: This method also builds the URL for us.
    $cache = $this->loadFromCache();
    if ($cache !== FALSE) {
      // Make sure the cached data is still good.
      if (REQUEST_TIME < $cache->expire || $cache->expire == CACHE_PERMANENT) {
        if ($refresh == FALSE) {
          return $this;
        }
      }
      
      // Even if the data is stale, it may still be possible to get a "304 Not
      // Modified" response.
      $headers['If-Modified-Since'] = $cache->data->headers['date'];
    }
    
    if (extension_loaded('zlib')) {
      $headers['Accept-Encoding'] = 'gzip';
    }
    
    $options['headers'] = $headers + $options['headers'];
    $response = drupal_http_request($this->url, $options);
    $encoding = isset($response->headers['content-encoding']) ? $response->headers['content-encoding'] : '';
    
    if (!empty($response->data)) {
      $this->data = self::decodeJson($response->data, $encoding);
    }
    
    $code = $response->code;
    if ($code > 500) {
      $code = floor($code / 100) * 100;  // Treat all server errors the same.
    }
    
    switch ($code) {
      case 200:  // OK
      case 302:  // Found
      case 307:  // Temporary redirect
        if (empty($this->data)) {
          throw new BlizzardApiException('A valid API request was made, but the response did not include data');
        }
        
        $this->prepareData();
        $this->modified = TRUE;
        
        $expire = $this->getExpiration($response->headers);
        cache_set($this->cachePrefix . $this->url, $response, 'cache_blizzardapi', $expire);
        break;
      case 304:  // Not modified
        // Update to the current request time plus the specified cache lifetime
        // or 'max-age' seconds (unless there was an 'Expires' header).
        $expire = $this->getExpiration($cache->data->headers);
        
        // RFC 2616: If a 304 response contains an 'Expires', 'Cache-Control' or
        // 'Vary' header, the value has probably changed from the original 200
        // response (section 10.3.5).
        if (isset($response->headers['expires']) || isset($response->headers['cache-control'])) {
          $expire = $this->getExpiration($response->headers);
          
          // "...the cache MUST update the entry to reflect any new field values
          // given in the response."
          if (isset($response->headers['expires'])) {
            $cache->data->headers['expires'] = $response->headers['expires'];
          }
          if (isset($response->headers['cache-control'])) {
            $cache->data->headers['cache-control'] = $response->headers['cache-control'];
          }
        }
        
        cache_set($cache->cid, $cache->data, 'cache_blizzardapi', $expire);
        break;
      case 403:  // Forbidden
        $reason = isset($this->data['detail']) ? $this->data['detail'] : '';
        
        if ($options['retry'] > 0 && $reason == 'Account Over Queries Per Second Limit') {
          $options['retry']--;
          usleep((int)$options['delay'] * 1000);
          return $this->sendRequest($refresh, $options);
        }
        
        $error_code = BlizzardApi::getErrorCode($reason);
        switch ($error_code) {
          case self::ERROR_ACCESS_DENIED:
            throw new BlizzardApiAccessDeniedException($reason, $error_code);
          case self::ERROR_INVALID_APPLICATION:
            throw new BlizzardApiAuthenticationException($reason, $error_code);
          case self::ERROR_TOO_MANY_REQUESTS:
            throw new BlizzardApiTooManyRequestsException($reason, $error_code);
          case self::ERROR_SERVER_FAULT:
            throw new BlizzardApiServiceException($reason, $error_code);
          default:
            watchdog('blizzardapi', 'Unhandled API error code from %resource. Reason: %detail',
              array('%resource' => $this->url, '%detail' => $reason), WATCHDOG_ERROR);
            throw new BlizzardApiException('Unhandled error code', $response->code);
        }
        break;
      case 404:  // Not found
        // Bad path and missing key? 403. Bad path and valid key? 404.
        throw new BlizzardApiNotFoundException('A request was made to a resource that does not exist', $response->code);
      case 500:  // Internal server error
        // @todo It is unknown if a '503 Service Unavailable' response is still due to maintenance mode.
        throw new BlizzardApiServerException('There was a server error preventing the request from being fulfilled', $response->code);
      default:
        // Network errors have no status message (and a negative code).
        $msg = isset($response->status_message) ? $response->status_message : $response->error;
        watchdog('blizzardapi', 'Unhandled API response code from %resource. Status: %code - %message',
          array('%resource' => $this->url, '%code' => $response->code, '%message' => $msg), WATCHDOG_ERROR);
        throw new BlizzardApiException('Unhandled response code', $response->code);
    }
    
    return $this;
  }
  
  /**
   * Set the current API path.
   *
   * Caution: Extending classes should most likely append to the current path.
   */
  protected function setApiPath($api_path, $placeholders = array()) {
    $this->apiPath = $api_path;
    
    if (!empty($placeholders)) {
      foreach ($placeholders as $placeholder => $replacement) {
        $this->addPlaceholder($placeholder, $replacement);
      }
    }
    
    return $this->resetProperties();
  }
  
  /**
   * Set the length of time that this API request should remain in the cache.
   *
   * @param int $lifetime
   *   One of the following values:
   *   - CACHE_PERMANENT
   *   - CACHE_TEMPORARY
   *   - The maximum length of time (in seconds) that a result should be cached.
   *     Must be greater than 0.
   *
   * @see BlizzardApi::setCachePrefix()
   */
  public function setCacheLifetime($lifetime) {
    if ($lifetime < CACHE_TEMPORARY) {
      throw new InvalidArgumentException('Argument must be one of: CACHE_PERMANENT, CACHE_TEMPORARY, or an integer greater than 0.');
    }
    
    $this->cacheLifetime = $lifetime;
    return $this;
  }
  
  /**
   * Set the prefix to use when storing an API response.
   *
   * Set a cache prefix when using non-standard cache lifetime values. This
   * ensures that requests made from other functions (or modules) do not become
   * invalid prematurely or too slowly.
   *
   * @param string $prefix
   *   The string to prefix the response's cache ID with.
   */
  public function setCachePrefix($prefix) {
    $this->cachePrefix = $prefix;
    return $this;
  }
  
  /**
   * Set the Battle.net locale to obtain API data from.
   *
   * Note: If this method is called after sending a request, the object's state
   * will be reset (any data obtained will be lost).
   *
   * @param string $region
   *   (optional) The Battle.net region to request data from.
   *   Valid regions include:
   *   - us: Americas (default), eu: Europe, kr: Korea, tw: Taiwan, cn: China
   * @param string $language
   *   (optional) The language to retrieve data in.
   *   - If not provided, the site default language will be used.
   *   - If the language is not supported, the Battle.net region default will
   *     be used.
   *
   * @see BlizzardApi::getSupportedLocales(), BlizzardApi::isSupportedLocale()
   */
  public function setLocale($region = BlizzardApi::REGION_US, $language = BlizzardApi::LANGUAGE_DEFAULT) {
    // Attempt to use the default site language.
    if (empty($language)) {
      $language = language_default('language');
    }
    
    $this->customLocale = '';
    $locales = $this->getLocales();
    
    // Default to the Americas if the region is unsupported.
    $this->region = array_key_exists($region, $locales) ? $region : BlizzardApi::REGION_US;
    
    // Get the region's default language, it is the first one listed.
    $default_language = key($locales[$this->region]);
    
    // Is this a supported language?
    if (array_key_exists($language, $locales[$this->region])) {
      $this->language = $language;
      
      // If the language is not the default, set the custom locale.
      if ($this->language != $default_language) {
        $this->customLocale = $locales[$this->region][$this->language];
      }
    }
    else {
      $this->language = $default_language;
    }
    
    return $this->resetProperties();
  }
}

/**
 * Interface for localizing an API request.
 */
interface BlizzardApiLocalizationInterface {
  /**
   * Returns the language that the API will request data in.
   */
  public function getLanguage();
  
  /**
   * Returns the region that the API will request data from.
   */
  public function getRegion();
  
  /**
   * Returns an array containing all the supported regions and languages.
   */
  public static function getSupportedLocales();
  
  /**
   * Checks if the supplied region and language are a valid Battle.net locale.
   */
  public static function isSupportedLocale($region, $language);
  
  /**
   * Checks if the supplied region is a valid Battle.net region.
   */
  public static function isSupportedRegion($region);
  
  /**
   * Set the Battle.net locale to obtain API data from.
   */
  public function setLocale($region, $language);
}

/**
 * -----------------------------------------------------------------------------
 * Exceptions
 * -----------------------------------------------------------------------------
 */

/**
 * Exception thrown when an unknown error has occured.
 */
class BlizzardApiException extends Exception {
  public function __construct($message = '', $code = 0, $previous = NULL) {
    // Since PHP 5.3.0
    if (method_exists($this, 'getPrevious')) {
      parent::__construct($message, $code, $previous);
    }
    else {
      parent::__construct($message, $code);
    }
  }
}

/**
 * Exception thrown when JSON data cannot be decoded.
 */
class BlizzardApiJsonException extends BlizzardApiException {
  public function __construct($code = JSON_ERROR_NONE, $previous = NULL) {
    $message = BlizzardApiJsonException::getJsonErrorMessage($code);
    parent::__construct($message, $code, $previous);
  }
  
  protected static function getJsonErrorMessage($code) {
    switch ($code) {
      case JSON_ERROR_CTRL_CHAR:
        return 'Control character error, possible incorrect encoding';
      case JSON_ERROR_DEPTH:
        return 'The maximum stack depth has been exceeded';
      case JSON_ERROR_NONE:
        return 'No error has occured';
      case JSON_ERROR_STATE_MISMATCH:
        return 'Invalid or malformed JSON';
      case JSON_ERROR_SYNTAX:
        return 'Syntax error';
      default:
        // Since PHP 5.3.3
        if (defined(JSON_ERROR_UTF8) && $code == JSON_ERROR_UTF8) {
          return 'Malformed UTF-8 characters, possible incorrect encoding';
        }
        // Since PHP 5.5.0
        else if (defined(JSON_ERROR_UNSUPPORTED_TYPE) && $code == JSON_ERROR_UNSUPPORTED_TYPE) {
          return 'A value of a type that cannot be encoded was given';
        }
        return 'Unable to decode JSON string';
    }
  }
}

/**
 * Exception thrown when the application or IP has been blocked from making
 * further requests.
 */
class BlizzardApiAccessDeniedException extends BlizzardApiException {}

/**
 * Exception thrown when there is an authentication failure. The exception code
 * will contain the specific cause.
 */
class BlizzardApiAuthenticationException extends BlizzardApiException {}

/**
 * Exception thrown when a method call is invalid for the object's current state.
 */
class BlizzardApiInvalidOperationException extends BlizzardApiException {}

/**
 * Exception thrown when a requested resource could not be found.
 */
class BlizzardApiNotFoundException extends BlizzardApiException {}

/**
 * Exception thrown when a request cannot be completed due to an unknown server
 * error.
 */
class BlizzardApiServerException extends BlizzardApiException {}

/**
 * Exception thrown when a request cannot be completed due to a temporary
 * service disruption.
 *
 * Applications should "back off" to reduce load on the system.
 */
class BlizzardApiServiceException extends BlizzardApiException {}

/**
 * Exception thrown when an application or IP has been throttled.
 */
class BlizzardApiTooManyRequestsException extends BlizzardApiException {}
