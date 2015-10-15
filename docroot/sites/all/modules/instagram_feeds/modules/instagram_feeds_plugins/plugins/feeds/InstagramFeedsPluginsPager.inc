<?php

/**
 * @file
 * Home of the InstagramFeedsPluginsPager.
 */

/**
 * Result of FeedsHTTPFetcher::fetch().
 */
class InstagramFeedsPluginsPagerResult extends FeedsFetcherResult {
  protected $url;

  /**
   * Constructor.
   */
  public function __construct($url = NULL) {
    $this->url = $url;
    parent::__construct('');
  }

  /**
   * Overrides FeedsFetcherResult::getRaw().
   */
  public function getRaw() {
    feeds_include_library('http_request.inc', 'http_request');
    $result = http_request_get($this->url, NULL, NULL, TRUE);
    if (!in_array($result->code, array(200, 201, 202, 203, 204, 205, 206))) {
      throw new Exception(t('Download of @url failed with code !code.', array(
        '@url' => $this->url,
        '!code' => $result->code,
      )));
    }
    return $this->sanitizeRaw($result->data);
  }
}

/**
 * Fetches data via Instagram API with the ability to page all results.
 */
class InstagramFeedsPluginsPager extends FeedsHTTPFetcher {

  /**
   * Implements parent::fetch().
   */
  public function fetch(FeedsSource $source) {
    // Taken from parent::fetch().
    $source_config = $source->getConfigFor($this);
    if ($this->config['use_pubsubhubbub'] && ($raw = $this->subscriber($source->feed_nid)->receive())) {
      return new InstagramFeedsPluginsPagerResult($raw);
    }

    // Initiate the fetch state.
    $state = $source->state(FEEDS_FETCH);

    // Populate the state.
    if (empty($state->inited)) {
      $state->inited = TRUE;
      $state->total = $this->config['max_pages'];
      $state->total_img = $this->config['number_of_images'];
      $state->item_count = array();
      $state->count = 0;
      $state->next_url = $source_config['source'];
      $state->page_until = $this->pageUntilID($source->feed_nid);
    }

    // Increment the operation count.
    $state->count++;

    // If there is no maximum amount of pages, then continue to
    // increment the total count so the batch doesn't end until
    // we finish paging.
    if (!$this->config['max_pages'] && (!$this->config['number_of_images']
        || !isset($state->item_count[$source_config['source']])
        || isset($state->item_count[$source_config['source']])
            && $state->item_count[$source_config['source']] < $this->config['number_of_images'])) {
      $state->total = $state->total + 2;
    }
    elseif (isset($state->item_count[$source_config['source']])
        && $state->item_count[$source_config['source']] >= $this->config['number_of_images']) {
      $state->total = $state->count - 1;
    }

    // Set the progress.
    $state->progress($state->total, $state->count);

    // Fetch the next URL, if there is one.
    if (isset($state->next_url) && $state->next_url) {
      $result = new InstagramFeedsPluginsPagerResult($state->next_url);
    }

    // Unset the next URL so we can determine if there is a new one.
    $state->next_url = NULL;

    // Extract the feed result data.
    if (isset($result) && ($data = $result->getRaw())) {
      // Decode the JSON.
      if ($data = drupal_json_decode($data)) {
        // Determine the next URL, if there is one.
        if (isset($data['pagination']['next_url'])) {
          $state->next_url = $data['pagination']['next_url'];
        }

        // See if we've reached the item we need to page until.
        if (isset($state->page_until) && $state->page_until) {
          if (isset($data['data']) && is_array($data['data'])) {
            // Iterate all of the results.
            if (!$state->item_count || !isset($state->item_count[$source_config['source']])) {
              if (!is_array($state->item_count)) {
                $state->item_count = array();
              }
              $state->item_count[$source_config['source']] = 0;
            }
            foreach ($data['data'] as $item) {
              if (isset($item['id'])) {
                // Stop if we've reached the ID we're looking for.
                if ($item['id'] == $state->page_until || ($state->total_img
                    && $state->total_img < $state->item_count[$source_config['source']])) {
                  // Remove the next URL so the operation stops.
                  $state->next_url = NULL;
                  break;
                }
              }
            }
          }
        }
      }
    }

    // If there is not a next URL, then stop the operation.
    if (!$state->next_url) {
      $state->progress($state->total, $state->total);
    }

    // @todo: What's best to return if we have no $result.
    return (isset($result) && $result) ? $result : new InstagramFeedsPluginsPagerResult('');
  }

  /**
   * Determine the last result ID fetched for a given feed nid.
   *
   * This is used when paging an Instagram feed to determine when to
   * stop paging.
   *
   * @param string $feed_nid
   *   The feed nid.
   *
   * @return string
   *   The GUID of the last item fetched for the feed nid.
   */
  public function pageUntilID($feed_nid) {
    return db_select('feeds_item', 'f')
       ->fields('f', array('guid'))
       ->condition('f.feed_nid', $feed_nid)
       ->range(0, 1)
       ->orderBy('guid', 'DESC')
       ->execute()
       ->fetchField();
  }

  /**
   * Override of parent::configDefaults().
   */
  public function configDefaults() {
    return parent::configDefaults() + array(
      'max_pages' => 0,
      'number_of_images' => 20,
    );
  }

  /**
   * Override of parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['max_pages'] = array(
      '#type' => 'textfield',
      '#title' => t('Max pages'),
      '#default_value' => $this->config['max_pages'],
      '#description' => t('Provide the maximum amount of pages to crawl. If set to 0, no maximum will be enforced.'),
    );
    $form['number_of_images'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of images'),
      '#default_value' => $this->config['number_of_images'],
      '#description' => t('Provide the amount of images to crawl. If set to 0, no maximum will be enforced.'),
      '#states' => array(
        'visible' => array(
          ':input[name="max_pages"]' => array('value' => 0),
        ),
      ),
    );
    return $form;
  }

  /**
   * Override of parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    parent::configFormValidate($values);

    // Make sure the max pages is numeric.
    if (!is_numeric($values['max_pages']) || ($values['max_pages'] < 0)) {
      form_set_error('max_pages', t('The maximum pages must be a number of zero or greater.'));
    }
    if (!is_numeric($values['number_of_images']) || ($values['number_of_images'] < 0)) {
      form_set_error('number_of_images', t('The amount of images must be a number of zero or greater.'));
    }
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    if (!feeds_valid_url($values['source'], TRUE)) {
      $form_key = 'feeds][' . get_class($this) . '][source';
      form_set_error($form_key, t('The URL %source is invalid.', array('%source' => $values['source'])));
    }
    elseif ($this->config['auto_detect_feeds']) {
      feeds_include_library('http_request.inc', 'http_request');
      if ($url = http_request_get_common_syndication($values['source'], array('accept_invalid_cert' => TRUE))) {
        $values['source'] = $url;
      }
    }
  }
}


/**
 * Enclosure element, can be part of the result array.
 */
class InstagramFeedsPluginsEnclosure extends FeedsEnclosure {
  protected $mimetype;

  /**
   * Constructor, requires MIME type.
   *
   * @param string $value
   *   A path to a local file or a URL to a remote document.
   * @param string $mimetype
   *   The mime type of the resource.
   */
  public function __construct($value, $mimetype) {
    parent::__construct($value, $mimetype);
    $this->mimetype = $mimetype;
  }

  /**
   * Get the content of the referenced resource.
   *
   * @return array()
   *   The content of the referenced resource.
   */
  public function getContent() {
    feeds_include_library('http_request.inc', 'http_request');
    $result = http_request_get($this->getUrlEncodedValue(), NULL, NULL, TRUE);
    if ($result->code != 200) {
      throw new Exception(t('Download of @url failed with code !code.', array(
        '@url' => $this->getUrlEncodedValue(),
        '!code' => $result->code,
      )));
    }
    return $result->data;
  }
}
