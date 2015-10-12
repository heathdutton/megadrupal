<?php
/**
 * @file
 * Views hook implemented for the Views Comment Closer module.
 */

/**
 * Implements hook_views_data().
 */
function views_commentcloser_views_data() {
  $data = array();
  $data['node']['commentcloser'] = array(
    'group' => t('Content'),
    'field' => array(
      'title' => t('Close/open commenting'),
      'help' => t('Provides a link to close or open commenting on the node.'),
      'handler' => 'views_handler_commentcloser',
    ),
  );
  return $data;
}

