<?php

/**
 * @file
 * Home of the FeedsHTTPFetcherAppendHeaders and related classes.
 */

/**
 * Result of FeedsHTTPFetcherAppendHeaders::fetch().
 */
class FeedsHTTPFetcherAppendHeadersResult extends FeedsFetcherResult {
  protected $url;
  protected $file_path;
  protected $timeout;
  protected $append_headers;

  /**
   * Constructor.
   */
  public function __construct($url = NULL, $append_headers = '') {
    $this->url = $url;
    $this->append_headers = $append_headers;
    parent::__construct('');
  }

  /**
   * Overrides FeedsFetcherResult::getRaw();
   */
  public function getRaw() {
    feeds_include_library('http_request.inc', 'http_request');
    module_load_include('inc', 'feeds_httpfetcher_append_headers', 'libraries/feeds_httpfetcher_append_headers_request');   
    $result = feeds_httpfetcher_append_headers_request_get($this->url, NULL, NULL, TRUE, $this->timeout, $this->append_headers);
    if (!in_array($result->code, array(200, 201, 202, 203, 204, 205, 206))) {
      throw new Exception(t('Download of @url failed with code !code.', array('@url' => $this->url, '!code' => $result->code)));
    }
    return $this->sanitizeRaw($result->data);
  }
  
  public function getTimeout() {
    return $this->timeout;
  }

  public function setTimeout($timeout) {
    $this->timeout = $timeout;
  }
}

/**
 * Fetches data via HTTP and appending custom headers.
 */
class FeedsHTTPFetcherAppendHeaders extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    $append_headers = $this->prepareAppendHeaders($this->config['append_headers']);
    $fetcher_result = new FeedsHTTPFetcherAppendHeadersResult($source_config['source'], $append_headers);
    // When request_timeout is empty, the global value is used.
    $fetcher_result->setTimeout($this->config['request_timeout']);
    return $fetcher_result;
  }

  /**
   * Clear caches.
   */
  public function clear(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    $url = $source_config['source'];
    feeds_include_library('http_request.inc', 'http_request');
    http_request_clear_cache($url);
  }

  /**
   * Implements FeedsFetcher::request().
   */
  public function request($feed_nid = 0) {
    feeds_dbg($_GET);
    @feeds_dbg(file_get_contents('php://input'));
    try {
      feeds_source($this->id, $feed_nid)->existing()->import();
    }
    catch (Exception $e) {
      // In case of an error, respond with a 503 Service (temporary) unavailable.
      header('HTTP/1.1 503 "Not Found"', NULL, 503);
      drupal_exit();
    }
    // Will generate the default 200 response.
    header('HTTP/1.1 200 "OK"', NULL, 200);
    drupal_exit();
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'append_headers' => '',
      'request_timeout' => NULL,
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['append_headers'] = array(
      '#type' => 'textarea',
      '#title' => t('Append headers'),
      '#description' => t('Enter request headers that should be appended in the HTTP request. One header per line in the format key|value.'),
      '#default_value' => $this->config['append_headers'],
    );
    // Per importer override of global http request timeout setting.
    $form['request_timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Request timeout'),
      '#description' => t('Timeout in seconds to wait for an HTTP get request to finish.</br>' .
                          '<b>Note:</b> this setting will override the global setting.</br>' .
                          'When left empty, the global value is used.'),
      '#default_value' => $this->config['request_timeout'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#maxlength' => 3,
      '#size'=> 30,
    );
    return $form;
  }

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['source'] = array(
      '#type' => 'textfield',
      '#title' => t('URL'),
      '#description' => t('Enter a feed URL.'),
      '#default_value' => isset($source_config['source']) ? $source_config['source'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    $values['source'] = trim($values['source']);

    if (!feeds_valid_url($values['source'], TRUE)) {
      $form_key = 'feeds][' . get_class($this) . '][source';
      form_set_error($form_key, t('The URL %source is invalid.', array('%source' => $values['source'])));
    }
  }
  
  
  public function prepareAppendHeaders($append_headers) {
    $headers = array();
    $append_headers = explode("\n", $append_headers);
    if (is_array($append_headers)) {
      foreach ($append_headers as $header) {
        $headers[] = str_replace('|', ': ', $header);
      }
      
    }
    return $headers;
  }  
}