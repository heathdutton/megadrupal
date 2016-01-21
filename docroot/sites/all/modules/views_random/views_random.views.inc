<?php

/**
 * Implements hook_views_plugins().
 */
function views_random_views_plugins() {
  $plugins = array(
    'pager' => array(
      'views_random' => array(
        'title' => t('Display randomly'),
        'help' => t('Display random items selected with JavaScript.'),
        'handler' => 'views_random_pager',
        'uses options' => TRUE,
        'type' => 'basic',
      ),
    ),
  );

  return $plugins;
}
