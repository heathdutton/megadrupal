<?php

/**
 * @file
 * Internals functions and callbacks.
 */

/**
 * Sending process.
 *
 * @param int $nid
 *   Node nid.
 * @param bool $goto
 *   If we need to do a drupal_goto after call this function.
 */
function twitter_queue_send($nid, $goto = TRUE) {
  libraries_load('tmhOAuth');
  module_load_include('php', 'twitter_queue', 'class/TwitterQueue');
  $test = !variable_get('twitter_queue_test', 0) ? '' : '_test';

  $settings = array(
    'consumer_key'    => variable_get('twitter_queue_consumer_key' . $test, ''),
    'consumer_secret' => variable_get('twitter_queue_consumer_secret' . $test, ''),
    'user_token'      => variable_get('twitter_queue_user_token' . $test, ''),
    'user_secret'     => variable_get('twitter_queue_user_secret' . $test, ''),
  );

  $twitter_queue = new TwitterQueue($settings);



  $status = db_select('twitter_queue', 'tq')
    ->fields('tq', array('text_status'))
    ->condition('nid', $nid)
    ->execute()
    ->fetchField();

  $response = $twitter_queue->create($status);

  if ($response['code'] === 200) {
    $message = "The tweet id: @id was successfully sended." . PHP_EOL . '<br>';
    $message .= "The status sended was: @status";
    watchdog('twitter_queue', $message, array('@id' => $nid, '@status' => $status));

    db_delete('twitter_queue')
      ->condition('nid', $nid)
      ->execute();

    if (is_numeric($nid)) {
      db_update('twitter_queue_status')
        ->fields(array(
          'status' => 0,
        ))
        ->execute();
    }

    // We want some redirect?
    if ($goto) {
      drupal_set_message(t('Successfully twetted'));
      drupal_goto('admin/structure/twitter-queue');
    }
    return TRUE;
  }
  else {
    $error_message = drupal_json_decode($response['response']);
    $error_message = $error_message['errors'][0];

    drupal_set_message(t('Some error accour, maybe this tweet was already sended. @code', array('@code' => $response['code'])), 'error');
    watchdog('twitter_queue', 'Twitter queue: Request error: @code ' . PHP_EOL . '<br>' . 'Error message: @error_message. Error Code: @error_code',
      array(
        '@code' => $response['code'],
        '@error_message' => $error_message['message'],
        '@error_code' => $error_message['code'],
      ));

    drupal_goto('admin/structure/twitter-queue');

    return FALSE;
  }
}

/**
 * Live send twitter from the form live tweet.
 */
function twitter_queue_send_live($status) {
  libraries_load('tmhOAuth');
  module_load_include('php', 'twitter_queue', 'class/TwitterQueue');

  $test = !variable_get('twitter_queue_test', 0) ? '' : '_test';

  $settings = array(
    'consumer_key'        => variable_get('twitter_queue_consumer_key' . $test, ''),
    'consumer_secret'     => variable_get('twitter_queue_consumer_secret' . $test, ''),
    'user_token'          => variable_get('twitter_queue_user_token' . $test, ''),
    'user_secret'         => variable_get('twitter_queue_user_secret' . $test, ''),
  );

  $twitter_queue = new TwitterQueue($settings);

  $response = $twitter_queue->create($status);

  if ($response['code'] === 200) {
    drupal_set_message(t('Tweet sended successfully'));
  }
  else {
    $error_message = drupal_json_decode($response['response']);
    $error_message = $error_message['errors'][0];

    drupal_set_message(t('Some error accour, maybe this tweet was already sended.'), 'error');
    watchdog('twitter_queue', 'Twitter queue: Request error: @code ' . PHP_EOL . '<br>' . 'Error message: @error_message. Error Code: @error_code',
      array(
        '@code' => $response['code'],
        '@error_message' => $error_message['message'],
        '@error_code' => $error_message['code'],
      ));
  }

  drupal_goto('admin/structure/twitter-queue');
}

/**
 * Internal function to handle the insert and update process of a node.
 *
 * @param obj $node
 *   Node object
 */
function _twitter_queue_ins_upd($node) {
  $array_types = variable_get('twitter_queue_types', array());
  $holder = "twitter_queue_{$node->type}_status";

  if (in_array($node->type, $array_types) && $node->{$holder}) {

    db_merge('twitter_queue')
    ->key(array('nid' => $node->nid))
    ->fields(array(
        'weight'      => -20,
        'node_status' => $node->status,
        'text_status' => $node->title,
      ))
    ->updateFields(array(
        'node_status' => $node->status,
        'text_status' => $node->title,
      ))
    ->execute();

    db_merge('twitter_queue_status')
    ->key(array('nid' => $node->nid))
    ->fields(array(
        'status' => $node->{$holder},
      ))
    ->execute();

  }
}
