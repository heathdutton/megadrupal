<?php

/**
 * @file
 * Drush interface to Yandex XML functionalities.
 */

/**
 * Implements hook_drush_command().
 */
function yandex_xml_drush_command() {
  $commands['yandex-xml-search'] = array(
    'description' => 'Send query to the Yandex search engine.',
    'arguments' => array(
      'search query' => 'Search query',
    ),
    'examples' => array(
      'drush yxs \'Drupal\'  --site-url=http://example.com/articles --limit=10' => 'Find 10 articles with "Drupal" word on example.com.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'site-url' => 'Website to restrict the search area.',
      'limit' => 'The maximum number of posts displayed on overview search results pages.',
      'page' => 'Current search results page',
    ),
    'aliases' => array('yxs'),
  );
  return $commands;
}

/**
 * Implements drush_validate().
 */
function drush_yandex_xml_search_validate() {
  if (!drush_get_option('site-url') || ((variable_get('yandex_xml_site_autodetect') && !drush_get_option('uri')))) {
    return drush_set_error(dt('You must specify this site\'s URL using the --uri parameter.'));
  }
}

/**
 * Callback for command yandex-xml-search.
 */
function drush_yandex_xml_search($keys) {

  try {
    $search = new YandexXml(variable_get('yandex_xml_search_url'));
    $search->query($keys);
    if (!$page = drush_get_option('page')) {
      $page = 0;
    }
    $search->page($page);
    if (!$site_url = drush_get_option('site-url')) {
      $site_url = variable_get('yandex_xml_site_autodetect') ? $GLOBALS['base_url'] : variable_get('yandex_xml_site_url');
    }
    $search->site($site_url);
    if (!$limit = drush_get_option('limit')) {
      $limit = variable_get('yandex_xml_grpoups_on_page');
    }
    $search->limit($limit);
    timer_start('query_execution_time');
    $search->send();
    $execution_time = timer_read('query_execution_time');
  }
  catch (Exception $e) {
    $request_failed = TRUE;
    watchdog('yandex_xml', $e->getMessage(), NULL, WATCHDOG_ERROR);
  }

  if (isset($request_failed)) {
    drupal_set_message(dt('Service temporarily unavailable. Please contact the site administrator.'), 'error');
  }
  elseif ($search->error) {
    // Should we move yandex_xml_error_messages() to yandex_xml.module file?
    require 'yandex_xml.pages.inc';
    $error_message = yandex_xml_error_messages($search->error);

    // "no results" message.
    if ($search->error == 15) {
      drush_print($error_message);
    }
    else {
      drupal_set_message(t('Service temporarily unavailable. Please contact the site administrator.'), 'error');
      watchdog('yandex_xml', $error_message, NULL, WATCHDOG_ERROR);
    }
  }
  else {
    drush_print(dt('Yandex found !total answers.', array('!total' => $search->total())));

    foreach ($search->results() as $index => $result) {
      drush_print(str_repeat('-', 80));
      drush_print('#' . ($page * $limit + $index + 1));
      drush_print(dt('Title: !title', array('!title' => strip_tags($result->title->asXML()))), 4);
      if ($result->passages) {
        drush_print(dt('Passages: !passages', array('!passages' => strip_tags($result->passages->asXML()))), 4);
      }
      drush_print(dt('URL: !url', array('!url' => (string) $result->url)), 4);
    }

    drush_print(str_repeat('-', 80));
    drush_print(dt('Query execution time: !execution_time ms.', array('!execution_time' => $execution_time)));

  }

}
