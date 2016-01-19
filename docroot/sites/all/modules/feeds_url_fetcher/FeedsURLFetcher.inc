<?php

/**
 * @file
 * This is a Feeds plugin that will fetch file from an URL.
 */

/**
 * Result of FeedsURLFetcher::fetch().
 */
class FeedsURLFetcherResult extends FeedsFetcherResult {
  protected $url;

  /**
   * Constructor.
   */
  public function __construct($url = NULL) {
    $this->url = $url;
    parent::__construct('');
  }

  /**
   * Overrides FeedsFetcherResult::getRaw();
   */
  public function getRaw() {
    $data = file_get_contents($this->url);

    if (!is_string($data)) {
      throw new Exception(t('Failed to retrieve URL.'));
      watchdog('feeds', 'Failed to retrieve URL !url.', array('!url' => $this->url), WATCHDOG_ERROR);
    }

    return $this->sanitizeRaw($data);
  }
}

/**
 * Fetches data via URL.
 */
class FeedsURLFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);

    return new FeedsURLFetcherResult($source_config['source']);
  }

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['source'] = array(
      '#type' => 'textfield',
      '#title' => t('URL'),
      '#description' => t('Enter a feed URL. You can use any of supported protocols by PHP.'),
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
    if (!feeds_valid_url($values['source'], TRUE)) {
      $form_key = 'feeds][' . get_class($this) . '][source';
      form_set_error($form_key, t('The URL %source is invalid.', array('%source' => $values['source'])));
    }
  }
}