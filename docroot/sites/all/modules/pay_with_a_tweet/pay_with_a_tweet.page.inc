<?php

/**
 * @file
 * Public page callbacks for the Pay with a Tweet module.
 *
 * @ingroup pay_with_a_tweet
 */

/**
 * Menu callback. Connect to Twitter to grant access to publish tweet.
 * 
 * @global $base_url
 * 
 * @param object $pay_with_a_tweet
 *   The pay_with_a_tweet entity to use.
 */
function pay_with_a_tweet_twitter_connect($pay_with_a_tweet) {
  global $base_url;

  module_load_include('php', 'oauth_common', 'lib/OAuth');

  $_SESSION['oauth_pid'] = $pay_with_a_tweet->pid;

  $settings = variable_get(PAY_WITH_A_TWEET, _pay_with_a_tweet_config());

  // Build PayWithATweetOAuth object with client credentials.
  $connection = new PayWithATweetOAuth($settings['consumer_key'], $settings['consumer_secret']);

  // Get temporary credentials.
  $request_token = $connection->getRequestToken($base_url . '/pay_with_a_tweet/' . $pay_with_a_tweet->pid . '/callback');

  // Save temporary credentials to session.
  $_SESSION['oauth_token'] = $token = $request_token['oauth_token'];
  $_SESSION['oauth_token_secret'] = $request_token['oauth_token_secret'];

  $http_code = $connection->getHttpCode();
  // If last connection failed don't display authorization link.
  switch ($http_code) {
    case 200:
      // Build authorize URL and redirect user to Twitter.
      $url = $connection->getAuthorizeURL($token);
      header('Location: ' . $url);
      break;

    default:
      // Show notification if something went wrong.
      $watchdog_array = array(
        '%title' => $pay_with_a_tweet->title . ' [' . $pay_with_a_tweet->pid . ']',
        '%error' => $http_code, '%message' => $connection->getError(),
      );
      watchdog('Pay with a Tweet', 'Twitter connect error %error in button %title: %message', $watchdog_array, WATCHDOG_CRITICAL, $base_url . '/pay_with_a_tweet/' . $pay_with_a_tweet->pid);

      drupal_set_message(t('Could not connect to Twitter. Refresh the page or try again later.'), 'error');
      drupal_goto('<front>');
  }
}

/**
 * Menu callback. Publish the tweet and send the download.
 * 
 * @global $base_url
 * 
 * @param object $pay_with_a_tweet
 *   The pay_with_a_tweet entity to use.
 */
function pay_with_a_tweet_twitter_callback($pay_with_a_tweet) {
  global $base_url;

  module_load_include('php', 'oauth_common', 'lib/OAuth');

  $_SESSION['oauth_pid'] = $pay_with_a_tweet->pid;
  $settings = variable_get(PAY_WITH_A_TWEET, _pay_with_a_tweet_config());

  if (!isset($_SESSION['oauth_token'])) {
    drupal_set_message(t('There is a problem with your session. Please, check if the tweet has been published, delete it and retry.'), 'error');
    drupal_goto('<front>');
  }

  // If the oauth_token is old redirect to the connect page.
  if (isset($_REQUEST['oauth_token']) && $_SESSION['oauth_token'] !== $_REQUEST['oauth_token']) {
    unset($_SESSION['oauth_token']);
    unset($_SESSION['oauth_token_secret']);
    drupal_set_message(t('The oauth_token is old'), 'error');
    drupal_goto('<front>');
  }

  // Create PayWithATweetOAuth object with app key/secret and token key/secret
  // from default phase.
  $connection = new PayWithATweetOAuth($settings['consumer_key'], $settings['consumer_secret'], $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);

  // Request access tokens from twitter.
  $access_token = $connection->getAccessToken($_REQUEST['oauth_verifier']);

  $http_code = $connection->getHttpCode();
  if ($http_code != 200) {
    $watchdog_array = array(
      '%title' => $pay_with_a_tweet->title . ' [' . $pay_with_a_tweet->pid . ']',
      '%error' => $connection->httpCode,
      '%message' => $connection->getError(),
    );
    watchdog('Pay with a Tweet', 'Twitter callback connect error %error in button %title: %message', $watchdog_array, WATCHDOG_CRITICAL, $base_url . '/pay_with_a_tweet/' . $pay_with_a_tweet->pid . '/callback');

    drupal_set_message(t('Could not connect to Twitter. Refresh the page or try again later.'), 'error');
    drupal_goto('<front>');
  }

  $connection->setToken($access_token['oauth_token'], $access_token['oauth_token_secret']);

  $twitter_user = $connection->getCredentials();
  $tweet = $connection->publishTweet(check_plain($pay_with_a_tweet->message));

  // Remove no longer needed request tokens.
  unset($_SESSION['oauth_token']);
  unset($_SESSION['oauth_token_secret']);
  unset($_SESSION['oauth_pid']);

  $http_code = $connection->getHttpCode();
  // If HTTP response is 200 continue otherwise send to connect page to retry.
  if ($http_code == 200) {

    $mime = 'application/force-download';
    $file = file_load($pay_with_a_tweet->download);
    $content = file_get_contents(drupal_realpath($file->uri));
    // Show notification if something went wrong.
    $watchdog_array = array(
      '%title' => $pay_with_a_tweet->title . ' [' . $pay_with_a_tweet->pid . ']',
      '%screen_name' => '@' . $twitter_user->screen_name,
    );
    watchdog('Pay with a Tweet', 'Tweet publish in %screen_name correctly. Download %title', $watchdog_array, WATCHDOG_INFO, $base_url . '/pay_with_a_tweet/' . $pay_with_a_tweet->pid . '/callback');

    header('Content-type: ' . $file->filemime);
    header("Content-Description: File Transfer");
    header("Content-Length: " . $file->filesize);
    header('Content-Disposition: attachment; filename="' . $file->filename . '"');

    print $content;
    drupal_exit();
  }
  else {
    // Show notification if something went wrong.
    $watchdog_array = array(
      '%title' => $pay_with_a_tweet->title . ' [' . $pay_with_a_tweet->pid . ']',
      '%error' => $http_code,
      '%message' => $connection->getError(),
    );
    watchdog('Pay with a Tweet', 'Tweet publish error %error in button %title: %message', $watchdog_array, WATCHDOG_CRITICAL, $base_url . '/pay_with_a_tweet/' . $pay_with_a_tweet->pid . '/callback');

    drupal_set_message(t('Could not publish in Twitter. Refresh the page or try again later.'), 'error');
    drupal_goto('<front>');
  }
}
