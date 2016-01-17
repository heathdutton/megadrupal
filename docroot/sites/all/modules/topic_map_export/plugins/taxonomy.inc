<?php
/**
 * @file
 * Association support for taxonomy module relationships
 */

/**
 * Implements hook_topic_map_export_association_info().
 */
function taxonomy_topic_map_export_association_info() {

  $return = array(
    array(
      'description' => t('Taxonomy term reference'),
      'requirements' => array(
        'entities' => array('taxonomy_term'),
        'field' => array(
          'module' => 'taxonomy',
          'type' => 'taxonomy_term_reference',
        ),
      ),
      'definitions' => array(
        'class' => array('taxonomy-reference' => t('Term referred by')),
        'roles' => array(
          'taxonomy-referrer' => t('Referrer entity'),
          'taxonomy-referred' => t('Referred term'),
        ),
      ),
      'handler' => '_topic_map_export_process_taxonomy_term_reference',
    ),
  );

  return $return;

}

/**
 * Creates association between entity and it's terms.
 *
 * @see _topic_map_export_sample_handler_process_taxonomy_term()
 * @see topic_map_export.api.php
 */
function _topic_map_export_process_taxonomy_term_reference($topic, $entity, $field_items) {

  $associations = array();

  foreach ($field_items as $key => $field_value) {

    $members = array();
    $members[] = array('type' => $topic->type, 'id' => $topic->entity_id);
    $members[] = array('type' => 'taxonomy_term', 'id' => $field_value['tid']);

    $associations[] = $members;
  }

  return $associations;

}
