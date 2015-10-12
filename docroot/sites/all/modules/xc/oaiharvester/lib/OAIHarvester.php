<?php
/**
 * @file
 *
 * The original version of this class was the Omeka's OAI-PMH plugin made by
 * Center for History and New Media. The current version contains some fragments
 * from the first and second versions of that library, but it is rewritten in
 * almost all parts.
 */

class OAIHarvester {

  const NAMESPACE_URI = 'http://www.openarchives.org/OAI/2.0/';
  const CONTENT_SOURCE_CACHE = 1;
  const CONTENT_SOURCE_CURL = 2;

  private $parser;
  private $baseUrl;
  private $requestVerb;
  private $requestArguments;
  private $http_cache;
  private $httpCode;
  private $error;
  private $content;
  private $hasFetched = FALSE;
  private $statistics = array();
  private $downloadSize = -1;
  private $fileSize = -1;
  private $fetchError = array();
  private $requestUrl = NULL;
  private $curlInfo = array();
  private $requestHeader;
  private $httpHeader;
  private $contentSource;

  public function __construct($requestVerb = 'ListRecords', $baseUrl = '', $requestArguments = array(), $http_cache = FALSE) {
    $this->requestVerb = $requestVerb;
    $this->baseUrl = $baseUrl;
    $this->requestArguments = $requestArguments;
    $this->http_cache = $http_cache;
  }

  public function fetchContent($type = 'url', $filename = '') {
    $t0 = microtime(TRUE);

    $this->content = '';

    if ($type == 'url') {
      $this->fetchHttpContent();
    }
    elseif ($type == 'file') {
      $this->fetchFileContent($this->baseUrl . '/' . $filename);
    }

    $this->statistics['fetch']['all'] = microtime(TRUE) - $t0;
  }

  public function fetchFileContent($filename) {
    try {
      $this->requestUrl = $filename;
      $this->content = file_get_contents($filename);
      $this->httpCode = 200;
    }
    catch (Exception $e) {
      xc_log_error('harvester', var_export($e, TRUE));
      $this->error = $e->getMessage();
      $this->httpCode = 0;
    }
    $this->hasFetched = TRUE;
  }

  public function fetchHttpContent() {
    $t0 = microtime(TRUE);
    $this->requestUrl = $this->baseUrl . '?verb=' . $this->requestVerb;
    if (count($this->requestArguments)) {
      foreach ($this->requestArguments as $key => $value) {
        $this->requestUrl .= '&' . $key . '=' . $value;
      }
    }

    $cache_file = $this->getCacheFile();
    if (isset($cache_file) && file_exists($cache_file)) {
      try {
        $this->content = file_get_contents($cache_file);
      }
      catch (Exception $e) {
        xc_log_error('harvester', 'file get contents error: ' . $e->getMessage());
        $this->error = $e->getMessage();
      }
      $this->statistics['fetch']['curl_init'] = microtime(TRUE) - $t0;
      $this->httpCode = 200;
      $this->contentSource = self::CONTENT_SOURCE_CACHE;
    }
    else {
      $ch = curl_init("");
      curl_setopt($ch, CURLINFO_HEADER_OUT, 1); // Get request header
      curl_setopt($ch, CURLOPT_HEADER, 1); // Get response header
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($ch, CURLOPT_NOPROGRESS, 1);
      curl_setopt($ch, CURLOPT_USERAGENT, 'Omeka-XC OAI harvester');
      curl_setopt($ch, CURLOPT_ENCODING, "gzip,deflate");
      curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10);
      if (substr($this->requestUrl, 0, 5) == 'https') {
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
      }
      curl_setopt($ch, CURLOPT_URL, $this->requestUrl);
      $this->statistics['fetch']['curl_init'] = microtime(TRUE) - $t0;

      // get the content
      $response = curl_exec($ch);
      // extract HTTP response header
      $this->httpHeader = substr($response, 0, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
      // extract content
      $this->content = substr($response, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
      $this->requestHeader = curl_getinfo($ch, CURLINFO_HEADER_OUT);
      $this->httpCode = (int) curl_getinfo($ch, CURLINFO_HTTP_CODE);
      $this->downloadSize = curl_getinfo($ch, CURLINFO_SIZE_DOWNLOAD);
      $this->curlInfo = curl_getinfo($ch);
      if (curl_errno($ch)) {
        $this->fetchError = array(
          'code' => curl_errno($ch), // http://curl.haxx.se/libcurl/c/libcurl-errors.html
          'text' => curl_error($ch),
        );
      }
      $this->contentSource = self::CONTENT_SOURCE_CURL;
      curl_close($ch);
    }
    $this->hasFetched = TRUE;
  }

  public function processContent($parsingMode = 'dom', $fetchMode = 'url', $fileName = NULL) {
    if (!$this->hasFetched) {
      $this->fetchContent($fetchMode, $fileName);
    }
    // IOAIParser is the parser interface
    require_once('IOAIParser.php');
    switch ($parsingMode) {
      // regex-based parser
      case 'regex':
        require_once('OAIRegexParser.php');
        $this->parser = new OAIRegexParser($this->content, $this->httpCode);
        break;
        // DOM-based parser
      case 'dom':
      default:
        require_once('OAIDomParser.php');
        $this->parser = new OAIDomParser($this->content, $this->httpCode);
    }
  }

  public function cacheContent() {
    if ($this->http_cache !== FALSE
        && $this->contentSource == self::CONTENT_SOURCE_CURL) {

      $cache_file = $this->getCacheFile();
      if (isset($cache_file) && $this->httpCode == 200) {
        if ($this->getSize() == 0) {
          $this->error = t('The data provider returned an empty string instead of a valid XML.');
        }
        else if (!$this->hasRecords()) {
          $this->error = t('The data provider returned no records.');
        }
        else {
          file_put_contents($cache_file, $this->content);

          // put content into the list
          $file_list = $this->http_cache . '/list.txt';
          $file_info = sprintf("%s\t%s\t%s\n", format_date(time(), 'custom', 'Y-m-d H:i:s'), md5($this->requestUrl), $this->requestUrl);
          file_put_contents($file_list, $file_info, FILE_APPEND);
          unset($list);
        }
      }
    }
  }

  /**
   * Validate the request verb.
   *
   * @param string $requestVerb
   *   The verb to request
   *
   * @return (boolean)
   */
  protected static function validateRequestVerb($requestVerb) {
    switch ($requestVerb) {
      case 'Identify':
      case 'ListMetadataFormats':
      case 'ListSets':
      case 'GetRecord':
      case 'ListIdentifiers':
      case 'ListRecords':
        return true;
        break;
    }
    throw new Exception('OAI-PMH parser error: Invalid request verb: ' . $requestVerb);
  }

  /**
   * Validate a URL.
   * @param $url
   * @return unknown_type
   */
  protected static function validateUrl($url) {
    if (!preg_match('/^http/', $url) || !@fopen($url, 'rb')) {
      throw new Exception(
        'OAI-PMH parser error: Could not validate or open provided URL.');
    }
    return true;
  }

  /**
   * Build the schema array containing all the names and namespace URIs of
   * the extended schema in the lib/Schema directory.
   */
  protected function _setSchemas() {
    foreach (new DirectoryIterator(OAIPMH_SCHEMA_DIRECTORY_PATH) as $item) {
      if ($item->isFile()) {
        include_once DRUPAL_ROOT . '/' . OAIPMH_SCHEMA_DIRECTORY_PATH . DIRECTORY_SEPARATOR . $item;
        $schemaName   = drupal_substr($item, 0, strpos($item, '.'));
        $className    = 'Schema_' . $schemaName;
        $namespaceUri = call_user_func(array($className, 'getNamespaceUri'));
        $this->_schemas[] = array(
          'schemaName' => $schemaName,
          'namespaceUri' => $namespaceUri,
        );
      }
    }
  }

  public function getSets() {
    $continue = FALSE;
    if ($this->requestVerb != 'ListSets') {
      return FALSE;
    }
    if (!$this->hasFetched) {
      $this->fetchContent();
    }
    // print $this->content;
    $items = array();
    do {
      $oaipmh = new SimpleXMLIterator($this->content);
      if (isset($oaipmh->ListSets)) {
        $sets = $oaipmh->ListSets->set;
        foreach ($sets as $set) {
          $item = (object) array(
            'set_spec' => (string) $set->setSpec,
            'display_name' => (string) $set->setName,
            'description' => '',
          );
          if (isset($set->setDescription)) {
            $descriptions = array();
            $dom = dom_import_simplexml($set->setDescription);
            foreach ($dom->childNodes as $childNode) {
              if ($childNode->nodeType == XML_ELEMENT_NODE) {
                $sescriptions[] = $childNode->nodeValue;
              }
            }
            $item->description = join(' ', $sescriptions);
          }
          $items[] = $item;
        }
        $continue = FALSE;
        if ($oaipmh->ListSets->resumptionToken) {
          $token = $oaipmh->ListSets->resumptionToken;
          if ((string) $token != '') {
            $this->requestArguments = array('resumptionToken' => (string) $oaipmh->ListSets->resumptionToken);
            print_r($this->requestArguments);
            $this->fetchContent();
            if (!$this->isErrorResponse()) {
              $continue = TRUE;
            }
          }
        }
      }
    } while ($continue);

    return $items;
  }

  public function getIdentify() {
    if ($this->requestVerb != 'Identify') {
      return FALSE;
    }
    if (!$this->hasFetched) {
      $this->fetchContent();
    }
    $oaipmh = new SimpleXMLIterator($this->content);
    $identify = array();
    $identify['repositoryName'] = (string) $oaipmh->Identify->repositoryName;
    $identify['baseUrl'] = (string) $oaipmh->Identify->baseURL;
    $identify['protocolVersion'] = (string) $oaipmh->Identify->protocolVersion;
    foreach ($oaipmh->Identify->adminEmail as $email) {
      $identify['adminEmail'][] = (string) $email;
    }

    $identify['earliestDatestamp'] = (string) $oaipmh->Identify->earliestDatestamp;
    $identify['deletedRecord'] = (string) $oaipmh->Identify->deletedRecord;
    $identify['granularity'] = (string) $oaipmh->Identify->granularity;
    $identify['compressions'] = array();
    foreach ($oaipmh->Identify->compression as $compression) {
      $identify['compressions'][] = (string) $compression;
    }

    foreach ($oaipmh->Identify->description as $description) {
      $dom = dom_import_simplexml($description);
      foreach ($dom->childNodes as $child) {
        if ($child->nodeType == XML_ELEMENT_NODE) {
          $identify['description'][] = array(
            'namespaceURI' => $child->namespaceURI,
            'childNode' => $child,
          );
        }
      }
    }

    return $identify;
  }

  public function getMetadataFormats() {
    if ($this->requestVerb != 'ListMetadataFormats') {
      return FALSE;
    }

    if (!$this->hasFetched) {
      $this->fetchContent();
    }

    $formats = array();
    $oaipmh = new SimpleXMLIterator($this->content);
    if (isset($oaipmh->ListMetadataFormats)) {
      $metadataFormats = $oaipmh->ListMetadataFormats->metadataFormat;
      foreach ($metadataFormats as $metadataFormat) {
        $formats[] = (object) array(
          'name' => (string) $metadataFormat->metadataPrefix,
          'schema_location' => (string) $metadataFormat->schema,
          'namespace' => (string) $metadataFormat->metadataNamespace,
        );
      }
    }

    // todo: resumptionToken
    return $formats;
  }

  public function isErrorResponse() {
    if (!$domDocument = @DOMDocument::loadXML($this->content)) {
      throw new Exception($this->content);
    }

    if ($domDocument->getElementsByTagNameNS(self::NAMESPACE_URI, 'error')->item(0) === NULL) {
      return FALSE;
    }
    else {
      $error = $domDocument->getElementsByTagNameNS(self::NAMESPACE_URI, 'error');
      $this->error = array(
        'code' => $error->item(0)->getAttribute('code'),
        'text' => $error->item(0)->nodeValue,
      );
      return TRUE;
    }
  }

  public function getBaseUrl() {
    return $this->baseUrl;
  }

  public function getContent() {
    return $this->content;
  }

  public function getHttpCode() {
    return $this->httpCode;
  }

  public function getCacheFile() {
    return $this->http_cache !== FALSE && !empty($this->http_cache)
      ? $this->http_cache . '/' . md5($this->requestUrl)
      : NULL;
  }

  function getStatistics($type = 'all') {
    switch ($type) {
      case 'all':
        return $this->statistics;
        break;
      case 'parser':
        return $this->parser->getStatistics();
        break;
      case 'fetch':
        return $this->statistics['fetch']['all'];
      case 'parse_response':
        return $this->parser->getStatistics('response');
        break;
      case 'parse_records':
        return $this->parser->getStatistics('records');
        break;
      default:
        return;
    }
  }

  /* WRAPPER FUNCTIONS FOR PARSER INTERFACE FUNCTIONS */

  public function hasErrors() {
    return $this->parser->hasErrors();
  }

  /** Get all errors */
  public function listErrors() {
    return $this->parser->listErrors();
  }

  /** Has records? */
  public function hasRecords() {
    return $this->parser->hasRecords();
  }

  /** Gets number of records */
  public function getRecordCount() {
    return $this->parser->getRecordCount();
  }

  /** Gets next record */
  public function getNextRecord() {
    return $this->parser->getNextRecord();
  }

  /** Has resumptionToken? */
  public function hasResumptionToken() {
    return $this->parser->hasResumptionToken();
  }

  /** Gets the resumptionToken */
  public function getResumptionToken() {
    return $this->parser->getResumptionToken();
  }

  /** Gets the size of downloaded XML file */
  public function getSize() {
    return strlen(trim($this->content));
  }

  /** Gets the errors generated during fetch content */
  public function getFetchError() {
    return $this->fetchError;
  }

  /** Gets the request URL */
  public function getRequestUrl() {
    return $this->requestUrl;
  }

  /**
   * Gets cURL information
   *
   * @see http://hu.php.net/curl_getinfo
   *
   * @return (array)
   *   An array of cURL information.
   */
  public function getCurlInfo() {
    return $this->curlInfo;
  }

  /**
   * Gets the HTTP request header
   *
   * @return (string)
   *   The full request header as one string.
   */
  public function getRequestHeader() {
    return $this->requestHeader;
  }

  /**
   * Gets the HTTP response header
   *
   * @return (string)
   *   The full response header as one string.
   */
  public function getHttpHeader() {
    return $this->httpHeader;
  }
}
