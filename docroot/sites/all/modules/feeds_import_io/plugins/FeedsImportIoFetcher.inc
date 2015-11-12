<?php
/**
 * @file
 * Custom FeedsFetcher.
 */

class FeedsImportIoFetcher extends FeedsFetcher {
  /**
   * Overrides parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();

    $form['connector_guid'] = array(
      '#type' => 'textfield',
      '#title' => t('Data Set ID'),
      '#description' => t('Created as a result of a Bulk or Chain Extract, or a Crawler; the last part of a share link'),
      '#default_value' => isset($this->config['connector_guid']) ? $this->config['connector_guid'] : '',
      '#maxlength' => 36,
      '#required' => TRUE,
    );

    $form['user_guid'] = array(
      '#type' => 'textfield',
      '#title' => t('User ID'),
      '#description' => t('Found at @url', array('@url' => 'https://import.io/data/account/')),
      '#default_value' => isset($this->config['user_guid']) ? $this->config['user_guid'] : '',
      '#maxlength' => 36,
      '#required' => TRUE,
    );

    $form['api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('API Key'),
      '#description' => t('Found at @url', array('@url' => 'https://import.io/data/account/')),
      '#default_value' => isset($this->config['api_key']) ? $this->config['api_key'] : '',
      '#maxlength' => 254,
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Overrides parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    // Cleanup.
    array_walk($values, 'trim');
  }

  /**
   * Define default configuration.
   */
  public function configDefaults() {
    return array(
      'connector_guid' => '',
      'user_guid' => '',
      'api_key' => '',
    );
  }

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $fetcher_result = new FeedsImportIoFetcherResult($this->getConfig());
    return $fetcher_result;
  }
}

class FeedsImportIoFetcherResult extends FeedsFetcherResult {
  protected $connectorGuid;
  protected $userGuid;
  protected $apiKey;

  /**
   * Constructor.
   */
  public function __construct($source_config = NULL) {
    if (is_array($source_config)) {
      if (isset($source_config['connector_guid'])) {
        $this->connectorGuid = $source_config['connector_guid'];
      }
      if (isset($source_config['user_guid'])) {
        $this->userGuid = $source_config['user_guid'];
      }
      if (isset($source_config['api_key'])) {
        $this->apiKey = $source_config['api_key'];
      }
    }
    parent::__construct('');
  }

  /**
   * Overrides FeedsFetcherResult::getRaw().
   */
  public function getRaw() {
    $auth = http_build_query(array(
      '_user' => $this->userGuid,
      '_apikey' => $this->apiKey,
    ));

    $url = "https://api.import.io/store/connector/{$this->connectorGuid}";

    $ch = curl_init("$url?$auth");
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'import-io-client: Drupal import.io client',
      'import-io-client-version: 0.0.0',
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    $result = curl_exec($ch);

    if (curl_error($ch)) {
      throw new Exception(
        t('cURL error (@code) @error for @url', array(
          '@code' => curl_errno($ch),
          '@error' => curl_error($ch),
          '@url' => $url,
        )), curl_errno($ch)
      );
    }

    curl_close($ch);

    $json = json_decode($result, TRUE);

    if (isset($json['error'])) {
      throw new Exception(
        t('import.io (@code) when fetching data set metadata: @error - @url', array(
          '@code' => $json['code'],
          '@error' => $json['error'],
          '@url' => "$url?$auth",
        ))
      );
    }
    elseif (!isset($json['snapshot'])) {
      throw new Exception(t('No snapshot found, was crawler executed?'));
    }

    $url .= "/_attachment/snapshot/{$json['snapshot']}";

    $ch = curl_init("$url?$auth");
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'import-io-client: Drupal import.io client',
      'import-io-client-version: 0.0.0',
    ));
    curl_setopt($ch, CURLOPT_ENCODING, '');
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    $result = curl_exec($ch);

    if (curl_error($ch)) {
      throw new Exception(
        t('cURL error (@code) @error for @url', array(
          '@code' => curl_errno($ch),
          '@error' => curl_error($ch),
          '@url' => $url,
        )), curl_errno($ch)
      );
    }

    curl_close($ch);

    $json = json_decode($result, TRUE);

    if (isset($json['error'])) {
      throw new Exception(
        t('import.io (@code) when fetching data set: @error - @url', array(
          '@code' => $json['code'],
          '@error' => $json['error'],
          '@url' => "$url?$auth",
        ))
      );
    }

    $ret_val = array(
      'title' => $json['tiles'][0]['type'],
      'link' => $url,
    );

    foreach ($json['tiles'][0]['results'][0]['pages'] as $page) {
      foreach ($page['results'] as $result) {
        $ret_val['results'][] = $result;
      }
    }

    return $ret_val;
  }
}
