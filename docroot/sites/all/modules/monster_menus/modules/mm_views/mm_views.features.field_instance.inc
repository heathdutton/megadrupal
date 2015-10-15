<?php
/**
 * @file
 * mm_views.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function mm_views_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-view_node-body'
  $field_instances['node-view_node-body'] = array(
    'bundle' => 'view_node',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A node that just contains a view, for display on this page in the MM tree.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'trim_length' => 600,
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
      'upcoming_events' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => 0,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => TRUE,
        'mm_cck_users' => array(),
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-view_node-field_node_view'
  $field_instances['node-view_node-field_node_view'] = array(
    'bundle' => 'view_node',
    'default_value' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'viewfield',
        'settings' => array(),
        'type' => 'viewfield_default',
        'weight' => 31,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'viewfield',
        'settings' => array(),
        'type' => 'viewfield_default',
        'weight' => 31,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'hidden',
        'module' => 'viewfield',
        'settings' => array(),
        'type' => 'viewfield_default',
        'weight' => 31,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'viewfield',
        'settings' => array(),
        'type' => 'viewfield_default',
        'weight' => 31,
      ),
      'upcoming_events' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_node_view',
    'label' => 'View to Display',
    'required' => FALSE,
    'settings' => array(
      'allowed_views' => array(),
      'force_default' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 31,
    'widget' => array(
      'active' => 1,
      'module' => 'viewfield',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => TRUE,
        'mm_cck_users' => array(),
      ),
      'type' => 'viewfield_select',
      'weight' => 31,
    ),
    'widget_type' => 'viewfield_select',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A node that just contains a view, for display on this page in the MM tree.');
  t('Body');
  t('View to Display');

  return $field_instances;
}
