<?php
/**
 * @file
 * Defines the base classes for use with the webclient module.
 */

/**
 * Defines the header interaction.
 */
interface WebHeaderInterface {

  /**
   * Get the headers.
   *
   * @return WebHeaderCollection
   *   An WebHeaderCollection object containing the headers.
   */
  public function getHeaders();

}

/**
 * Defines the base for handling formatted response data.
 */
class WebResponseFormatter {

  /**
   * Get the formatted response for the specified MIME type.
   *
   * @param string $mime_type
   *   The MIME type that applies to the response data.
   * @param string $data
   *   The response data.
   *
   * @return string
   *   A string containing the response data as received.
   */
  public function format($mime_type, $data) {
    // Return the data as received.
    return $data;
  }

}

/**
 * Contains protocol headers associated with a request or response.
 */
class WebHeaderCollection implements ArrayAccess, Iterator, Countable {

  /*
   * Class members.
   */

  private $isResponse = FALSE;
  private $headers = array();
  private $iterator = array();
  private $iteratorPosition = 0;
  private $iteratorRebuild = FALSE;

  /*
   * HttpHeaderCollection implementation.
   */

  /**
   * Create a new HttpHeaderCollection object.
   *
   * @param bool $response
   *   Indicate whether the header collection is used for an HTTP response.
   */
  public function __construct($response = FALSE) {
    // Set the type of header collection.
    $this->isResponse = $response;
  }

  /**
   * Add the given header and value.
   *
   * @param string $header
   *   The HTTP header.
   * @param mixed $value
   *   The value which will be linked to the header.
   */
  public function add($header, $value = NULL) {
    // Check if the header can be used in the current context.
    if (WebHeaderCollection::isValidHeader($header, $this->isResponse)) {
      // Check if the value is a valid header value.
      if (WebHeaderCollection::isValidValue($value)) {
        // Add the header to the headers.
        $this->addWithoutValidate($header, $value);
      }
      else {
        // Throw exception: Value is not valid.
        throw new InvalidArgumentException("Invalid value was specified: {$value}");
      }
    }
    else {
      // Throw exception: Header is not valid.
      throw new InvalidArgumentException("Invalid header was specified: {$header}");
    }
  }

  /**
   * Parse the header data and add it to the header collection.
   *
   * @param string $data
   *   The raw header data.
   */
  public function addRaw($data) {
    // Extract the header and value information.
    list($header, $value) = explode(':', $data) + array(NULL, NULL);
    // Check if the header and value is valid.
    if (WebHeaderCollection::isValidHeader($header, $this->isResponse) && WebHeaderCollection::isValidValue($value)) {
      // Add the header information.
      $this->addWithoutValidate($header, trim($value));
    }
  }

  /**
   * Add the header and value without validation.
   *
   * @param string $header
   *   The HTTP header.
   * @param mixed $value
   *   The value which will be linked to the header.
   */
  protected function addWithoutValidate($header, $value) {
    // Check if rebuilding the iterator is required.
    $rebuild_iterator = !$this->has($header);
    // Add the header and value to the headers.
    $this->headers[$header] = $value;
    // Check if the iterator rebuild flag needs to be set.
    if ($rebuild_iterator) {
      // Mark the iterator for rebuild.
      $this->setIteratorRebuild();
    }
  }

  /**
   * Remove the given header.
   *
   * @param string $header
   *   The header to remove.
   */
  public function remove($header) {
    // Check if the header is present in the headers.
    if ($this->has($header)) {
      // Remove the header from the headers.
      unset($this->headers[$header]);
      // Mark the iterator for rebuild.
      $this->setIteratorRebuild();
    }
  }

  /**
   * Get a value indicating whether the header is present.
   *
   * @param string $header
   *   The header to check.
   */
  public function has($header) {
    // Check if the header is defined in the headers.
    return array_key_exists($header, $this->headers);
  }

  /**
   * Get the value for the given header.
   *
   * @param string $header
   *   The header for which the value is requested.
   *
   * @return mixed
   *   Mixed value which is linked to the header.
   */
  public function &get($header) {
    $value = NULL;
    // Check if the header is present.
    if ($this->has($header)) {
      // Get the header value by reference.
      $value = &$this->headers[$header];
    }
    return $value;
  }

  /**
   * Get a value indicating whether the given header is valid.
   *
   * @param string $header
   *   The HTTP header.
   * @param bool $response
   *   Indicate in which context the validation should occur.
   *
   * @return bool
   *   TRUE if the header is valid, otherwise FALSE.
   */
  public static function isValidHeader($header, $response = FALSE) {
    // Initialize the HTTP headers list.
    static $http_headers = array(
      'Cache-Control' => 3,
      'Connection' => 3,
      'Date' => 3,
      'Keep-Alive' => 3,
      'Pragma' => 3,
      'Trailer' => 3,
      'Transfer-Encoding' => 3,
      'Upgrade' => 3,
      'Via' => 3,
      'Warning' => 3,
      'Allow' => 3,
      'Content-Length' => 3,
      'Content-Type' => 3,
      'Content-Encoding' => 3,
      'Content-Language' => 3,
      'Content-Location' => 3,
      'Content-MD5' => 3,
      'Content-Range' => 3,
      'Expires' => 3,
      'Last-Modified' => 3,
      'Accept' => 1,
      'Accept-Charset' => 1,
      'Accept-Encoding' => 1,
      'Accept-Language' => 1,
      'Accept-Ranges' => 2,
      'Age' => 2,
      'Authorization' => 1,
      'Cookie' => 1,
      'ETag' => 2,
      'Expect' => 1,
      'From' => 1,
      'Host' => 1,
      'If-Match' => 1,
      'If-Modified-Since' => 1,
      'If-None-Match' => 1,
      'If-Range' => 1,
      'If-Unmodified-Since' => 1,
      'Location' => 2,
      'Max-Forwards' => 1,
      'Proxy-Authorization' => 3,
      'Range' => 1,
      'Referer' => 1,
      'Retry-After' => 2,
      'Server' => 2,
      'Set-Cookie' => 2,
      'Te' => 1,
      'Translate' => 1,
      'User-Agent' => 1,
      'Vary' => 2,
      'WWW-Authenticate' => 2,
    );
    $valid = FALSE;
    // Check if the header is a non-empty string an is defined in the above
    // list.
    if (!empty($header) && is_string($header)) {
      // Check if the header is predefined.
      if (isset($http_headers[$header])) {
        // Get the restriction value for the header.
        $restriction = $http_headers[$header];
        // Determine the restriction parameter.
        $restriction_param = $response ? 2 : 1;
        // Determine whether the header is valid.
        $valid = ($restriction & $restriction_param) == $restriction_param;
      }
      // Check if the header is non-standard.
      elseif (strpos($header, 'X-') === 0 || strpos($header, 'x-') === 0) {
        // Mark the header as valid.
        $valid = TRUE;
      }
    }
    return $valid;
  }

  /**
   * Get a value indicating whether the given value is valid.
   *
   * @param mixed $value
   *   The value to validate.
   *
   * @return bool
   *   TRUE if the value is valid, otherwise FALSE.
   */
  public static function isValidValue($value) {
    // Determine whether the value is empty or shorter then 65536 characters.
    return empty($value) || (is_scalar($value) && drupal_strlen((string) $value) <= 65535);
  }

  /*
   * ArrayAccess interface implementation.
   */

  /**
   * Get a value indicating whether the offset is present in the collection.
   *
   * @param string $offset
   *   The offset to check for.
   *
   * @return bool
   *   TRUE if the offset is present in the collection, otherwise FALSE.
   */
  public function offsetExists($offset) {
    // Check if the header collection contains the header.
    return $this->has($offset);
  }

  /**
   * Get the value corresponding to the offset.
   *
   * @param string $offset
   *   The offset to which the value is linked.
   *
   * @return mixed
   *   A mixed value if the offset is present, otherwise NULL.
   */
  public function offsetGet($offset) {
    // Get the value at the given offset.
    return $this->get($offset);
  }

  /**
   * Set the value for the given offset.
   *
   * @param string $offset
   *   The offset which needs to be added.
   * @param mixed $value
   *   The value which needs to be linked to the offset.
   */
  public function offsetSet($offset, $value) {
    // Add the header and value to the header collection.
    $this->add($offset, $value);
  }

  /**
   * Remove the offset from the collection.
   *
   * @param string $offset
   *   The offset to remove.
   */
  public function offsetUnset($offset) {
    // Remove the header from the header collection.
    $this->remove($offset);
  }

  /*
   * Iterator interface implementation.
   */

  /**
   * Get the key for the current iterator position.
   *
   * @return string
   *   The key corresponding to the current iterator position.
   */
  public function key() {
    // Get the key/value pair from the iterator.
    $iterator_item = $this->getIteratorItem($this->iteratorPosition);
    // Get the key from the iterator item;
    return isset($iterator_item) ? $iterator_item['key'] : NULL;
  }

  /**
   * Get the value for the current iterator position.
   *
   * @return mixed
   *   The value corresponding to the current iterator position.
   */
  public function current() {
    // Get the key/value pair from the iterator.
    $iterator_item = $this->getIteratorItem($this->iteratorPosition);
    // Get the key from the iterator item;
    return isset($iterator_item) ? $iterator_item['value'] : NULL;
  }

  /**
   * Move the iterator to the next position.
   */
  public function next() {
    // Increment the iterator position.
    $this->iteratorPosition++;
  }

  /**
   * Rewind the iterator to the beginning of the collection.
   */
  public function rewind() {
    // Reset the iterator position.
    $this->iteratorPosition = 0;
    // Check if the iterator needs to rebuild.
    if ($this->isIteratorRebuildNeeded()) {
      // Rebuild the iterator.
      $this->iteratorRebuild();
    }
  }

  /**
   * Get a value indicating whether the current iterator position is valid.
   *
   * @return bool
   *   TRUE if the current iterator position is valid, otherwise FALSE.
   */
  public function valid() {
    // Check if the iterator position exists in the header keys array.
    return array_key_exists($this->iteratorPosition, $this->iterator);
  }

  /*
   * Countable interface implementation.
   */

  /**
   * Get the number of headers in the collection.
   *
   * @return int
   *   The number of headers contained within this collection.
   */
  public function count() {
    // Return the header count.
    return count($this->iterator);
  }

  /*
   * Iterator helper functions.
   */

  /**
   * Get the iterator item located at the given position.
   *
   * @param int $position
   *   The position to retrieve the iterator item from.
   *
   * @return array
   *   An associative array containing key/value pair.
   */
  protected final function getIteratorItem($position) {
    // Get the iterator item and return the result.
    return isset($this->iterator[$position]) ? $this->iterator[$position] : NULL;
  }

  /**
   * Set the iterator rebuild flag.
   *
   * This will cause the iterator to be rebuild during the next rewind().
   */
  protected final function setIteratorRebuild() {
    $this->iteratorRebuild = TRUE;
  }

  /**
   * Get a value indicating whether the iterator needs to be rebuild.
   *
   * @return bool
   *   TRUE if the iterator needs to be rebuild, otherwise FALSE.
   */
  protected function isIteratorRebuildNeeded() {
    return $this->iteratorRebuild;
  }

  /**
   * Rebuild the iterator.
   */
  protected function iteratorRebuild() {
    // Remove the iterator rebuild flag.
    $this->iteratorRebuild = FALSE;
    // The iterator variable needs to be rebuild during every rewind.
    // Get the array keys from the headers variable.
    $header_keys = array_keys($this->headers);
    // Iterate through the header keys.
    foreach ($header_keys as $delta => $key) {
      // Add the key/value pair to the iterator array.
      $this->iterator[] = array(
        'key' => $key,
        // Store a reference to the header value. This will ensure that changes
        // that the iterator will have up to date values and does not need a
        // rebuild if a value changes.
        'value' => &$this->headers[$key],
      );
    }
  }

}

/**
 * Defines the base class for handling web requests.
 */
class WebRequest implements WebHeaderInterface {

  /*
   * Class members
   */

  private $url = NULL;
  private $requestHeaders = NULL;
  private $curlOptions = array();
  private $tags = array();
  private $metaData = array();

  /*
   * WebRequest implementation.
   */

  /**
   * Create a new WebRequest object.
   *
   * @param string $url
   *   A string containing a fully qualified URI.
   *
   * @throws InvalidArgumentException
   *   If the URL argument is not a fully qualified URI.
   */
  public function __construct($url) {
    // Set the request URL.
    $this->setURL($url);
    // Initialize the request headers.
    $this->requestHeaders = new WebHeaderCollection(FALSE);
  }

  /**
   * Get the URL to which the request will be send.
   *
   * @return string
   *   A fully qualified URI.
   */
  public function getURL() {
    // Returns the fully qualified URI.
    return $this->url;
  }

  /**
   * Set the URL to which the request will be send.
   *
   * @param string $url
   *   A string containing a fully qualified URI.
   *
   * @throws InvalidArgumentException
   *   If the URL argument is not a fully qualified URI.
   */
  public function setURL($url) {
    // Parse the URL and make sure we can handle the schema.
    $uri = @parse_url($url);
    // Check if the URL argument is invalid.
    if ($uri == FALSE || !isset($uri['scheme'])) {
      // Raise an invalid argument exception due to invalid URL.
      throw new InvalidArgumentException('The URL argument must contain a fully qualified URI.');
    }
    else {
      // Set the request URL.
      $this->url = $url;
    }
  }

  /**
   * Get the request headers.
   *
   * @return WebHeaderCollection
   *   An WebHeaderCollection object containing the request headers.
   */
  public function getHeaders() {
    // Return the request headers.
    return $this->requestHeaders;
  }

  /**
   * Set the request option with the given value.
   *
   * @param int $option
   *   The option identifier.
   * @param mixed $value
   *   The option value.
   */
  public function setOption($option, $value) {
    // Check if the value is set.
    if (isset($value)) {
      // Override the current option with the new value.
      $this->curlOptions[$option] = $value;
    }
    else {
      // Remove the current option.
      unset($this->curlOptions[$option]);
    }
  }

  /**
   * Set the request options.
   *
   * The options array is constructed using key/value pairs.
   *
   * @param array $options
   *   A request options array.
   */
  public function setOptions(array $options) {
    // Overwrite the current curl options.
    $this->curlOptions = $options;
  }

  /**
   * Get the value for the given option.
   *
   * @param int $option
   *   The option identifier.
   *
   * @return mixed
   *   A mixed value is returned if the identifier is present, otherwise NULL.
   */
  public function getOption($option) {
    // Return the option value for the given option.
    return isset($this->curlOptions[$option]) ? $this->curlOptions[$option] : NULL;
  }

  /**
   * Get the options.
   *
   * @return array
   *   An associative array containing the option values, keyed by the option
   *   identifier.
   */
  public function getOptions() {
    // Return the curl options.
    return $this->curlOptions;
  }

  /**
   * Get the server response.
   *
   * @return WebResponse
   *   A WebResponse object containing the response data.
   *
   * @throws RuntimeException
   *   If the response object could not initialize the cURL handle.
   */
  public function getResponse() {
    // Return the response using the current request.
    return new WebResponse($this);
  }

  /**
   * Get the raw response data.
   *
   * @return string
   *   A string containing the raw response data if successful, otherwise NULL.
   */
  public function getRawResponse() {
    // Get the response for the current request.
    $response = $this->getResponse();
    // Return the raw data.
    return $response->getRawData();
  }

  /**
   * Get the formatted response data.
   *
   * @return mixed
   *   Mixed value containing the formatted response data if successful,
   *   otherwise NULL.
   */
  public function getFormattedResponse() {
    // Get the response for the current request.
    $response = $this->getResponse();
    // Return the formatted data.
    return $response->getFormattedData();
  }

  /**
   * Adds a tag.
   *
   * @param string $tag
   *   The tag to add.
   *
   * @return WebRequest
   *   The called object.
   */
  public function addTag($tag) {
    // Check if the tag is set.
    if (isset($tag)) {
      // Add the tag.
      $this->tags[$tag] = $tag;
    }
    return $this;
  }

  /**
   * Indicates whether the tag is present.
   *
   * @param string $tag
   *   The tag to check.
   *
   * @return bool
   *   TRUE if this object has been marked with this tag, FALSE otherwise.
   */
  public function hasTag($tag) {
    // Determine whether the tag is present.
    return isset($this->tags[$tag]);
  }

  /**
   * Indicates whether all the given tags are present.
   *
   * @param array $tags
   *   A variable number of arguments, one for each tag to check.
   *
   * @return bool
   *   TRUE if this object has been marked with all specified tags, otherwise
   *   FALSE.
   */
  public function hasAllTags(array $tags) {
    // Determine whether all the tags are present.
    return !(boolean) array_diff($tags, $this->tags);
  }

  /**
   * Indicates whether any of the given tags are present.
   *
   * @param array $tags
   *   A variable number of arguments, one for each tag to check.
   *
   * @return bool
   *   TRUE if this object has been marked with at least one of the specified
   *   tags, FALSE otherwise.
   */
  public function hasAnyTag(array $tags) {
    // Determine whether at least one tag is present.
    return (boolean) array_intersect($tags, $this->tags);
  }

  /**
   * Get the tags.
   *
   * @return array
   *   An array containing the tags.
   */
  public function getTags() {
    // Return the tags values.
    return array_values($this->tags);
  }

  /**
   * Add additional meta data.
   *
   * @param string $key
   *   The unique identifier for this piece of metadata. Must be a string that
   *   follows the same rules as any other PHP identifier.
   * @param mixed $object
   *   The additional data to add. May be any valid PHP variable.
   *
   * @return WebRequest
   *   The called object.
   */
  public function addMetaData($key, $object) {
    // Check if the object is set.
    if (isset($object)) {
      // Overwrite the meta data with the new value.
      $this->metaData[$key] = $object;
    }
    else {
      // Remove the meta data.
      unset($this->metaData[$key]);
    }
    return $this;
  }

  /**
   * Retrieves a given piece of metadata.
   *
   * @param string $key
   *   The unique identifier for the piece of metadata to retrieve.
   *
   * @return mixed
   *   The previously attached metadata object, or NULL if one doesn't exist.
   */
  public function getMetaData($key) {
    // Return the meta data linked to the given key.
    return isset($this->metaData[$key]) ? $this->metaData[$key] : NULL;
  }

}

/**
 * Defines the base class for handling web responses.
 */
class WebResponse implements WebHeaderInterface {

  /*
   * Class members.
   */

  private $responseProtocol = NULL;
  private $responseErrorMessage = '';
  private $responseError = 0;
  private $responseHeaders = NULL;
  private $responseInfo = array();
  private $responseBody = NULL;
  private $request = NULL;

  /*
   * WebResponse implementation.
   */

  /**
   * Create a new WebResponse object.
   *
   * @param WebRequest $request
   *   A web request object.
   *
   * @throws RuntimeException
   *   If a cURL handle was not initialized.
   */
  public function __construct(WebRequest $request) {
    // Initialize the response headers.
    $this->responseHeaders = new WebHeaderCollection(TRUE);
    // Allow other modules to alter the request and set the final request.
    $this->alterAndSetRequest($request);
    // Perform the request and process the response data.
    $this->execute();
  }

  /**
   * Allow other modules to alter the request before using it.
   *
   * @param WebRequest $request
   *   The request object.
   */
  protected function alterAndSetRequest(WebRequest $request) {
    // Allow other modules to alter the request before executing it.
    // Construct the default alter hooks array.
    $hooks = array('webclient_request');
    // Iterate through the alter tags.
    foreach ($request->getTags() as $tag) {
      // Construct and add the tag specific alter hook.
      $hooks[] = "webclient_request_{$tag}";
    }
    // Allow other module to alter the request.
    drupal_alter($hooks, $request);
    // Create a copy of the original request.
    $this->request = clone($request);
  }

  /**
   * Perform the request and process the response data.
   *
   * @throws RuntimeException
   *   If a cURL handle was not initialized.
   */
  protected function execute() {
    // Try to initialize a curl handle.
    if (($handle = curl_init()) !== FALSE) {
      // Get the request object.
      $request = $this->getRequest();
      // Initialize the headers as an empty array.
      $headers = array();
      // Iterate through the headers.
      foreach ($request->getHeaders() as $header => $value) {
        // Add the header.
        $headers[] = "{$header}: {$value}";
      }
      // Construct the curl options.
      $options = array(
        CURLOPT_URL => $request->getURL(),
        CURLOPT_HTTPHEADER => $headers,
        CURLOPT_HEADERFUNCTION => array(&$this, 'parseResponseHeader'),
        CURLOPT_FAILONERROR => FALSE,
        CURLOPT_RETURNTRANSFER => TRUE,
      ) + $request->getOptions();
      // Apply the options to the current curl handle.
      curl_setopt_array($handle, $options);
      // Perform the request and wait for the response.
      $this->responseBody = curl_exec($handle);
      // Determine whether an error occurred.
      if (($this->responseError = curl_errno($handle)) != 0) {
        // Get the error message corresponding to the current error.
        $this->responseErrorMessage = curl_error($handle);
        // Only log non-HTTP related errors. CURLE_HTTP_RETURNED_ERROR is named
        // differently in PHP (CURLE_HTTP_NOT_FOUND).
        if ($this->responseError != CURLE_HTTP_NOT_FOUND) {
          // Log error to watchdog.
          watchdog('webclient', 'An error occurred during the request "@url": @error_message', array('@url' => $this->getRequest()->getURL(), '@error_message' => $this->responseErrorMessage), WATCHDOG_ERROR);
        }
      }
      // Get the response information.
      $this->responseInfo = curl_getinfo($handle);
      // Close the curl handle and free resources.
      curl_close($handle);
    }
    else {
      // Throw a runtime exception because the curl handle cannot be
      // initialized.
      throw new RuntimeException('Failed to initialize a cURL handle, operation was aborted.');
    }
  }

  /**
   * Callback function for the cURL option CURLOPT_HEADERFUNCTION.
   *
   * This function should not be called directly.
   */
  protected function parseResponseHeader($curl_handle, $header) {
    // Check if the header starts with HTTP/ which is the protocol header.
    if (strpos($header, 'HTTP/') === 0) {
      // Split the protocol header on whitespaces.
      $protocol_parts = explode(' ', $header);
      // Get the protocol.
      $this->responseProtocol = array_shift($protocol_parts);
    }
    else {
      // Add the raw header data to the response headers.
      $this->responseHeaders->addRaw($header);
    }
    // Return the number of bytes used.
    return drupal_strlen($header);
  }

  /**
   * Get the protocol.
   *
   * @return string
   *   A string containing the protocol is response contains no errors,
   *   otherwise NULL.
   */
  public function getProtocol() {
    $protocol = NULL;
    // Check if the response contains no error. HTTP errors are allowed.
    if (!$this->hasError() || $this->hasHTTPError()) {
      // Get the response protocol.
      $protocol = $this->responseProtocol;
    }
    return $protocol;
  }

  /**
   * Get the response headers.
   *
   * @return WebHeaderCollection
   *   An WebHeaderCollection object containing the response headers.
   */
  public function getHeaders() {
    // Return a copy of the response headers.
    return clone($this->responseHeaders);
  }

  /**
   * Get the original request.
   *
   * @return WebRequest
   *   The original request object.
   */
  public function getRequest() {
    // Return a copy of the request so that the original object cannot be
    // modified.
    return clone($this->request);
  }

  /**
   * Get the response information.
   *
   * @param string $key
   *   The key for which the information is requested.
   *
   * @return mixed
   *   A mixed value containing the information if the key is valid, otherwise
   *   NULL.
   */
  public function getResponseInfo($key) {
    // Return the response information for the given key.
    return isset($this->responseInfo[$key]) ? $this->responseInfo[$key] : NULL;
  }

  /**
   * Get all the response information.
   *
   * @return array
   *   An associative array containing the response information.
   */
  public function getAllResponseInfo() {
    // Return the entire response information array.
    return $this->responseInfo;
  }

  /**
   * Get the raw response data.
   *
   * @return string
   *   A string containing the raw response data if the request was successful,
   *   otherwise NULL.
   */
  public function getRawData() {
    // Return the response body.
    return $this->responseBody;
  }

  /**
   * Get the converted response data using the response mime type.
   *
   * @return mixed
   *   Mixed value depending on the MIME type if the response contains no error,
   *   otherwise NULL.
   */
  public function getFormattedData() {
    // Check if the library performed a request.
    if (!$this->hasLibError()) {
      // Get the MIME type from the response header.
      list($mime_type) = explode(';', $this->getResponseInfo('content_type')) + array(NULL);
      // Trim leading and trailing whitespaces.
      $mime_type = trim($mime_type);
      // Check if the MIME type is present.
      if (!empty($mime_type)) {
        // Format the response data using the given MIME type.
        return webclient_format_response($mime_type, $this->getRawData());
      }
    }
    return NULL;
  }

  /**
   * Get a value indicating whether the response describes an error.
   *
   * @return bool
   *   TRUE if the response contains an error, otherwise FALSE.
   */
  public function hasError() {
    // Return the has error flag.
    return $this->hasLibError() || $this->hasHTTPError();
  }

  /**
   * Get a value indicating whether the library encountered an error.
   *
   * @return bool
   *   TRUE if the library encountered an error, otherwise FALSE.
   */
  public function hasLibError() {
    return $this->responseError != 0;
  }

  /**
   * Get a value indicating whether the response describes an HTTP error.
   *
   * @return bool
   *   TRUE if the response contains an HTTP error, otherwise FALSE.
   */
  public function hasHTTPError() {
    // Check if the response has no error.
    if ($this->responseError == 0) {
      // Get the HTTP status code. Do not use the getStatusCode() function
      // because that function depends on this function.
      $code = $this->getResponseInfo('http_code');
      // Determine whether the HTTP status code describes an error.
      return $code >= 400;
    }
    return FALSE;
  }

  /**
   * Get the response error code.
   *
   * @return mixed
   *   A numeric value indicating the response error code if the response has an
   *   error, otherwise FALSE.
   */
  public function getErrorCode() {
    $error_code = FALSE;
    // Check if the response contains an error.
    if ($this->hasError()) {
      // Check if the response contains an HTTP error.
      if ($this->hasHTTPError()) {
        // Get the HTTP status code.
        $error_code = $this->getStatusCode();
      }
      else {
        // Get the response error code.
        $error_code = $this->responseError;
      }
    }
    return $error_code;
  }

  /**
   * Get the response error message.
   *
   * @return mixed
   *   A string containing the response error message if the response has an
   *   error, otherwise FALSE.
   */
  public function getErrorMessage() {
    $error_message = FALSE;
    // Check if the response contains an error.
    if ($this->hasError()) {
      // Check if the response contains an HTTP error.
      if ($this->hasHTTPError()) {
        // Get the HTTP status code.
        $code = $this->getStatusCode();
        // Get the status message.
        $error_message = WebResponse::getStatusMessageByCode($code);
      }
      else {
        // Use the response error message.
        $error_message = $this->responseErrorMessage;
      }
    }
    return $error_message;
  }

  /**
   * Get the status code.
   *
   * @return mixed
   *   The status code if the response contains no errors, otherwise FALSE.
   */
  public function getStatusCode() {
    $code = FALSE;
    // Check if the response contains no error. HTTP errors are allowed.
    if (!$this->hasError() || $this->hasHTTPError()) {
      // Get the HTTP status code.
      $code = $this->getResponseInfo('http_code');
    }
    return $code;
  }

  /**
   * Get the status message.
   *
   * @return mixed
   *   A string containing the status message if the response has a valid HTTP
   *   code, otherwise FALSE.
   */
  public function getStatusMessage() {
    $status_message = FALSE;
    // Check if the response contains no error. HTTP errors are allowed.
    if (!$this->hasError() || $this->hasHTTPError()) {
      // Get the HTTP status code.
      $code = $this->getStatusCode();
      // Get the HTTP status message.
      $status_message = WebResponse::getStatusMessageByCode($code);
    }
    return $status_message;
  }

  /**
   * Get the status message for the specified HTTP code.
   *
   * @param int $code
   *   An HTTP code.
   *
   * @return mixed
   *   A string containing the status message if the HTTP code is valid,
   *   otherwise FALSE.
   */
  public static function getStatusMessageByCode($code) {
    // Initialize the response messages.
    static $response_messages = array(
      100 => 'Continue',
      101 => 'Switching Protocols',
      200 => 'OK',
      201 => 'Created',
      202 => 'Accepted',
      203 => 'Non-Authoritative Information',
      204 => 'No Content',
      205 => 'Reset Content',
      206 => 'Partial Content',
      300 => 'Multiple Choices',
      301 => 'Moved Permanently',
      302 => 'Found',
      303 => 'See Other',
      304 => 'Not Modified',
      305 => 'Use Proxy',
      307 => 'Temporary Redirect',
      400 => 'Bad Request',
      401 => 'Unauthorized',
      402 => 'Payment Required',
      403 => 'Forbidden',
      404 => 'Not Found',
      405 => 'Method Not Allowed',
      406 => 'Not Acceptable',
      407 => 'Proxy Authentication Required',
      408 => 'Request Time-out',
      409 => 'Conflict',
      410 => 'Gone',
      411 => 'Length Required',
      412 => 'Precondition Failed',
      413 => 'Request Entity Too Large',
      414 => 'Request-URI Too Large',
      415 => 'Unsupported Media Type',
      416 => 'Requested range not satisfiable',
      417 => 'Expectation Failed',
      500 => 'Internal Server Error',
      501 => 'Not Implemented',
      502 => 'Bad Gateway',
      503 => 'Service Unavailable',
      504 => 'Gateway Time-out',
      505 => 'HTTP Version not supported',
    );
    // Return the response message for the given code.
    return isset($response_messages[$code]) ? $response_messages[$code] : FALSE;
  }

}
