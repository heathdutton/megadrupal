<?php

/**
 * @file
 * Implementation of Twitter DB API methods.
 */
define('TWITTER_DB_TABLE', 'twitter_db');
define('TWITTER_DB_URL', 'twitter.com');
define('TWITTER_DB_SETTINGS', 'admin/config/media/twitter-db');

/**
 * Update the Twitter DB table with new tweets. Only tweets newer than last update will be saved
 */
function _twitter_db_update(&$context) {
  $retrieve_name = variable_get('twitter_db_retrieve_name', '');
  $type = variable_get('twitter_db_what_to_retrieve', 'user');
  if (!$retrieve_name) {
    drupal_set_message(t('Twitter DB: No Twitter account or hashtag is set. New tweets can not be retrieved.'), 'warning');
    return;
  }
  
  // Retrieve the tweets
  $tweets = _twitter_db_retrive();

  if ($tweets) {
    if (!isset($context['sandbox']['progress'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['current_tweet'] = 0;
      $context['sandbox']['max'] = count($tweets);
    }
    
    foreach ($tweets as $key => $tweet) {
      try {
        $tid = $tweet->id;
        $postid = db_merge(TWITTER_DB_TABLE)
            ->key(array('tid' => $tid))
            ->fields(array(
              'tid' => $tid,
              'created' => strtotime($tweet->created_at),
              'text' => $tweet->text,
              'source' => $tweet->source,
              'replyto' => $tweet->in_reply_to_screen_name,
              'name' => $retrieve_name,
              'type' => $type,
              'user_id' => $tweet->user->id,
              'user_name' => $tweet->user->name,
              'user_profile_pic' => $tweet->user->profile_image_url,
            ))
            ->execute();
        $context['results'][] = check_plain($tweet->id);
        $context['sandbox']['progress']++;
        $context['sandbox']['current_tweet'] = $tweet->id;
        $context['message'] = t('Now processing @tweet', array('@tweet' => $tweet->id));
      }
      catch (Exception $e) {
        watchdog('Twitter DB: update', $e->getMessage());
      }
    }
    
      if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
      }
      variable_set('twitter_db_last_update', time());
    
    
  }
  else {
    $message = t('Error when retrieving tweets.');
    watchdog('Twitter DB', $message, NULL, WATCHDOG_ERROR);
  }


  /**
   * The old code that stoped work when Twitter changed its API
   */
//  switch ($type) {
//    case 'user':
//      $rss_url = "https://api.twitter.com/1/statuses/user_timeline/{$retrieve_name}.xml";
//      break;
//    case 'hashtag':
//      $rss_url = "http://search.twitter.com/search.rss?q={$retrieve_name}";
//      break;
//  }
//  $rss = drupal_http_request($rss_url);
//  if ($rss->code == 200) {
//    $xml = simplexml_load_string($rss->data);
//    if ($xml !== false) {
////      $result = db_truncate(TWITTER_DB_TABLE)->execute();
//      switch ($type) {
//        case 'user':
//          if (!isset($context['sandbox']['progress'])) {
//            $context['sandbox']['progress'] = 0;
//            $context['sandbox']['current_tweet'] = 0;
//            $context['sandbox']['max'] = count($xml->status);
//          }
//          foreach ($xml->status as $tweet) {
//            $created = strtotime($tweet->created_at);
//            // Get tweet only if it is newer than the last update
//            try {
//              $tid = $tweet->id;
//              $postid = db_merge(TWITTER_DB_TABLE)
//                  ->key(array('tid' => $tid))
//                  ->fields(array(
//                    'tid' => $tid,
//                    'created' => $created,
//                    'text' => $tweet->text,
//                    'source' => $tweet->source,
//                    'replyto' => $tweet->in_reply_to_screen_name,
//                    'name' => $retrieve_name,
//                    'type' => 'user',
//                    'user_id' => $tweet->user->id,
//                    'user_name' => $tweet->user->name,
//                    'user_profile_pic' => $tweet->user->profile_image_url,
//                  ))
//                  ->execute();
//              $context['results'][] = check_plain($tweet->id);
//              $context['sandbox']['progress']++;
//              $context['sandbox']['current_tweet'] = $tweet->id;
//              $context['message'] = t('Now processing @tweet', array('@tweet' => $tweet->id));
//            } catch (Exception $e) {
//              watchdog('Twitter DB: update', $e->getMessage());
//            }
//          }
//          break;
//
//        case 'hashtag':
//          $last_update = variable_get('twitter_db_last_update', 0);
//          if (!isset($context['sandbox']['progress'])) {
//            $context['sandbox']['progress'] = 0;
//            $context['sandbox']['current_tweet'] = 0;
//            $context['sandbox']['max'] = count($xml->channel->item);
//          }
//          foreach ($xml->channel->item as $tweet) {
//            $created = strtotime($tweet->pubDate);
//            // Get tweet only if it is newer than the last update
//            try {
//              $urlvars = explode('/', $tweet->link);
//              $tid = $urlvars[5];
//              $user_name = $urlvars[3];
//              $user = _twitter_db_get_user_profile($user_name);
//              if ($user) {
//                $user_id = $user->id;
//                $user_profile_pic = $user->profile_image_url;
//              }
//              else {
//                $user_id = '';
//                $user_profile_pic = '';
//              }
//              $postid = db_merge(TWITTER_DB_TABLE)
//                  ->key(array('tid' => $tid))
//                  ->fields(array(
//                    'tid' => $tid,
//                    'created' => $created,
//                    'text' => $tweet->description,
//                    'source' => '',
//                    'replyto' => '',
//                    'name' => $retrieve_name,
//                    'type' => 'hashtag',
//                    'user_id' => $user_id,
//                    'user_name' => $user_name,
//                    'user_profile_pic' => $user_profile_pic,
//                  ))
//                  ->execute();
//
//              $context['results'][] = check_plain($tid);
//              $context['sandbox']['progress']++;
//              $context['sandbox']['current_tweet'] = $tid;
//              $context['message'] = t('Now processing @tweet', array('@tweet' => $tid));
//            } catch (Exception $e) {
//              watchdog('Twitter DB: update', $e->getMessage());
//            }
//          }
//          break;
//      }
//      if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
//        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//      }
//      variable_set('twitter_db_last_update', time());
//    }
//  }
//  else {
//    $xml = simplexml_load_string($rss->data);
//    if ($rss->code == 400) {
//      drupal_set_message('Twitter DB: ' . $xml->error, 'error');
//      $message = $xml->error;
//    }
//    else {
//      $message = 'Error when retrieving tweets.';
//    }
//    watchdog('Twitter DB', $message);
//  }
}

/**
 * Get the $quantity last tweets. If $quantity is 0, then all tweets is returned.
 * @param integer $quantity The number of tweets to return. If 0, all tweets are returned.
 * @return DatabaseStatementBase Object 
 */
function _twitter_db_get_last_tweets($quantity = 0) {
  
  $what_to_retrieve = variable_get('twitter_db_what_to_retrieve', 'user');
  $retrieve_name = variable_get('twitter_db_retrieve_name', '');
  
  if ($quantity > 0) {
    $result = db_select(TWITTER_DB_TABLE, 't')
        ->fields('t')
        ->condition('name', $retrieve_name,'=')
        ->condition('type', $what_to_retrieve,'=')
        ->orderBy('created', 'DESC')
        ->range(0, $quantity)
        ->execute();
  }
  else {
    $result = db_select(TWITTER_DB_TABLE, 't')
        ->fields('t')
        ->condition('name', $retrieve_name,'=')
        ->condition('type', $what_to_retrieve,'=')
        ->orderBy('created', 'DESC')
        ->execute();
  }
  return $result;
}

/**
 * Returns the content to show in a block or pane.
 * @return string HTML content with the last tweets.
 */
function _twitter_db_generate_content() {
  $output = '';
  $content = '';
  $quantity = variable_get('twitter_db_number_of_tweets_to_display', 3);
  $tweets = _twitter_db_get_last_tweets($quantity);
  $type = 'ul';
  $attributes = array(
    'id' => 'twitter-db-list',
    'class' => 'tweet-list',
  );
  $items = array();
  $display_user_picture = variable_get('twitter_db_display_user_picture', FALSE);
  $display_datetime = variable_get('twitter_db_display_datetime', FALSE);
  $datetime_format = variable_get('twitter_db_datetime_format', 'relative');

  foreach ($tweets as $record) {

    if ($datetime_format == 'relative') {
      $created = _twitter_db_relative_time($record->created);
    }
    else if ($datetime_format == 'month_day') {
      $created = date('M d', $record->created);
    }
    else if ($datetime_format == 'day_month') {
      $created = date('d M', $record->created);
    }

    $item_content = theme('twitter_db_tweet', array(
      'tid' => $record->tid,
      'created' => $display_datetime ? $created : '',
      'text' => _twitter_db_replaces($record->text, $record->type),
      'source' => $record->source,
      'replyto' => $record->replyto,
      'type' => $record->type,
      'user_id' => $record->user_id,
      'user_name' => $record->user_name,
      'user_profile_pic' => $display_user_picture ? $record->user_profile_pic : '',
        ));
    $items[] = array(
      'data' => $item_content,
      'id' => $record->tid,
      'class' => '',
    );
  }
  $content .= theme_item_list(array(
    'items' => $items,
    'title' => NULL,
    'type' => $type,
    'attributes' => $attributes
      ));

  $output .= theme('twitter_db_tweets', array('tweets' => $content));

  return $output;
}

/**
 * Batch for Twitter DB update
 */
function _twitter_db_update_batch() {
  $batch = array(
    'operations' => array(
      array('_twitter_db_update', array()),
    ),
    'finished' => '_twitter_db_update_finished',
    'title' => t('Updating Twitter DB table'),
    'init_message' => t('Twitter DB update is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Twitter DB update has encountered an error.'),
  );
  batch_set($batch);

  $destination = drupal_get_destination();
  if (!$destination) {
    $destination = TWITTER_DB_SETTINGS;
  }
  batch_process($destination);
}

/**
 * Batch 'finished' callback
 */
function _twitter_db_update_finished($success, $results, $operations) {
  if ($results) {
    $message = t('The Twitter DB table has been updated. @count tweets retrieved.', array('@count' => count($results)));
    $type = 'status';
  }
  else if (!$success) {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    $type = 'error';
  }
  else {
    $message = t('It was not possible to retrieve the tweets.');
    $type = 'error';
  }
  drupal_set_message($message, $type);
}

/**
 * Replaces URLs and user names in the tweet with links for $type = 'user'.
 * @param string $text The Twitter text.
 * @param string $type The of tweet.
 * @return string The tweet text, parsed or not.
 */
function _twitter_db_replaces($text, $type) {
  if ($type == 'user') {
    $output = $text;
    // Replaces URLs with a link
    $pattern_url = "/\b((?:[a-z][\w-]+:(?:\/{1,3}|[a-z0-9%])|www\d{0,3}[.]|[a-z0-9.\-]+[.][a-z]{2,4}\/)(?:[^\s()<>]+|\(([^\s()<>]+|(\([^\s()<>]+\)))*\))+(?:\(([^\s()<>]+|(\([^\s()<>]+\)))*\)|[^\s`!()\[\]{};:'\".,<>?«»“”‘’]))/i";
    $replacement_url = '<a href="$1">$1</a>';
    $output = preg_replace($pattern_url, $replacement_url, $output);
    // Replaces Twitter user names with a link to the user
    $pattern_user = "/(^|[\W])@(\w+)/i";
    $replacement_user = '$1@<a href="http://' . TWITTER_DB_URL . '/$2">$2</a>';
    $output = preg_replace($pattern_user, $replacement_user, $output);
    return $output;
  }
  return $text;
}

/**
 * Job Scheduler worker callback.
 */
function _twitter_db_update_job() {
  $context = array();
  _twitter_db_update($context);
}

function _twitter_db_set_job($period = 3600) {
  $job = array(
    'type' => 'story',
    'id' => 12,
    'period' => $period,
    'periodic' => TRUE,
  );
  JobScheduler::get('twitter_db_update')->set($job);
}

/**
 * Return the relative time to $date, in other words NOW - $date.
 * @param integer $date Date and time in timestamp format.
 * @return string The relative time between NOW and $date, ex "2 hours ago".
 */
function _twitter_db_relative_time($date) {
  $relative_to = (strlen($date) > 1) ? $date : time();
  $delta = (time() - $relative_to);
  if ($delta < 1) {
    $r = t('Right now');
  }
  else if ($delta < 60) {
    $r = round($delta) . ' ' . t('seconds ago');
  }
  else if ($delta < 120) {
    $r = ' ' . t('1 minute ago');
  }
  else if ($delta < (45 * 60)) {
    $r = round($delta / 60) . ' ' . t('minutes ago');
  }
  else if ($delta < (2 * 60 * 60)) {
    $r = ' ' . t('1 hour ago');
  }
  else if ($delta < (24 * 60 * 60)) {
    $r = round($delta / 3600) . ' ' . t('hours ago');
  }
  else if ($delta < (48 * 60 * 60)) {
    $r = ' ' . t('1 day ago');
  }
  else {
    $r = round($delta / 86400) . ' ' . t('days ago');
  }
  return $r;
}

/**
 * Parse a SimpleXML object with a user timeline and store the tweets in the database.
 * @param SimpleXMLObject $tweets The tweets in SimpleXML format.
 * @return boolean TRUE or FALSE.
 */
//function _twitter_db_parse_user_timeline($tweets) {
//  $last_update = variable_get('twitter_db_last_update', 0);
//  foreach ($tweets->status as $tweet) {
//    $created = strtotime($tweet->created_at);
//    // Get tweet only if it is newer than the last update
//    if ($created > $last_update) {
//      try {
////        $postid = db_insert(TWITTER_DB_TABLE)
////            ->fields(array(
////              'tid' => $tweet->id,
////              'created' => $created,
////              'text' => $tweet->text,
////              'source' => $tweet->source,
////              'replyto' => $tweet->in_reply_to_screen_name,
////              'type' => 'user',
////              'user_id' => $tweet->user->id,
////              'user_name' => $tweet->user->name,
////              'user_profile_pic' => $tweet->user->profile_image_url,
////            ))
////            ->execute();
//
//        $postid = db_merge(TWITTER_DB_TABLE)
//            ->key(array('tid' => $tid))
//            ->fields(array(
//              'tid' => $tweet->id,
//              'created' => $created,
//              'text' => $tweet->text,
//              'source' => $tweet->source,
//              'replyto' => $tweet->in_reply_to_screen_name,
//              'type' => 'user',
//              'user_id' => $tweet->user->id,
//              'user_name' => $tweet->user->name,
//              'user_profile_pic' => $tweet->user->profile_image_url,
//            ))
//            ->execute();
//      } catch (Exception $e) {
//        watchdog('Twitter DB: update', $e->getMessage());
//      }
//    }
//  }
//  variable_set('twitter_db_last_update', time());
//  return TRUE;
//}

/**
 * Parse a SimpleXML object with a hashtag and store the tweets in the database.
 * @param SimpleXMLObject $tweets The tweets in SimpleXML format.
 * @return boolean TRUE or FALSE.
 */
//function _twitter_db_parse_hashtag($tweets) {
//  $last_update = variable_get('twitter_db_last_update', 0);
//  foreach ($tweets->channel->item as $tweet) {
//    $created = strtotime($tweet->pubDate);
//    // Get tweet only if it is newer than the last update
//    if ($created > $last_update) {
//      try {
//        $urlvars = explode('/', $tweet->link);
//        $tid = $urlvars[5];
//        $user_name = $urlvars[3];
//        $user = _twitter_db_get_user_profile($user_name);
//        if ($user) {
//          $user_id = $user->id;
//          $user_profile_pic = $user->profile_image_url;
//        }
//        else {
//          $user_id = '';
//          $user_profile_pic = '';
//        }
////        $postid = db_insert(TWITTER_DB_TABLE)
////            ->fields(array(
////              'tid' => $tid,
////              'created' => $created,
////              'text' => $tweet->description,
////              'source' => '',
////              'replyto' => '',
////              'type' => 'hashtag',
////              'user_id' => $user_id,
////              'user_name' => $user_name,
////              'user_profile_pic' => $user_profile_pic,
////            ))
////            ->execute();
//        
//        $postid = db_merge(TWITTER_DB_TABLE)
//            ->key(array('tid' => $tid))
//            ->fields(array(
//              'tid' => $tid,
//              'created' => $created,
//              'text' => $tweet->description,
//              'source' => '',
//              'replyto' => '',
//              'type' => 'hashtag',
//              'user_id' => $user_id,
//              'user_name' => $user_name,
//              'user_profile_pic' => $user_profile_pic,
//            ))
//            ->execute();
//      } catch (Exception $e) {
//        watchdog('Twitter DB: update', $e->getMessage());
//      }
//    }
//  }
//  variable_set('twitter_db_last_update', time());
//  return TRUE;
//}

/**
 * Get and return  an user object from Twitter.
 * @param string $screen_name The user's screen name.
 * @return SimpleXMLObject/boolean The user object or FALSE if it fails.
 */
function _twitter_db_get_user_profile($screen_name) {
  $url = "https://api.twitter.com/1/users/show.xml?screen_name={$screen_name}&include_entities=true";
  $rss = drupal_http_request($url);
  if ($rss->code == 200) {
    $xml = simplexml_load_string($rss->data);
    if ($xml !== false) {
      return $xml;
    }
    else {
      return FALSE;
    }
  }
}

/**
 * *****************************************************************************
 * Added because of Twitter's new API
 */

/**
 * Build a base string.
 * @param type $baseURI
 * @param type $method
 * @param type $params
 * @return type
 */
function buildBaseString($baseURI, $method, $params) {
  $r = array();
  ksort($params);
  foreach ($params as $key => $value) {
    $r[] = "$key=" . rawurlencode($value);
  }

  return $method . "&" . rawurlencode($baseURI) . '&' . rawurlencode(implode('&', $r)); //return complete base string
}

/**
 * Build authorization header.
 * @param type $oauth
 * @return type
 */
function buildAuthorizationHeader($oauth) {
  $r = 'Authorization: OAuth ';
  $values = array();
  foreach ($oauth as $key => $value)
    $values[] = "$key=\"" . rawurlencode($value) . "\"";

  $r .= implode(', ', $values);
  return $r;
}

/**
 * Retrieve the tweets with a authenticated REST call.
 */
function _twitter_db_retrive() {

  $oauth_access_token = variable_get('twitter_db_access_token', NULL);
  $oauth_access_token_secret = variable_get('twitter_db_access_token_secret', NULL);
  $consumer_key = variable_get('twitter_db_consumer_key', NULL);
  $consumer_secret = variable_get('twitter_db_consumer_secret', NULL);

  if (!$oauth_access_token || !$oauth_access_token_secret || !$consumer_key || !$consumer_secret) {
    return FALSE;
  }

  // Define the oauth params
  $oauth = array(
    'count' => 20,
    'oauth_consumer_key' => $consumer_key,
    'oauth_nonce' => time(),
    'oauth_signature_method' => 'HMAC-SHA1',
    'oauth_token' => $oauth_access_token,
    'oauth_timestamp' => time(),
    'oauth_version' => '1.0',
  );

  $what_to_retrieve = variable_get('twitter_db_what_to_retrieve', 'user');
  $retrieve_name = variable_get('twitter_db_retrieve_name', FALSE);

  if (!$retrieve_name) {
    return FALSE;
  }

  $curlopt_url = '';
  switch ($what_to_retrieve) {
    case 'user':
      $url = "https://api.twitter.com/1.1/statuses/user_timeline.json";
      $oauth['screen_name'] = $retrieve_name;
      $curlopt_url = $url . '?screen_name=' . $retrieve_name . '&count=' . $oauth['count'];
      break;

    case 'hashtag':
      $url = "https://api.twitter.com/1.1/search/tweets.json";
      $oauth['q'] = $retrieve_name;
      $oauth['result_type'] = 'recent';
      $curlopt_url = $url . '?q=' . $retrieve_name . '&result_type=' . $oauth['result_type'] . '&count=' . $oauth['count'];
      break;
  }

  $base_info = buildBaseString($url, 'GET', $oauth);
  $composite_key = rawurlencode($consumer_secret) . '&' . rawurlencode($oauth_access_token_secret);
  $oauth_signature = base64_encode(hash_hmac('sha1', $base_info, $composite_key, true));
  $oauth['oauth_signature'] = $oauth_signature;

  $header = array(buildAuthorizationHeader($oauth), 'Expect:');

  if (!$curlopt_url) {
    return FALSE;
  }

  $options = array(
    CURLOPT_HTTPHEADER => $header,
    //CURLOPT_POSTFIELDS => $postfields,
    CURLOPT_HEADER => false,
    CURLOPT_URL => $curlopt_url,
    CURLOPT_RETURNTRANSFER => true, CURLOPT_SSL_VERIFYPEER => false
  );

  $feed = curl_init();
  curl_setopt_array($feed, $options);
  $json = curl_exec($feed);
  curl_close($feed);

  $twitter_data = json_decode($json);

  switch ($what_to_retrieve) {
    case 'user':
      return $twitter_data;
    case 'hashtag':
      return $twitter_data->statuses;
      
  }
  return FALSE;
}

