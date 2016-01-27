<?php

/**
 * @file
 * Provides views handlers.
 */

/**
 * Implements hook_views_data().
 */
function views_now_views_data() {
  $data = array();
  $data['views']['views_now'] = array(
    'title' => t('Current Timestamp'),
    'help' => t('Dislpay the current server timestamp.'),
    'field' => array(
      'help' => t('Display the current server timestamp.'),
      'handler' => 'views_now_handler_field',
      'notafield' => TRUE,
    ),
  );
  return $data;
}


