<?php

/**
 * Implements hook_views_data()
 */
function views_system_message_views_data() {
  $data['views']['system_message'] = array(
    'title' => t('System message'),
    'help' => t('Provide the message in a view.'),
    'area' => array(
      'handler' => 'views_handler_area_system_message',
    ),
  );

  return $data;
}
