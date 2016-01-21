<?php

/**
 * @file
 * Implementation of Twitter Field Pull API methods.
 */


/**
 * Update the Twitter Field Pull Tweet table with new tweets.
 *
 * Only tweets newer than last update will be saved.
 */

function _twitterfield_pull_tweet(&$job = array()) {

  $retrieve_name = $job['data']['name'];
  $current_tweet = (isset($job['data']['current_tweet'])) 
    ? $job['data']['current_tweet'] : 1;

  // Check.
  if (! $retrieve_name) {
    watchdog('twitterfield_pull', 'Twitterfield Pull: No Twitter account or '
      . 'hashtag is set. New tweets can not be retrieved.', NULL, WATCHDOG_INFO);
    return;
  }

  // Grab the user name without prefix if it starts with @.
  elseif (strcmp(drupal_substr($retrieve_name, 0, 1), '@') === 0) {
    $retrieve_name = drupal_substr($retrieve_name, 1, 
      drupal_strlen($retrieve_name));
    $type = 'user';
  }

  // Grab hashtag without prefix if it starts with #.
  elseif (strcmp(drupal_substr($retrieve_name, 0, 1), '#') === 0) {
    $retrieve_name = drupal_substr($retrieve_name, 1, 
      drupal_strlen($retrieve_name));
    $type = 'hashtag';
  }
  else {
    watchdog('twitterfield_pull', 'Twitterfield Pull: No valid twitter '
      . '(@retrieve_name) account or hashtag is set. New tweets can not be '
      . 'retrieved.', array('@retrieve_name' => $retrieve_name), WATCHDOG_ERROR);
    return;
  }

  $current_tweet = _twitterfield_pull_get_last_tid($retrieve_name, $type);

  // Params to retrieve Twitter feed.
  // Only get Tweets newer than last retrieved Tweet.
  $parameters = array('since_id' => $current_tweet);

  switch ($type) {
    case 'user':
      $parameters['screen_name'] = $retrieve_name;
      break;

    case 'hashtag':
      $parameters['q'] = '#' . $retrieve_name;
      break;
  }

  watchdog('twitterfield_pull', 'Trying to get tweets for @retrieve_name '
    . '(@type) - Last tweet id is @$current_tweet', array(
    '@retrieve_name' => $retrieve_name,
    '@type' => $type,
    '@$current_tweet' => $current_tweet)
  );

  $tweets_data = _twitterfield_pull_retrieve_tweets($type, $parameters);

  // If there's an error, $tweets_data will be an object e.g.
  // $tweets_data: stdClass Object (
  //   [errors] => Array (
  //     [0] => stdClass Object (
  //       [message] => 'Sorry, that page does not exist'
  //       [code] => 34
  //     )
  //   )
  // )

  if (is_object($tweets_data) && property_exists($tweets_data, 'errors')) {
    watchdog('twitterfield_pull', 'Error retrieving tweets: @error',
      array('@error' => print_r($tweets_data, TRUE)), WATCHDOG_ERROR);
  }
  else {

    // For debugging tweets retrieved.
    // watchdog('twitterfield_pull', 'Tweets from OAuth: @tweets', array(
    //   '@tweets' => print_r($tweets_data, TRUE)));

    // Record number of tweets processed.
    $job['data']['progress'] = 0;

    $job['data']['max'] = count($tweets_data);
    
    switch ($type) {
      case 'user':
        foreach ($tweets_data as $tweet) {

          $created = strtotime($tweet->created_at);
          // Get tweet only if it is newer than the last update.
          try {
            $postid = db_merge(TWITTERFIELD_PULL_TABLE)
              ->key(array('tid' => $tweet->id))
              ->fields(array(
                'nid' => $job['id'],
                'field_name' => $job['type'],
                'tid' => $tweet->id,
                'timestamp' => $created,
                'text' => 
                  _twitterfield_pull_sanitize_multibyte_utf8($tweet->text),
                'source' => $tweet->source,
                'replyto' => $tweet->in_reply_to_screen_name,
                'name' => $retrieve_name,
                'type' => 'user',
                'user_id' => $tweet->user->id,
                'user_screen_name' => $tweet->user->name,
                'user_profile_pic' => $tweet->user->profile_image_url,
              ))
              ->execute();
            $job['data']['progress']++;
            $job['data']['current_tweet'] = $tweet->id;
            $job['data']['message'] = t('Now processing @tweet', 
              array('@tweet' => $tweet->id));
          }
          catch (Exception $e) {
            watchdog('twitterfield_pull', 'Error updating DB. Error message: '
              . '@errmsg . Tweet data: @tweet .', array(
              '@errmsg' => $e->getMessage(), 
              '@tweet' => print_r($tweet, TRUE)), 
              WATCHDOG_ERROR);
          }
        }
        break;

      case 'hashtag':
        foreach ($tweets_data as $tweet) {
          $created = strtotime($tweet->pubDate);

          try {
            $urlvars = explode('/', $tweet->link);
            $tid = $urlvars[5];
            $user_name = substr($urlvars[3], 0, 50);
            $postid = db_merge(TWITTERFIELD_PULL_TABLE)
              ->key(array('tid' => $tid))
              ->fields(array(
                'nid' => $job['id'],
                'field_name' => $job['type'],
                'tid' => $tid,
                'timestamp' => $created,
                'text' => 
                  _twitterfield_pull_sanitize_multibyte_utf8($tweet->description),
                'source' => '',
                'replyto' => '',
                'name' => $retrieve_name,
                'type' => 'hashtag',
                'user_id' => 0,
                'user_screen_name' => $user_name,
                'user_profile_pic' => '',
              ))
              ->execute();

            $job['data']['progress']++;
            $job['data']['current_tweet'] = $tid;
            $job['data']['message'] = t('Now processing @tweet', 
              array('@tweet' => $tid));

          }
          catch (Exception $e) {
            watchdog('twitterfield_pull', 'Error updating DB. Error message: '
              . '@errmsg . Tweet data: @tweet .', array(
              '@errmsg' => $e->getMessage(), 
              '@tweet' => print_r($tweet, TRUE)), 
              WATCHDOG_ERROR);
          }
        }
        break;
    }
  }
  watchdog('twitterfield_pull', 'Finished pulling tweets for @retrieve_name '
    . '(@type) - @progress new tweet', array(
    '@retrieve_name' => $retrieve_name,
    '@type' => $type,
    '@progress' => $job['data']['progress'])
  );
}


/**
 * Retrieve the tweets using OAuth.
 *
 * @param string $type
 *   Type of feed to get: 'user' (user statuses) or 'hashtag' (tweets with a
 *   hashtag.
 *   If 'hashtag', $parameters must include 'q' => '#[tag]'.
 *   If 'user', $parameters must include 'screen_name' => '[Twitter username]'.
 * @param array $parameters
 *   Array of parameters e.g. array('screen_name' => 'markzuckerberg', 
 *   'count' => 2).
 */
function _twitterfield_pull_retrieve_tweets($type, array $parameters) {

  // Load in OAuth class.
  module_load_include('inc','twitterfield_pull','twitter_oauth');

  $url = 'https://api.twitter.com/1.1/statuses/user_timeline.json';
  if ($type == 'hashtag') {
    $url = 'https://api.twitter.com/1.1/search/tweets.json';
  }

  /* Build TwitterOAuth object with client credentials. */
  $con = new TwitterOAuth(
    variable_get('twitterfield_pull_oauth_consumer_key'),
    variable_get('twitterfield_pull_oauth_consumer_secret'),
    variable_get('twitterfield_pull_oauth_token'),
    variable_get('twitterfield_pull_oauth_token_secret')
  );

  /* get the tweets */
  $tdata = json_decode(
    $con->oAuthRequest($url, 'GET', $parameters)
  );

  return $tdata;
}


/**
 * Convert a 4-byte multibyte UTF-8 string to a 3-byte one.
 * 
 * MySQL has problems storing a 4-byte multibyte UTF-8 string, like an emoji.
 * Error emitted:
 * "SQLSTATE[HY000]: General error: 1366 Incorrect string value: '\xF0\x9F\x94\xA5\xF0\x9F...'"
 * So we convert them to 3-byte one. Solution from here:
 * http://www.avidheap.org/2013/a-quick-way-to-normalize-a-utf8-string-when-your-mysql-database-is-not-utf8mb4
 *
 * @param $input
 *   A piece of text to convert.
 * @return
 *   Text in correct encoding.
 */
function _twitterfield_pull_sanitize_multibyte_utf8($input) {

  if (empty($input)) {
    return $input;
  }

  if (extension_loaded('mbstring')) {
    $detected_encoding = mb_detect_encoding($input);

    // Only need to convert if encoding is 4-byte UTF-8.
    if ($detected_encoding != 'UTF-8') {
      return $input;
    }
  }

  $utf8_2byte = 0xC0 /*1100 0000*/; $utf8_2byte_bmask = 0xE0 /*1110 0000*/;
  $utf8_3byte = 0xE0 /*1110 0000*/; $utf8_3byte_bmask = 0XF0 /*1111 0000*/;
  $utf8_4byte = 0xF0 /*1111 0000*/; $utf8_4byte_bmask = 0xF8 /*1111 1000*/;

  $sanitized = "";
  $len = strlen($input);
  for ($i = 0; $i < $len; ++$i) {
    $mb_char = $input[$i]; // Potentially a multibyte sequence
    $byte = ord($mb_char);
    if (($byte & $utf8_2byte_bmask) == $utf8_2byte) {
      $mb_char .= $input[++$i];
    }
    else if (($byte & $utf8_3byte_bmask) == $utf8_3byte) {
      $mb_char .= $input[++$i];
      $mb_char .= $input[++$i];
    }
    else if (($byte & $utf8_4byte_bmask) == $utf8_4byte) {
      // Replace with ? to avoid MySQL exception
      $mb_char = '?';
      $i += 3;
    }

    $sanitized .=  $mb_char;
  }

  return $sanitized;
}


/*
 * Get Latest tweet id from specific name.
 * 
 */
function _twitterfield_pull_get_last_tid($name, $type) {
  $last_tid = db_select('twitterfield_pull_tweet','t')
    ->fields('t', array('tid'))
    ->condition('name', $name)
    ->condition('type', $type)
    ->orderBy('tid', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetchObject();
  if ($last_tid) {
    return $last_tid->tid;
  }
  else {
    return 1;
  }
}


/*
 * Save Job in schedule DB.
 * 
 */
function _twitterfield_pull_set_job($nid, $field, $data, $period = 3600) {
  $job = array(
    'id' => $nid,
    'type' => $field,
    'data' => $data,
    'period' => $period,
    'periodic' => TRUE,
  );
  JobScheduler::get('twitterfield_pull')->set($job);
}


/*
 * Delete Job in schedule DB.
 */
function _twitterfield_pull_delete_job($nid, $field) {
  $job = array(
    'id' => $nid,
    'type' => $field,
  );
  JobScheduler::get('twitterfield_pull')->remove($job);
}

/**
 * Helper hook_node_save().
 */
function _twitterfield_pull_node_save($node) {

  // Each node type which are enabled.
  $node_types = variable_get('twitterfield_pull_node_types', array());
  $frequency = variable_get('twitterfield_pull_update_frequency', 3600);

  // If content type is selected.
  if (in_array($node->type, $node_types)) {
    $selected_fields = twitterfield_pull_get_selected_field($node->type);

    foreach ($selected_fields as $field_name) {
      $field_info = field_info_field($field_name);

      // Delete old job scedulers.
      _twitterfield_pull_delete_job($node->nid, $field_name);

      $language = field_language('node', $node, $field_name);
      $fields = field_get_items('node', $node, $field_name, $language);

      foreach ($fields as $field) {

        switch ($field_info['type']) {
          case 'twitter_username':
            $name = ($field['twitter_username'] != '') ? '@' . $field['twitter_username'] : '';
            break;

          default:
            $name = ($field['value'] != '') ? $field['value'] : '';
        }

        if ($node->nid && $name != '') {
          $data = array('name' => $name, 'field_name' => $field_name);
          _twitterfield_pull_set_job($node->nid, $field_name, $data, $frequency);
        }

      }

    }
  }

}

/**
 * Batch callback for importing all the tweets.
 */
function twitterfield_pull_bacth() {
  drupal_set_message(t('start batch'));
  // Get all jobs done.
  $jobs = db_select('job_schedule', NULL, array('fetch' => PDO::FETCH_ASSOC))
    ->fields('job_schedule')
    ->condition('name', 'twitterfield_pull')
    ->condition('scheduled', 0)
    ->execute();
  foreach ($jobs as $job) {
    drupal_set_message(t('start processing @name', array('@name' => unserialize($job['data']))));
    _twitterfield_pull_tweet($job);
  }
  return '';
}


/**
 * Get selected twitter fields for a bundle.
 *
 * @param $bundle
 */
function twitterfield_pull_get_selected_field($bundle) {
  $track_field_changess = db_select('twitterfield_pull', 'a')->fields('a')->condition('bundle', $bundle)->execute()->fetchCol();
  $options = array();
  foreach ($track_field_changess as $track_field_changes) {
    $options[$track_field_changes] = $track_field_changes;
  }
  return $options;
}
