<?php
/**
 * @file
 * Helper functions for the Yandex.Share module.
 */

/**
 * Returns all services with supported features.
 */
function yashare_services_list() {
  return array(
    'blogger' => array(
      'name' => 'Blogger',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'delicious' => array(
      'name' => 'Delicious',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'diary' => array(
      'name' => 'Дневники',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'digg' => array(
      'name' => 'Digg',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'evernote' => array(
      'name' => 'Evernote',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'facebook' => array(
      'name' => 'Facebook',
      'dark' => TRUE,
      'counter' => TRUE,
    ),
    'friendfeed' => array(
      'name' => 'FriendFeed',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'gplus' => array(
      'name' => 'Google+',
      'dark' => FALSE,
      'counter' => TRUE,
    ),
    'juick' => array(
      'name' => 'Juick',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'liveinternet' => array(
      'name' => 'LiveInternet',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'linkedin' => array(
      'name' => 'LinkedIn',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'lj' => array(
      'name' => 'LiveJournal',
      'dark' => TRUE,
      'counter' => FALSE,
    ),
    'moikrug' => array(
      'name' => 'Мой Круг',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'moimir' => array(
      'name' => 'Мой Мир',
      'dark' => FALSE,
      'counter' => TRUE,
    ),
    'myspace' => array(
      'name' => 'MySpace',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'odnoklassniki' => array(
      'name' => 'Одноклассники',
      'dark' => TRUE,
      'counter' => TRUE,
    ),
    'surfingbird' => array(
      'name' => 'Surfingbird',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'tutby' => array(
      'name' => 'Я тут!',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
    'twitter' => array(
      'name' => 'Twitter',
      'dark' => TRUE,
      'counter' => TRUE,
    ),
    'vkontakte' => array(
      'name' => 'VK.com',
      'dark' => TRUE,
      'counter' => TRUE,
    ),
    'yaru' => array(
      'name' => 'Я.Ру',
      'dark' => TRUE,
      'counter' => TRUE,
    ),
    'yazakladki' => array(
      'name' => 'Яндекс.Закладки',
      'dark' => FALSE,
      'counter' => FALSE,
    ),
  );
}

/**
 * Returns default services.
 */
function yashare_get_default_services() {
  return drupal_map_assoc(array('yaru', 'vkontakte', 'twitter', 'facebook', 'lj', 'moimir', 'friendfeed', 'odnoklassniki'));
}

/**
 * Returns enabled services.
 */
function yashare_get_enabled_services($type = 'block', $theme = 'default') {
  $services = array_filter(variable_get('yashare_' . $type . '_services', yashare_get_default_services()));
  if ($theme != 'default') {
    foreach (yashare_services_list() as $codename => $service) {
      if (!$service[$theme]) {
        unset($services[$codename]);
      }
    }
  }
  return $services;
}

/**
 * Returns supported languages.
 */
function yashare_get_languages() {
  return array(
    'auto' => t("User's language"),
    'az' => t('Azerbaijani'),
    'be' => t('Belorussian'),
    'en' => t('English'),
    'hy' => t('Armenian'),
    'ka' => t('Georgian'),
    'kk' => t('Kazakh'),
    'ro' => t('Romanian'),
    'ru' => t('Russian'),
    'tr' => t('Turkish'),
    'tt' => t('Tatar'),
    'uk' => t('Ukrainian'),
  );
}

/**
 * Returns selected language or protected language for API.
 */
function yashare_get_language($protect = FALSE) {
  $lang = variable_get('yashare_l10n', 'auto');
  if (!$protect) {
    return $lang;
  }

  $langs = yashare_get_languages();
  if ($lang == 'auto') {
    global $language;
    $lang = $language->language;
    if (!array_key_exists($lang, $langs)) {
      $lang = 'ru';
    }
  }
  return $lang;
}
