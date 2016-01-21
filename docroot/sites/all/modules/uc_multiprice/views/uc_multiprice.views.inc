<?php

/**
 * Implements hook_views_plugins().
 */
function uc_multiprice_views_plugins() {
  return array(
    'cache' => array(
      'uc_multiprice' => array(
        'path' => drupal_get_path('module', 'uc_multiprice') . '/views',
        'title' => t('UC Multiprice country based'),
        'help' => t('Cache views per uc_multiprice country.'),
        'handler' => 'uc_multiprice_plugin_cache',
        'uses options' => TRUE,
      ),
    ),
  );
}
