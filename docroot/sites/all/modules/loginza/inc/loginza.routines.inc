<?php
/**
 * @file loginza.routines.inc
 *    General routines.
 */

function loginza_providers_list($default = FALSE) {
  $providers = array(
    'google' => 'Google Account',
    'yandex' => 'Yandex',
    'mailruapi' => 'Mail.ru API',
    'mailru' => 'Mail.ru',
    'vkontakte' => 'Vkontakte',
    'facebook' => 'Facebook',
    'twitter' => 'Twitter',
    'loginza' => 'Loginza',
    'myopenid' => 'MyOpenID',
    'webmoney' => 'WebMoney',
    'rambler' => 'Rambler',
    'flickr' => 'Flickr',
    'lastfm' => 'Last.fm',
    'verisign' => 'Verisign',
    'aol' => 'AOL',
    'steam' => 'Steam',
    'openid' => 'OpenID',
  );

  $titles = variable_get('loginza_providers_titles', array());
  if (!$default && !empty($titles)) {
    foreach (array_keys($providers) as $p) {
      if (!empty($titles[$p])) $providers[$p] = $titles[$p];
    }
  }

  return $providers;
}

function loginza_widgets_images() {
  $custom = variable_get('loginza_widgets_images', array());

  $default = array();
  foreach (array_keys(loginza_providers_list(TRUE)) as $provider) {
    $default[$provider] = sprintf('http://loginza.ru/img/widget/%s_ico.gif', $provider);
  }

  $icons = array(
    'button_widget' => !empty($custom['button_widget'])
        ? $custom['button_widget'] : 'http://loginza.ru/img/sign_in_button_gray.gif',
  );
  foreach (array_keys(loginza_providers_list(TRUE)) as $provider) {
    $icons[$provider] = !empty($custom[$provider]) ? $custom[$provider] : $default[$provider];
  }

  return $icons;
}

function loginza_save_identity_data(array $data) {
  db_merge('loginza_identities')
      ->key(array('identity' => $data['identity']))
      ->fields(array('provider' => $data['provider'], 'data' => drupal_json_encode($data)))
      ->execute();
}

function _loginza_username($data) {
  $name = '';
  if (isset($data['name'])) {
    if (!empty($data['name']['full_name'])) {
      $name = $data['name']['full_name'];
    }
    else {
      $name_parts = array();
      if (!empty($data['name']['first_name'])) {
        $name_parts[] = $data['name']['first_name'];
      }
      if (!empty($data['name']['last_name'])) {
        $name_parts[] = $data['name']['last_name'];
      }
      $name = trim(implode(' ', $name_parts));
    }
  }

  if (empty($name)) {
    $name = empty($data['nickname']) ? variable_get('loginza_default_name', 'user@loginza') : $data['nickname'];
  }

  // check duplicate user name
  $base_name = $name;
  while (TRUE) {
    $uid = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $name))->fetchField();
    if (empty($uid)) break;
    $name = $base_name . uniqid();
  }

  return $name;
}

function _loginza_fake_email() {
  $mail = '';
  $site_domain = parse_url(url('<front>', array('absolute' => TRUE)), PHP_URL_HOST);
  // Drupal does not validate emails like loginza-123@localhost
  if ($site_domain == 'localhost') $site_domain .= '.com';
  while (TRUE) {
    $mail = 'loginza-' . uniqid() . '@' . $site_domain;
    $uid = db_query("SELECT uid FROM {users} WHERE mail = :mail", array(':mail' => $mail))->fetchField();
    if (empty($uid)) break;
  }
  return $mail;
}

function _loginza_param_token_url($token_url = NULL, $destination = NULL) {
  if (empty($destination)) {
    $destination = drupal_get_destination();
    $amnesia_paths = variable_get('loginza_amnesia_paths', "user/register\r\nuser");
    if (drupal_match_path($destination['destination'], $amnesia_paths)) $destination = array();
  }
  else {
    $destination = array('destination' => $destination);
  }

  if (empty($token_url)) $token_url = url('loginza/token_url', array('absolute' => TRUE, 'query' => $destination));
  $token_url = urlencode($token_url);
  return $token_url;
}

function _loginza_param_providers($providers_set, $provider) {
  $all_providers = loginza_providers_list();
  $params = array();

  $providers_list = empty($providers_set)
      ? array_filter(variable_get('loginza_providers_set', array()))
      : $providers_set;
  if (!empty($providers_list)) {
    $set = array();
    foreach ($providers_list as $p) {
      if (!empty($all_providers[$p])) $set[] = $p;
    }
    $params[] = 'providers_set=' . implode(',', $set);
  }

  $provider = empty($provider) ? variable_get('loginza_default_provider', '') : $provider;
  if (!empty($provider) && !empty($all_providers[$provider])) {
    $params[] = 'provider=' . $provider;
  }

  return empty($params) ? '' : (implode('&', $params) . '&');
}

function _loginza_param_lang($lang = NULL) {
  $all_langs = array('en', 'ru', 'uk');

  if (empty($lang) || !in_array($lang, $all_langs)) {
    global $language;
    $lang = in_array($language->language, $all_langs) ? $language->language : 'en';
  }

  return $lang;
}
