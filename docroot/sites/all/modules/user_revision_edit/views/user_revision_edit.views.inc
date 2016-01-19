<?php

/**
 * @file
 * Provide views data and handlers for user_revision_edit.module.
 */

/**
 * Implements hook_views_data().
 */
function user_revision_edit_views_data() {

  // Adds edit revision link.
  $data['user_revision']['edit_revision'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the revision.'),
      'handler' => 'UserRevisionEditLink',
    ),
  );
  return $data;
}
