<?php

/**
 * @file
 * Organic Groups (OG) SimpleStats views include.
 */

/**
 * Implements hook_views_data().
 *
 * @todo can the table join work with non-node tables with
 *   og_get_all_group_entity()?
 */
function og_simplestats_views_data_alter(&$data) {
  $data['node']['og_simplestats_node_count'] = array(
    'title' => t('Total Posts'),
    'help' => t('Total number of Nodes posted in the Group.'),
    'group' => t('OG SimpleStats'),
    'field' => array(
      'handler' => 'og_simplestats_handler_field_node_count',
    ),
  );
  $data['node']['og_simplestats_user_count'] = array(
    'title' => t('Total Members'),
    'help' => t('Total number of members in the Group.'),
    'group' => t('OG SimpleStats'),
    'field' => array(
      'handler' => 'og_simplestats_handler_field_user_count',
    ),
  );
}
