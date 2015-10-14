<?php

/**
 * @file
 *  Views hooks file.
 */

/**
 * Implements hook_views_data_alter().
 */
function boolean_icon_views_data_alter(&$data) {
  // Node status icon.
  $data['node']['published_graphic'] = array(
    'title' => t('Published (icon)'),
    'help' => t('Whether or not the content is published (icon representation).'),
    'real field' => 'status',
    'field' => array(
      'handler' => 'boolean_icon_handler_field_boolean_graphic',
      'click sortable' => TRUE,
    ),
  );

  // Comment status icon.
  $data['comment']['status_graphic'] = array(
    'title' => t('Approved (icon)'),
    'help' => t('Whether the comment is approved (or still in the moderation queue) - icon representation.'),
    'real field' => 'status',
    'field' => array(
      'handler' => 'boolean_icon_handler_field_boolean_graphic',
      'click sortable' => TRUE,
    ),
  );
}