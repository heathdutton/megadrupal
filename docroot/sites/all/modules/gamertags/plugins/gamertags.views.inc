<?php
/**
 * @file
 * Views specific include file for implementing Gamertags in Views.
 */

/**
 * Implements hook_views_handlers().
 */
function gamertags_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'gamertags') . '/plugins',
    ),
    'handlers' => array(
      'views_handler_field_user_gamertags_all' => array(
        'parent' => 'views_handler_field',
        'file' => 'views_handler_field_user_gamertags.inc',
      ),
      'views_handler_field_user_gamertags_single' => array(
        'parent' => 'views_handler_field',
        'file' => 'views_handler_field_user_gamertags.inc',
      ),
    )
  );
}

/**
 * Implements hook_views_data_alter().
 */
function gamertags_views_data_alter(&$data) {
  $data['users']['gamertags'] = array(
    'real field' => 'data',
    'field' => array(
      'title' => t('Gamertags'),
      'help' => t("All the user's Gamertags."),
      'handler' => 'views_handler_field_user_gamertags_all',
      'click sortable' => FALSE,
    ),
  );
  foreach (gamertags_return_all() as $class => $platform_info) {
    if (variable_get("{$class}_collect", TRUE)) {
      $data['users'][$class] = array(
        'real field' => 'data',
        'field' => array(
          'title' => $platform_info['title'],
          'help' => $platform_info['description'],
          'handler' => 'views_handler_field_user_gamertags_single',
          'click sortable' => FALSE,
        ),
      );
    }
  }
}
