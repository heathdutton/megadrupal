<?php
/**
 * @file
 * Implementations of social_counters hooks.
 */

/**
 * Implements hook_social_counters().
 */
function social_counters_social_counters() {
  // Facebook.
  $items['facebook'] = array(
    'callback' => 'social_counters_get_facebook_likes',
    'title' => t('Facebook'),
    'variables' => array(
      'id' => array(
        'title' => t('Facebook Page Name (or User ID)'),
        'description' => t("Either the page name or ID. Examples: 'coca-cola' or '40796308305'."),
      ),
      'token' => array(
        'title' => t('Facebook Access Token'),
        'description' => t('See <a href="@facebook-handbook">Access Tokens on Facebook API page</a> and <a href="@stackoverflow-url">detailed explanation on stackoverflow.com</a>.', array(
          '@facebook-handbook' => url('https://developers.facebook.com/docs/facebook-login/access-tokens'),
          '@stackoverflow-url' => url('http://stackoverflow.com/a/28418469/1391963'),
        )),
      ),
    ),
  );

  // Twitter.
  $items['twitter'] = array(
    'callback' => 'social_counters_get_twitter_followers',
    'title' => t('Twitter'),
    'dependencies' => array(
      'oauth_common' => t('Twitter functionality depends on the <a href="@oauth">OAuth module</a>.', array('@oauth' => url('https://drupal.org/project/oauth'))),
    ),
    'variables' => array(
      'id' => array(
        'title' => t('Twitter account'),
        'description' => t('The Twitter account to pull the number of followers.'),
      ),
      'consumer_key' => array(
        'title' => t('OAuth Consumer Key'),
        'description' => t('OAuth Consumer Key (Twitter).'),
      ),
      'consumer_secret' => array(
        'title' => t('OAuth Consumer Secret'),
        'description' => t('OAuth Consumer Secret (Twitter).'),
      ),
      'token' => array(
        'title' => t('OAuth Token (Twitter).'),
        'description' => t('OAuth Token (Twitter).'),
      ),
      'secret' => array(
        'title' => t('OAuth Secret (Twitter).'),
        'description' => t('OAuth Secret (Twitter).'),
      ),
    ),
  );

  // Google Plus.
  $items['google_plus'] = array(
    'callback' => 'social_counters_get_google_plus_followers',
    'title' => t('Google Plus'),
    'variables' => array(
      'id' => array(
        'title' => t('Google Plus Name (or User ID)'),
        'description' => t("Either the page name or ID. Examples: '+Coca-Cola' or '113050383214450284645'"),
      ),
      'key' => array(
        'title' => t('Google Plus API Key'),
        'description' => t('You can generate Google Plus API Key at <a href="@dev-google">developers.google.com</a>.', array(
          '@dev-google' => url('https://developers.google.com/api-client-library/python/guide/aaa_apikeys'),
        )),
      ),
    ),
  );

  // Vk.com.
  $items['vk'] = array(
    'callback' => 'social_counters_get_vk_members',
    'title' => t('VK'),
    'variables' => array(
      'id' => array(
        'title' => t('VK Group ID'),
        'description' => t('The group ID of a VK Group.'),
      ),
    ),
  );

  // Instagram.
  $items['instagram'] = array(
    'callback' => 'social_counters_get_instagram_followers',
    'title' => t('Instagram'),
    'variables' => array(
      'id' => array(
        'title' => t('Instagram Id'),
        'description' => t('Instagram Id. Pay attention that Instagram username isn\'t valid. Examples: 249655166'),
      ),
      'access_token' => array(
        'title' => t('Access Token'),
        'description' => t('You can read how to generate Instagram Access Token at  <a href="@dev-instagram">instagram.com</a>.', array(
          '@dev-instagram' => url('https://instagram.com/developer/authentication/'),
        )),
      ),
    ),
  );

  // YouTube.
  $items['youtube'] = array(
    'callback' => 'social_counters_get_youtube_subscribers',
    'title' => t('YouTube'),
    'variables' => array(
      'id' => array(
        'title' => t('YouTube username'),
        'description' => t('YouTube username. Examples: cocacola'),
      ),
      'key' => array(
        'title' => t('Google Plus API Key'),
        'description' => t('You can generate Google Plus API Key at <a href="@dev-google">developers.google.com</a>.', array(
          '@dev-google' => url('https://developers.google.com/api-client-library/python/guide/aaa_apikeys'),
        )),
      ),
    ),
  );

  // Flickr.
  $items['flickr'] = array(
    'callback' => 'social_counters_get_flickr_members',
    'title' => t('Flickr'),
    'variables' => array(
      'id' => array(
        'title' => t('Flickr group name'),
        'description' => t('Flickr group name. Examples: thecoca-colaco'),
      ),
      'api_key' => array(
        'title' => t('Flickr API Key'),
        'description' => t('You can generate Flickr API Key at <a href="@dev-flickr">flickr.com</a>.', array(
          '@dev-flickr' => url('https://www.flickr.com/services/apps/create/apply/'),
        )),
      ),
    ),
  );

  return $items;
}

/**
 * Get the number of Facebook likes.
 */
function social_counters_get_facebook_likes($variables) {
  $number = FALSE;

  try {
    $url = url('https://graph.facebook.com/' . $variables['id'], array('query' => array(
      'access_token' => $variables['token'],
      'fields' => 'likes',
    )));
    $response = drupal_http_request($url);
    $result = json_decode($response->data);
    if ($response->code == 200) {
      $number = $result->likes;
    }
    else {
      if (!empty($result->error)) {
        $message = $variables['social_network'] . ': ' . $result->error->message;
        watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}

/**
 * Get the number of Twitter followers.
 */
function social_counters_get_twitter_followers($variables) {
  $number = FALSE;

  try {
    $url = 'https://api.twitter.com/1.1/users/show.json';
    $consumer = new OAuthConsumer($variables['consumer_key'], $variables['consumer_secret'], NULL);
    $token    = new OAuthConsumer($variables['token'], $variables['secret']);
    $params = array('screen_name' => $variables['id']);

    $oauth_request = OAuthRequest::from_consumer_and_token($consumer, $token, 'GET', $url, $params);
    $oauth_request->sign_request(new OAuthSignatureMethod_HMAC_SHA1(), $consumer, $token);

    $response = drupal_http_request($oauth_request->to_url());
    $result = json_decode($response->data);
    if ($response->code == 200) {
      $number = $result->followers_count;
    }
    else {
      if (!empty($result->errors)) {
        foreach ($result->errors as $error) {
          $message = $variables['social_network'] . ': ' . $error->message;
          watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}

/**
 * Get the number of Google Plus followers.
 */
function social_counters_get_google_plus_followers($variables) {
  $number = FALSE;

  try {
    $url = url('https://www.googleapis.com/plus/v1/people/' . $variables['id'], array('query' => array(
      'key' => $variables['key'],
    )));
    $response = drupal_http_request($url);

    $result = json_decode($response->data);
    if ($response->code == 200) {
      $number = $result->circledByCount;
    }
    else {
      $message = $variables['social_network'] . ': ' . $result->error->message;
      watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}

/**
 * Get the number of Vk.com group's members.
 */
function social_counters_get_vk_members($variables) {
  $number = FALSE;

  try {
    $url = url('https://api.vk.com/method/groups.getById', array('query' => array(
      'group_id' => $variables['id'],
      'count' => 20,
      'v' => 5.21,
      'fields' => 'members_count',
    )));
    $response = drupal_http_request($url);
    $result = json_decode($response->data);
    if ($response->code == 200 && !isset($result->error)) {
      $number = current($result->response)->members_count;
    }
    else {
      $message = $variables['social_network'] . ': ' . $response->error->error_msg;
      watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}

/**
 * Get the number of Instagram followers.
 */
function social_counters_get_instagram_followers($variables) {
  $number = FALSE;

  try {
    $url = url('https://api.instagram.com/v1/users/' . $variables['id'], array('query' => array(
      'access_token' => $variables['access_token'],
    )));
    $response = drupal_http_request($url);
    $result = json_decode($response->data);
    if ($result->meta->code == 200) {
      $number = $result->data->counts->followed_by;
    }
    else {
      $message = $variables['social_network'] . ': ' . $result->meta->error_message;
      watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}

/**
 * Get the number of Youtube subscribers.
 */
function social_counters_get_youtube_subscribers($variables) {
  $number = FALSE;

  try {
    $url = url('https://www.googleapis.com/youtube/v3/channels', array('query' => array(
      'forUsername' => $variables['id'],
      'key' => $variables['key'],
      'part' => 'statistics',
    )));
    $response = drupal_http_request($url);
    $result = json_decode($response->data);
    if (empty($result->error)) {
      $number = current($result->items)->statistics->subscriberCount;
    }
    else {
      foreach ($result->error->errors as $error) {
        $message = $variables['social_network'] . ': ' . $error->message;
        watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}

/**
 * Get the number of Flickr group members.
 */
function social_counters_get_flickr_members($variables) {
  $number = FALSE;

  try {
    $url = url('https://api.flickr.com/services/rest/', array('query' => array(
      'group_path_alias' => $variables['id'],
      'api_key' => $variables['api_key'],
      'method' => 'flickr.groups.getInfo',
      'format' => 'json',
      'nojsoncallback' => 1,
    )));
    $response = drupal_http_request($url);
    $result = json_decode($response->data);
    if ($result->stat == 'ok') {
      $number = $result->group->members->_content;
    }
    else {
      $message = $variables['social_network'] . ': ' . $result->message;
      watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}
