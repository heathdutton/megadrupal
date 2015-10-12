<?php
/**
 * @file
 * Association support for references module relationships
 */

/**
 * Implements hook_topic_map_export_association_info().
 */
function references_topic_map_export_association_info() {

  $return = array(
    array(
      'description' => t('Node reference'),
      'requirements' => array(
        'entities' => array('node'),
        'field' => array(
          'module' => 'node_reference',
          'type' => 'node_reference',
        ),
      ),
      'definitions' => array(
        'class' => array('node-reference' => t('Node referred by')),
        'roles' => array(
          'node-referrer' => t('Referrer node'),
          'node-referred' => t('Referred node'),
        ),
      ),
      'handler' => '_topic_map_export_references_process_node_reference',
    ),
  );

  return $return;

}

/**
 * Creates association between entity and the referred node(s).
 *
 * @see _topic_map_export_sample_handler_process_taxonomy_term()
 * @see topic_map_export.api.php
 */
function _topic_map_export_references_process_node_reference($topic, $entity, $field_items) {

  $associations = array();
  foreach ($field_items as $key => $field_value) {

    $members = array();
    $members[] = array('type' => $topic->type, 'id' => $topic->entity_id);
    $members[] = array('type' => 'node', 'id' => $field_value['nid']);

    $associations[] = $members;

  }

  return $associations;

}
