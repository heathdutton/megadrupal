<?php

/**
 * @file
 * Public page callbacks for the Pay with a Tweet module.
 *
 * @ingroup commerce_pay_with_a_tweet
 */

/**
 * Menu callback. Connect to Twitter to grant access to publish tweet.
 * 
 * @global $base_url
 * 
 * @param object $commerce_pay_with_a_tweet
 *   The commerce_pay_with_a_tweet entity to use.
 */
function commerce_pay_with_a_tweet_twitter_connect($commerce_pay_with_a_tweet, $callback_name = NULL) {
  global $base_url;

  module_load_include('php', 'oauth_common', 'lib/OAuth');

  $_SESSION['oauth_pid'] = $commerce_pay_with_a_tweet->pid;

  $settings = variable_get(PAY_WITH_A_TWEET, _pay_with_a_tweet_config());

  if ((!empty($settings['consumer_key'])) || (!empty($settings['consumer_secret']))) {
    // Build PayWithATweetOAuth object with client credentials.
    $connection = new PayWithATweetOAuth($settings['consumer_key'], $settings['consumer_secret']);

    // Get temporary credentials.
    $url = $base_url . '/pay_with_a_tweet/' . $commerce_pay_with_a_tweet->pid . '/callback';
    if (!empty($callback_name)) {
      $url .= '/' . $callback_name;
    }

    $request_token = $connection->getRequestToken($url);

    // Save temporary credentials to session.
    $_SESSION['oauth_token'] = $token = $request_token['oauth_token'];
    $_SESSION['oauth_token_secret'] = $request_token['oauth_token_secret'];

    $http_code = $connection->getHttpCode();
    // If last connection failed don't display authorization link.
    switch ($http_code) {
      case 200:
        // Build authorize URL and redirect user to Twitter.
        $url = $connection->getAuthorizeURL($token);
        drupal_goto($url, array('external' => TRUE));
        break;

      default:
        // Show notification if something went wrong.
        $watchdog_array = array(
          '%title' => $commerce_pay_with_a_tweet->title . ' [' . $commerce_pay_with_a_tweet->pid . ']',
          '%error' => $http_code, '%message' => $connection->getError(),
        );
        watchdog('Pay with a Tweet', 'Twitter connect error %error in button %title: %message', $watchdog_array, WATCHDOG_CRITICAL, $base_url . '/commerce_pay_with_a_tweet/' . $commerce_pay_with_a_tweet->pid);

        drupal_set_message(t('Could not connect to Twitter. Refresh the page or try again later.'), 'error');
        drupal_goto('<front>');
    }
  }
  else {
    drupal_set_message(t('Could not connect to Twitter. Contact administrator because consumer_key or/and consumer_secret are empty.'), 'error');
    drupal_goto('<front>');
  }
}

/**
 * Menu callback. Publish the tweet and send the download.
 * 
 * @global $base_url
 * 
 * @param object $commerce_pay_with_a_tweet
 *   The commerce_pay_with_a_tweet entity to use.
 */
function commerce_pay_with_a_tweet_twitter_callback($commerce_pay_with_a_tweet, $callback_name = NULL) {
  global $base_url;

  module_load_include('php', 'oauth_common', 'lib/OAuth');

  $_SESSION['oauth_pid'] = $commerce_pay_with_a_tweet->pid;
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
      '%title' => $commerce_pay_with_a_tweet->title . ' [' . $commerce_pay_with_a_tweet->pid . ']',
      '%error' => $http_code,
      '%message' => $connection->getError(),
    );
    watchdog('Pay with a Tweet', 'Twitter callback connect error %error in button %title: %message', $watchdog_array, WATCHDOG_CRITICAL, $base_url . '/commerce_pay_with_a_tweet/' . $commerce_pay_with_a_tweet->pid . '/callback');

    drupal_set_message(t('Could not connect to Twitter. Refresh the page or try again later.'), 'error');
    drupal_goto('<front>');
  }

  $connection->setToken($access_token['oauth_token'], $access_token['oauth_token_secret']);

  // Last order of current user.
  $last_order = commerce_pay_with_a_tweet_get_last_order_for_user();

  // Token replace. Get first product object in order.
  // See commerce_product_tokens() and
  // https://drupal.org/node/390482#drupal7tokenslist
  if (!empty($last_order)) {
    $message = commerce_pay_with_a_tweet_get_message_with_first_product_token_replace($last_order, $commerce_pay_with_a_tweet->message);
  }
  else {
    $message = $commerce_pay_with_a_tweet->message;
  }

  $twitter_user = $connection->getCredentials();
  $connection->publishTweet(check_plain($message));

  // Remove no longer needed request tokens.
  unset($_SESSION['oauth_token']);
  unset($_SESSION['oauth_token_secret']);
  unset($_SESSION['oauth_pid']);

  $http_code = $connection->getHttpCode();
  // If HTTP response is 200 continue otherwise send to connect page to retry.
  if ($http_code == 200) {

    if ((!empty($callback_name)) && ($callback_name == COMMERCE_PAY_WITH_A_TWEET_PAYMENT) && (!empty($last_order))) {
      $last_order->data['commerce_pay_with_a_tweet']['httpcode'] = 200;
      commerce_order_save($last_order);
      module_load_include('inc', 'commerce_pay_with_a_tweet', 'commerce_pay_with_a_tweet.commerce');
      commerce_pay_with_a_tweet_success_transaction($last_order->data['commerce_pay_with_a_tweet']['payment_method'], $last_order, $last_order->data['commerce_pay_with_a_tweet']['charge']);
      commerce_payment_redirect_pane_next_page($last_order);
      drupal_goto('checkout/' . $last_order->order_id);
    }
    else {
      $file = file_load($commerce_pay_with_a_tweet->download);
      $content = file_get_contents(drupal_realpath($file->uri));
      // Show notification if something went wrong.
      $watchdog_array = array(
        '%title' => $commerce_pay_with_a_tweet->title . ' [' . $commerce_pay_with_a_tweet->pid . ']',
        '%screen_name' => '@' . $twitter_user->screen_name,
      );
      watchdog('Pay with a Tweet', 'Tweet publish in %screen_name correctly. Download %title', $watchdog_array, WATCHDOG_INFO, $base_url . '/commerce_pay_with_a_tweet/' . $commerce_pay_with_a_tweet->pid . '/callback');

      header('Content-type: ' . $file->filemime);
      header("Content-Description: File Transfer");
      header("Content-Length: " . $file->filesize);
      header('Content-Disposition: attachment; filename="' . $file->filename . '"');

      print $content;
      drupal_exit();
    }
  }
  else {
    // Show notification if something went wrong.
    $watchdog_array = array(
      '%title' => $commerce_pay_with_a_tweet->title . ' [' . $commerce_pay_with_a_tweet->pid . ']',
      '%error' => $http_code,
      '%message' => $connection->getError(),
    );
    watchdog('Pay with a Tweet', 'Tweet publish error %error in button %title: %message', $watchdog_array, WATCHDOG_CRITICAL, $base_url . '/commerce_pay_with_a_tweet/' . $commerce_pay_with_a_tweet->pid . '/callback');

    drupal_set_message(t('Could not publish in Twitter. Refresh the page or try again later.'), 'error');
    drupal_goto('<front>');
  }
}
