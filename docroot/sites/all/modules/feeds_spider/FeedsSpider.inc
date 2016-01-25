<?php

/**
 * @file
 * Home of FeedsSpider.
 */

/**
 * Fetches data via HTTP.
 */
class FeedsSpider extends FeedsHTTPFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    if ($this->config['use_pubsubhubbub'] && ($raw = $this->subscriber($source->feed_nid)->receive())) {
      return new FeedsFetcherResult($raw);
    }

    $state = $source->state(FEEDS_FETCH);

    feeds_include_library('http_request.inc', 'http_request');

    if (empty($state->inited)) {
      $state->inited = TRUE;
      $response = http_request_get($source_config['source']);
      if ($response->code != 200) {
        throw new Exception();
      }
      $state->urls = $this->parseURLs($response->data, $source_config);
      $state->total = count($state->urls);
    }

    if (empty($state->urls)) {
      new FeedsHTTPFetcherResult('');
    }

    $url = array_shift($state->urls);

    $state->progress($state->total, $state->total - count($state->urls));

    return new FeedsHTTPFetcherResult($url);
  }

  protected function parseURLs($html, $source_config) {
    $urls = array();
    $dom = new DOMDocument();
    $dom->loadHTML($html);
    $xml = simplexml_import_dom($dom);

    if (!$xml) {
      return $urls;
    }

    foreach ($xml->xpath($source_config['xpath']) as $url) {
      $urls[] = http_request_create_absolute_url((string) $url, $source_config['source']);
    }
    return $urls;
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return array(
      'xpath' => '',
      'timeout' => variable_get('http_request_timeout', 15),
    );
  }

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $form = parent::sourceForm($source_config);

    $source_config = $source_config + $this->sourceDefaults();

    $form['xpath'] = array(
      '#type' => 'textfield',
      '#title' => t('XPath'),
      '#default_value' => $source_config['xpath'],
    );

    $form['timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Timeout'),
      '#default_value' => $source_config['timeout'],
    );

    return $form;
  }
}
