<?php
/**
 * @file
 * mobile_switch.views.inc
 */

/**
 * Implements hook_views_data()
 */
function mobile_switch_views_data() {
  $data['node']['mobile_switch'] = array(
    'group' => t('Mobile Switch'),
    'title' => t('Mobile detection'),
    'help' => t('Display content depending on mobile device detection.'),
    'real field' => 'nid',
    'filter' => array(
      'handler' => 'mobile_switch_handler_filter'
    )
  );

  return $data;
}
