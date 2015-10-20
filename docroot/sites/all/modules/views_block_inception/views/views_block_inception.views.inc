<?php

/**
 * Implementation of hook_views_plugins().
 */
function views_block_inception_views_plugins() {
  $path = drupal_get_path('module', 'views_block_inception');

  return array(
    'row' => array(
      'views_block_inception_block' => array(
        'title' => t('Block row'),
        'help' => t('Displays row in a system block'),
        'handler' => 'views_block_inception_row_block',
        'path' => $path . '/views',
        'theme' => 'views_block_inception_row',
        'theme file' => 'theme.inc',
        'theme path' => $path . '/theme',
        'uses fields' => TRUE,
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      )
    )
  );
}
