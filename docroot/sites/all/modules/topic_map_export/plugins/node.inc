<?php
/**
 * @file
 * Association support for node module relationships
 */

/**
 * Implements hook_topic_map_export_association_info().
 */
function node_topic_map_export_association_info() {

  $return = array(
    array(
      'description' => t('Node created by User'),
      'requirements' => array(
        'entities' => array('node', 'user'),
      ),
      'definitions' => array(
        'class' => array('node-created-by' => t('Created by')),
        'roles' => array(
          'creation' => t('Creation'),
          'creator' => t('Creator'),
        ),
      ),
      'handler' => '_topic_map_export_node_process_user_node_creation',
      'handler_entity_type' => 'node',
    ),
  );

  return $return;

}


/**
 * Creates assocation for user -> node creation type relationship.
 *
 * @see _topic_map_export_sample_handler_process_taxonomy_term()
 * @see topic_map_export.api.php
 */
function _topic_map_export_node_process_user_node_creation($topic, $node) {

  $members = array();
  $members[] = array('type' => 'node', 'id' => $topic->entity_id);
  $members[] = array('type' => 'user', 'id' => $node->uid);

  return array($members);

}
