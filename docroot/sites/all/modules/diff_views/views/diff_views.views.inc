<?php

/**
 * @file
 * Views hooks implementations.
 */

/**
 * Implements hook_views_data_alter().
 */
function diff_views_views_data_alter(&$data) {
  // TODO: add support for all revisionable entities.
  $data['node_revision']['diff_views_radio'] = array(
    'title' => t('Compare'),
    'group' => t('Content revision'),
    'help' => t('Provide a radio button to select revision for Diff.'),
    'real field' => 'vid',
    'field' => array(
      'handler' => 'diff_views_handler_field_diff_radio',
      'click sortable' => FALSE,
    ),
  );
}
