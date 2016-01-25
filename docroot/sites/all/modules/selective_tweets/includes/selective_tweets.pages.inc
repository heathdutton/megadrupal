<?php
/**
 * @file
 * Provides page callbacks and their helper functions.
 */

/**
 * Page callback; Function that returns the block content for a Twitter feed.
 *
 * @param $config
 *   The block configuration array.
 * @param $page
 *   Page to load.
 * @param $type
 *   Type of callback, ajax or nojs.
 *
 * @return
 *   Render array for the block.
 */
function selective_tweets_twitter_feed_build($config, $page = 0, $type = 'nojs') {
  $data = $config['data'];
  $bid = $config['bid'];

  // When first loaded, take initial_count as number of tweets to load.
  $count = ($type == 'ajax') ? $data['count'] : $data['initial_count'];

  // Retrieve latest tweet ids from cache.
  try {
    $display_mode = $data['display_mode'];
    $tweet_ids = selective_tweets_twitter_feed_get($config, $display_mode, $page);
  } catch (Exception $ex) {
    // Serve fault message when something went wrong.
    unset($_SESSION['messages']);
    drupal_set_message($ex->getMessage(), 'error');
    $render['tweet_list']['#markup'] = theme('status_messages');

    return $render;
  }

  // Render the tweets.
  $render['tweet_list']['#markup'] = selective_tweets_feed_render($tweet_ids, $config);

  // Determine whether a load more link is needed or not.
  if (count($tweet_ids) == $count && $data['count'] != 0) {
    $render['load_more'] = array(
      '#theme' => 'link',
      '#text' => t('Load more'),
      '#path' => 'selective-tweets/feed/load/' . $bid . '/' . (++$page) . '/nojs',
      '#options' => array(
        'html' => TRUE,
        'attributes' => array(
          'id' => 'load-more-tweets-bid-' . $bid,
          'class' => array('use-ajax', 'load-more'),
        ),
      ),
    );
  }

  // If ajax request, insert the ajax commands and stop the page rendering.
  if ($type == 'ajax') {
    $commands[] = ajax_command_invoke('#block-selective-tweets-' . $bid . ' .selective-tweet:last', 'after', array(drupal_render($render['tweet_list'])));
    if (!empty($render['load_more'])) {
      $commands[] = ajax_command_replace('#load-more-tweets-bid-' . $bid, drupal_render($render['load_more']));
    }
    else {
      $commands[] = ajax_command_remove('#load-more-tweets-bid-' . $bid);
      // During the last page load, behaviors are not executed for an unknown
      // reason. We execute it here.
      $commands[] = array(
        'command' => 'selective_tweets_attach_behaviors',
        'container' => '#block-selective-tweets-' . $bid,
      );
    }

    // Render as ajax response.
    ajax_deliver(array(
      '#type' => 'ajax',
      '#commands' => $commands,
    ));

    drupal_exit();
  }

  return $render;
}

/**
 * Helper function to render a twitter feed based on tweet IDs.
 *
 * @param $ids
 *   An array of tweet IDs to be rendered.
 * @param $block_config
 *   The configuration array of the block.
 *
 * @return
 *   A string containing the rendered tweets (HTML).
 */
function selective_tweets_feed_render($ids, $block_config) {
  $output = '';

  try {
    foreach ($ids as $tid) {
      $output .= selective_tweets_tweet_render($tid, $block_config);
    }
  } catch (Exception $ex) {
    drupal_set_message(t("Sorry, we're not able to display Tweets at this moment. Try again later."), 'error');
    watchdog('selective_tweets', $ex->getMessage(), array(), WATCHDOG_ERROR);
  }

  return $output;
}

/**
 * Helper function to convert one tweet ID to HTML.
 *
 * @param $tid
 *   The tweet ID.
 * @param $block_config
 *   The configuration array of the block.
 *
 * @return
 *   A tweet as HTML.
 */
function selective_tweets_tweet_render($tid, $block_config) {
  // We separate the tweets per block in the cache table, so
  // they can be cleared separately.
  $rendered_tweet_cid = 'selective_tweets::bid_' . $block_config['bid'] . '_rendered_tweets';
  $cache = cache_get($rendered_tweet_cid);

  // Check if rendered Tweet is cached already.
  if ($cache && isset($cache->data[$tid])) {
    return $cache->data[$tid];
  }
  else {
    try {
      // Get the current cache to add the new tweet to it.
      $current_rendered_cache = array();
      if (is_object($cache) && isset($cache->data)) {
        $current_rendered_cache = $cache->data;
      }

      // Authentication Twitter API.
      $connection = selective_tweets_twitter_connect();
      $api_version = variable_get('selective_tweets_api_version', SELECTIVE_TWEETS_API_VERSION);

      switch ($block_config['data']['render_mode']) {
        case SELECTIVE_TWEETS_EMBED:
          $params = array(
            'id' => $tid,
            'hide_media' => $block_config['data']['hide_media'],
            'hide_thread' => $block_config['data']['hide_thread'],
            'maxwidth' => $block_config['data']['maxwidth'],
            'omit_script' => TRUE,
          );

          // Convert a tweet ID to an embedded Tweet via the Twitter API.
          $response = json_decode(
            $connection->oAuthRequest(
              'https://api.twitter.com/' . $api_version . '/statuses/oembed.json',
              'GET',
              $params
            )
          );

          $current_rendered_cache[$tid] = '<div class="selective-tweet tweet-id-' . $tid . '">' . $response->html . '</div>';
          break;

        case SELECTIVE_TWEETS_RAW:
          // Get the cached raw tweet data.
          $raw_tweet_cid = 'selective_tweets::bid_' . $block_config['bid'] . '_raw_data_tweets';
          $raw_tweet_cache = cache_get($raw_tweet_cid);

          // Check if rendered Tweet is cached already.
          if ($raw_tweet_cache && isset($raw_tweet_cache->data[$tid])) {
            $response = $raw_tweet_cache->data[$tid];
          }
          else {
            // If any, add current cache to newly built cache.
            $current_raw_cache = array();
            if (is_object($raw_tweet_cache) && isset($raw_tweet_cache->data)) {
              $current_raw_cache = $raw_tweet_cache->data;
            }

            $params = array(
              'id' => $tid,
              'trim_user' => FALSE,
              'include_my_retweet' => FALSE,
              'include_entities' => TRUE,
            );

            // Convert a tweet ID to an embedded Tweet via the Twitter API.
            $response = json_decode(
              $connection->oAuthRequest(
                'https://api.twitter.com/' . $api_version . '/statuses/show.json',
                'GET',
                $params
              )
            );

            // Convert entities to links in the Tweet.
            if ($block_config['data']['insert_links']) {
              _selective_tweets_insert_links($response);
            }

            // Add a created_timestamp to the Tweet opbject so it's easier to
            // choose how to display the datetime through overrides.
            if (isset($response->created_at) && $response->created_at && $created_timestamp = _selective_tweets_get_timestamp($response->created_at)) {
              $response->created_timestamp = $created_timestamp;
            }

            // Add Tweet action links to the Tweet object to use in the tpl.
            $response->link_reply_to = 'https://twitter.com/intent/tweet?in_reply_to=' . $response->id_str;
            $response->link_retweet = 'https://twitter.com/intent/retweet?tweet_id=' . $response->id_str;
            $response->link_favorite = 'https://twitter.com/intent/favorite?tweet_id=' . $response->id_str;

            // Cache the raw Tweet data.
            $current_raw_cache[$tid] = $response;
            cache_set($raw_tweet_cid, $current_raw_cache, 'cache', CACHE_PERMANENT);
          }

          // Add the time ago value. We do this after the data is retrieved from
          // cache because this has to be up to date.
          if (isset($response->created_timestamp) && $response->created_timestamp && $time_ago = _selective_tweets_get_time_ago($response->created_timestamp)) {
            $response->time_ago = $time_ago;
          }

          $current_rendered_cache[$tid] = theme('selective_tweet_raw', array('tweet' => $response));
          break;
      }

      // Cache the rendered Tweet if requested.
      if ($block_config['data']['render_mode'] == SELECTIVE_TWEETS_EMBED || $block_config['data']['cache_rendered_tweet']) {
        cache_set($rendered_tweet_cid, $current_rendered_cache, 'cache', CACHE_PERMANENT);
      }

      return $current_rendered_cache[$tid];

    } catch (Exception $ex) {
      watchdog('selective_tweets', $ex->getMessage(), array(), WATCHDOG_ERROR);
    }
  }

  return '';
}

/**
 * Helper function to get the tweets from tweet stack in cache. If the caching
 * time exceeded, the cache will be supplemented with new tweets.
 *
 * @param $block_config
 *   The configuration array of the block.
 * @param $display_mode
 *   The type of feed to load: live or manual.
 * @param $page
 *   The page to load.
 *
 * @return
 *   An array containing the tweet IDs.
 */
function selective_tweets_twitter_feed_get($block_config, $display_mode = NULL, $page = 0) {
  // If caching time period exceeded, fill cache with new tweets, but keep the
  // old ones as they can still be displayed.
  $cid = 'selective_tweets::bid_' . $block_config['bid'] . '_tweet_stack';

  $cache = cache_get($cid);

  // Only get new Tweets from API when first page is loaded AND when no cache is
  // available or the Selective Tweets cache for this block is expired.
  if ((REQUEST_TIME >= $block_config['pull_cache_expire'] || !$cache) && $page == 0) {
    // If the Tweet stack cache is expired, we drop it and refill the stack with
    // a fresh list of Tweets.
    if (REQUEST_TIME >= $block_config['stack_cache_expire'] && $block_config['stack_cache_expire'] != 0) {
      $cache = NULL;
    }

    // Add newest Tweets to the tweet stack in cache.
    $tweet_ids = selective_tweets_build_cache($cid, $cache, $block_config, $display_mode);
  }
  else {
    $tweet_ids = $cache->data;
  }

  // Get the tweets of the requested page out of the array.
  // These calculations are made because the initial number of tweets can differ
  // from the ones that get loaden on the next pages.
  if ($page > 0) {
    $offset = $block_config['data']['initial_count'] + (--$page * $block_config['data']['count']);
    $count = $block_config['data']['count'];
  }
  else {
    $offset = 0;
    $count = $block_config['data']['initial_count'];
  }

  return array_slice($tweet_ids, $offset, $count);
}

/**
 * Helper function to get tweets from the Twitter API and cache them in order
 * to save expensive calls to Twitter.
 *
 * @param $cid
 *   The cache ID where the tweets for this block are cached.
 * @param $cache
 *   The currently cached tweet IDs.
 * @param $block_config
 *   The configuration array of the block.
 * @param $display_mode
 *   The mode which tweets to display.
 *
 * @return
 *   An array containing all cached tweet IDs.
 */
function selective_tweets_build_cache($cid, $cache, $block_config, $display_mode) {
  // Get currently cached tweets and the last cached tweet id.
  $cached_tweet_ids = array();
  $since_id = NULL;
  if ($cache) {
    $cached_tweet_ids = $cache->data;
    $since_id = reset($cached_tweet_ids);
  }

  $api_version = variable_get('selective_tweets_api_version', SELECTIVE_TWEETS_API_VERSION);
  $data = $block_config['data'];

  // Authentication Twitter API.
  $connection = selective_tweets_twitter_connect();

  // Default params for all requests.
  $params = array(
    'screen_name' => $data['screen_name'],
    'trim_user' => TRUE,
    'exclude_replies' => TRUE,
    'include_rts' => $data['include_retweets'],
    'include_entities' => FALSE,
    'result_type' => 'recent',
  );

  switch ($display_mode) {
    case ST_SELECTION_MODE_SEARCH:
      $params += array(
        'count' => 200,
        'q' => '',
      );
      if ($data['from_account'] != '') {
        $params['q'] .= ($params['q'] == '') ? 'from:' : ' from:';
        $data['from_account'] = str_replace(' ', ' OR from:', $data['from_account']);
        $params['q'] .= $data['from_account'];
      }
      if ($data['to_account'] != '') {
        $params['q'] .= ($params['q'] == '') ? 'to:' : ' to:';
        $data['to_account'] = str_replace(' AND ', ' AND to:', $data['to_account']);
        $data['to_account'] = str_replace(' OR ', ' OR to:', $data['to_account']);
        $params['q'] .= $data['to_account'];
      }
      if ($data['user_mentions'] != '') {
        $params['q'] .= ($params['q'] == '') ? '@' : ' @';
        $data['user_mentions'] = str_replace(' AND ', ' AND @', $data['user_mentions']);
        $data['user_mentions'] = str_replace(' OR ', ' OR @', $data['user_mentions']);
        $params['q'] .= $data['user_mentions'];
      }
      if ($data['hashtags'] != '') {
        $params['q'] .= ($params['q'] == '') ? '#' : ' #';
        $data['hashtags'] = str_replace(' AND ', ' AND #', $data['hashtags']);
        $data['hashtags'] = str_replace(' OR ', ' OR #', $data['hashtags']);
        $params['q'] .= $data['hashtags'];
      }
      if (!$data['include_retweets']) {
        $params['q'] .= ($params['q'] == '') ? '' : ' ';
        $params['q'] .= '-filter:retweets';
      }
      $endpoint = 'https://api.twitter.com/' . $api_version . '/search/tweets.json';
      break;

    case ST_SELECTION_MODE_FAVORITES:
      $params += array(
        'count' => 200,
      );
      $endpoint = 'https://api.twitter.com/' . $api_version . '/favorites/list.json';
      break;

    case ST_SELECTION_MODE_LIVE:
      $params += array(
        'count' => 200,
      );
      $endpoint = 'https://api.twitter.com/' . $api_version . '/statuses/user_timeline.json';
  }

  // If we don't have to load extra Tweets, we might just as well request only
  // the number of Tweets on the initial page. We add 5 more Tweets to
  // compensate deleted Tweets, which Twitter removes from the response after
  // the count has been applied in its query.
  if ($block_config['data']['count'] == 0) {
    $params['count'] = $block_config['data']['initial_count'] + 5;
  }

  // If we already cached tweets, only get tweets that come after the last
  // cached tweet.
  if ($since_id) {
    $params += array('since_id' => $since_id);
  }

  // Make the actual request to Twitter.
  $response = json_decode(
    $connection->oAuthRequest(
      $endpoint,
      'GET',
      $params
    )
  );

  $tweet_ids = array();

  if (is_object($response)) {
    if (isset($response->errors)) {
      foreach ($response->errors as $error) {
        watchdog('selective_tweets', 'Twitter API: @error_code: @error_message', array(
          '@error_code' => $error->code,
          '@error_message' => $error->message,
        ), WATCHDOG_ERROR);
      }
      throw new Exception(t('The tweets could not be loaded.'));
    }
    $response = $response->statuses;
  }

  foreach ($response as $tweet) {
    $tweet_ids[] = $tweet->id_str;
  }

  // Merge old and new Tweets into one array.
  $new_tweet_stack = array_merge($tweet_ids, $cached_tweet_ids);

  // Limit the Tweet stack to the maximum.
  $new_tweet_stack = array_slice($new_tweet_stack, 0, $block_config['data']['tweet_stack_max']);

  // Save the new tweet stack in cache.
  cache_set($cid, $new_tweet_stack, 'cache', CACHE_PERMANENT);

  // Update the cache_expire time for this Selective Tweets block.
  db_update('selective_tweets_block')
    ->fields(array(
      'pull_cache_expire' => (REQUEST_TIME + ($block_config['data']['pull_cached_time'] * 60)),
      'stack_cache_expire' => (REQUEST_TIME + ($block_config['data']['stack_cached_time'] * 60)),
    ))
    ->condition('bid', $block_config['bid'])
    ->execute();

  return $new_tweet_stack;
}

/**
 * Helper function to convert hashtags, users and urls to links.
 *
 * @param $tweet
 *   The tweet object returned from the Twitter API.
 */
function _selective_tweets_insert_links(&$tweet) {
  $twitter_url = 'https://twitter.com';

  // Convert hashtags.
  $tweet->text = _selective_tweets_filter_text($tweet->text, '#', $twitter_url . '/hashtag/');

  // Convert user mentions.
  $tweet->text = _selective_tweets_filter_text($tweet->text, '@', $twitter_url . '/');

  // Convert urls.
  if (isset($tweet->entities->urls) && !empty($tweet->entities->urls)) {
    foreach ($tweet->entities->urls as $key => $url) {
      $link = l($url->display_url, $url->expanded_url, array(
        'attributes' => array(
          'target' => '_blank',
          'rel' => 'nofollow',
        ),
      ));
      $tweet->text = str_replace($url->url, $link, $tweet->text);
    }
  }
}

/**
 * Helper function to convert user mentions and hashtags into
 * actual links.
 *
 * @param $text
 *   The Tweet text.
 * @param $prefix
 *   The entity prefix (@ or #).
 * @param $destination
 *   The destination url base.
 *
 * @return
 *   The Tweet text with links.
 */
function _selective_tweets_filter_text($text, $prefix, $destination) {
  $matches = array(
    '/\>' . $prefix . '(\w+)/ui',
    '/^' . $prefix . '(\w+)/ui',
    '/(\s+)' . $prefix . '(\w+)/ui',
  );
  $replacements = array(
    '><a target="_blank" rel="nofollow" href="' . $destination . '${1}">' . $prefix . '${1}</a>',
    '<a target="_blank" rel="nofollow" href="' . $destination . '${1}">' . $prefix . '${1}</a>',
    '${1}<a target="_blank" rel="nofollow" href="' . $destination . '${2}">' . $prefix . '${2}</a>',
  );

  return preg_replace($matches, $replacements, $text);
}

/**
 * Helper function to convert the Twitter created_at value to a timestamp.
 *
 * @param $created_at
 *   The created_at value from the Twitter API.
 *
 * @return
 *   The created timestamp.
 */
function _selective_tweets_get_timestamp($created_at) {
  $timezone = variable_get('date_default_timezone', FALSE);
  if (module_exists('date_api')) {
    $timezone = date_default_timezone(TRUE);
  }

  $datetime = new DateTime($created_at);
  if ($timezone) {
    $datetime->setTimezone(new DateTimeZone($timezone));
  }

  return $datetime->format('U');
}

/**
 * Helper function to convert the Twitter created_timestamp value to a
 * time-ago value.
 *
 * @param $created_timestamp
 *   The created_timestamp.
 *
 * @return
 *   The time ago as string.
 */
function _selective_tweets_get_time_ago($created_timestamp = NULL) {
  $time_ago = '';

  if ($created_timestamp) {
    $time_ago = t('@time ago', array('@time' => format_interval(REQUEST_TIME - $created_timestamp , 2)));
  }

  return $time_ago;
}
