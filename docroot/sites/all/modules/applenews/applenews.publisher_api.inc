<?php

/**
 * @file
 * PublisherAPI class integration.
 */

/**
 * Gets a news api object.
 *
 * @return \ChapterThree\AppleNewsAPI\PublisherAPI
 *   An initialized api object.
 */
function applenews_publisher_api() {

  $key = variable_get('applenews_variable_api_key', FALSE);
  $secret = variable_get('applenews_variable_api_secret', FALSE);
  $endpoint = variable_get('applenews_variable_api_endpoint', FALSE);

  $publisher_api = new ChapterThree\AppleNewsAPI\PublisherAPI($key, $secret, $endpoint);

  // Allow configuring curl for debugging.
  $opts = variable_get('applenews_variable_api_curlopt',
    [
      'ssl'        => TRUE,
      'proxy'      => '',
      'proxy_port' => '',
    ]
  );

  /** @var \Curl\Curl $client */
  $client = $publisher_api->client;
  $client->setOpt(CURLOPT_SSL_VERIFYPEER, $opts['ssl']);
  if (!empty($opts['proxy'])) {
    $client->setOpt(CURLOPT_PROXY, $opts['proxy']);
  }
  if (!empty($opts['proxy_port'])) {
    $client->setOpt(CURLOPT_PROXYPORT, $opts['proxy_port']);
  }

  return $publisher_api;
}

/**
 * Debug log api request.
 */
function _applenews_publisher_api_log(\ChapterThree\AppleNewsAPI\PublisherAPI $publisher_api) {
  if (variable_get('applenews_variable_api_debug', FALSE)) {
    /** @var \Curl\Curl $client */
    $client = $publisher_api->client;
    watchdog('applenews', 'Publisher API Debug request: %method %url', [
      '%method' => $publisher_api->method,
      '%url' => $client->url,
    ], WATCHDOG_DEBUG);
    $value = [];
    array_walk_recursive($client->requestHeaders,
      function($a) use (&$value) {
        $value[] = $a;
      }
    );
    watchdog('applenews', 'Publisher API Debug request headers: <pre>@headers</pre>', [
      '@headers' => implode("\n", $value),
    ], WATCHDOG_DEBUG);
    watchdog('applenews', 'Publisher API Debug response headers: <pre>@headers</pre>', [
      '@headers' => $client->rawResponseHeaders,
    ], WATCHDOG_DEBUG);
    watchdog('applenews', 'Publisher API Debug response: <pre>@response</pre>', [
      '@response' => $client->rawResponse,
    ], WATCHDOG_DEBUG);
  }
}

/**
 * Fetches information about a channel.
 */
function applenews_get_channel($channel_id) {
  $publisher_api = applenews_publisher_api();
  $response = $publisher_api->get('/channels/{channel_id}',
    [
      'channel_id' => $channel_id
    ]
  );
  _applenews_publisher_api_log($publisher_api);
  if (!$response) {
    /** @var \Curl\Curl $client */
    watchdog('applenews', $publisher_api->client->errorMessage, [], WATCHDOG_ERROR);
    return FALSE;
  }
  return $response;
}

/**
 * Fetches a list of all sections for a channel.
 */
function applenews_get_sections($channel_id) {
  $publisher_api = applenews_publisher_api();
  $response = $publisher_api->get('/channels/{channel_id}/sections',
    [
      'channel_id' => $channel_id
    ]
  );
  _applenews_publisher_api_log($publisher_api);
  if (!$response) {
    /** @var \Curl\Curl $client */
    watchdog('applenews', $publisher_api->client->errorMessage, [], WATCHDOG_ERROR);
    return FALSE;
  }
  return $response;
}

/**
 * Fetches information about a single section.
 */
function applenews_get_section($section_id) {
  $publisher_api = applenews_publisher_api();
  $response = $publisher_api->get('/sections/{section_id}',
    [
      'section_id' => $section_id
    ]
  );
  _applenews_publisher_api_log($publisher_api);
  if (!$response) {
    /** @var \Curl\Curl $client */
    watchdog('applenews', $publisher_api->client->errorMessage, [], WATCHDOG_ERROR);
    return FALSE;
  }
  return $response;
}

/**
 * Fetches an article.
 *
 * @return mixed
 *   Response object, FALSE if not found, or NULL if error.
 */
function applenews_get_article($article_id) {
  $publisher_api = applenews_publisher_api();
  $response = $publisher_api->get('/articles/{article_id}',
    [
      'article_id' => $article_id
    ]
  );
  _applenews_publisher_api_log($publisher_api);
  if (empty($response)) {
    /** @var \Curl\Curl $client */
    watchdog('applenews', $publisher_api->client->errorMessage, [], WATCHDOG_ERROR);
    return NULL;
  }
  elseif (!empty($response->errors)) {
    foreach ($response->errors as $err) {
      switch ($err->code) {

        case 'NOT_FOUND':
          return FALSE;

        default:
          watchdog('applenews', $err->code, [], WATCHDOG_ERROR);
          return NULL;

      }
    }
  }
  return $response;
}

/**
 * Update existing article.
 */
function applenews_update_article($article_id, $data = []) {
  $publisher_api = applenews_publisher_api();
  $response = $publisher_api->post('/articles/{article_id}',
    [
      'article_id' => $article_id
    ],
    $data
  );
  _applenews_publisher_api_log($publisher_api);
  if (!$response) {
    /** @var \Curl\Curl $client */
    watchdog('applenews', $publisher_api->client->errorMessage, [], WATCHDOG_ERROR);
    return FALSE;
  }
  return $response;
}

/**
 * Publishes a new article to a channel.
 */
function applenews_post_article($channel_id, $data = []) {
  $publisher_api = applenews_publisher_api();
  $response = $publisher_api->post('/channels/{channel_id}/articles',
    [
      'channel_id' => $channel_id
    ],
    $data
  );
  _applenews_publisher_api_log($publisher_api);
  if (!$response) {
    /** @var \Curl\Curl $client */
    watchdog('applenews', $publisher_api->client->errorMessage, [], WATCHDOG_ERROR);
    return FALSE;
  }
  return $response;
}

/**
 * Deletes an article.
 */
function applenews_delete_article($article_id) {
  $publisher_api = applenews_publisher_api();
  $response = $publisher_api->delete('/articles/{article_id}',
    [
      'article_id' => $article_id
    ]
  );
  _applenews_publisher_api_log($publisher_api);
  if (!empty($response)) {
    /** @var \Curl\Curl $client */
    watchdog('applenews', $publisher_api->client->errorMessage, [], WATCHDOG_ERROR);
    return FALSE;
  }
  return $response;
}
