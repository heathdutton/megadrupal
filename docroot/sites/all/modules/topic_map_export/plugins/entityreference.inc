<?php
/**
 * @file
 * Association support for references module relationships
 */

/**
 * Implements hook_topic_map_export_association_info().
 */
function entityreference_topic_map_export_association_info() {

  $return = array(
    array(
      'description' => t('Entity reference'),
      'requirements' => array(
        'field' => array(
          'module' => 'entityreference',
          'type' => 'entityreference',
        ),
      ),
      'definitions' => array(
        'class' => array('entity-reference' => t('Referenced by')),
        'roles' => array(
          'entity-referrer' => t('Referrer entity'),
          'entity-referred' => t('Referred entity'),
        ),
      ),
      'handler' => '_topic_map_export_process_entityreferences',
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
function _topic_map_export_process_entityreferences($topic, $entity, $field_items) {

  $associations = array();

  foreach ($field_items as $key => $field_value) {

    $members = array();
    $members[] = array('type' => $topic->type, 'id' => $topic->entity_id);
    $members[] = array('type' => $field_value['target_type'], 'id' => $field_value['target_id']);
    $associations[] = $members;

  }

  return $associations;

}
