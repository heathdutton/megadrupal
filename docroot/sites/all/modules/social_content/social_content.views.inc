<?php
/**
 * @file
 * Social Content views.
 */

/**
 * Implements hook_data().
 */
function social_content_views_data() {
  $data['social_content_history']['table']['group']  = t('Social content');
  $data['social_content_history']['table']['join']['node'] = array(
    'field' => 'internal_id',
    'left_field' => 'nid',
  );
  $data['social_content_history']['table']['join']['social_content_instances'] = array(
    'field' => 'instance',
    'left_field' => 'id',
  );
  $data['social_content_history']['instance'] = array(
    'relationship' => array(
        'base' => 'social_content_instances', // The name of the table to join with.
        'base field' => 'id', // The name of the field on the joined table.
        'handler' => 'views_handler_relationship',
        'label' => t('Social Content Instance'),
        'title' => t('Social Content Instance'),
        'help' => t('Social Content Instance'),
      ),
    'title' => 'Instance ID',
    'help' => t('The social account it was imported from.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['social_content_instances']['table']['group']  = t('Social content');
  $data['social_content_instances']['global'] = array(
    'title' => 'Source',
    'help' => t('The source network of the import.'),
    'field' => array(
      'handler' => 'social_content_source_views_handler_field',
      'click sortable' => TRUE
    ),
    'filter' => array(
      'handler' => 'social_content_views_handler_filter_source',
    ),
  );
  return $data;
}
