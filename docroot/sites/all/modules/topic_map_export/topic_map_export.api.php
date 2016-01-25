<?php

/**
 * @file
 * Hooks provided by the Topic Map Export module.
 */

/**
 * Implement this hook to declare a associations between entities.
 *
 * This hook needs to return one or more association definitions. The
 * definitions are used to describe associations between Drupal entities.
 *
 * @return array
 *   An associative array whose keys define the association structure:
 *   - 'description' (required): This is used in the configuration section as
 *     descriptive label for the association.
 *   - 'requirements' (required): A associative that holds requirements useed
 *     to declare the minimum requirements in order for this association to be
 *     created.
 *     - entities: Required entities. Array with entity machine names in it.
 *     - fields: Required fields. Associative array with the following keys
 *       'field' the field type required and 'module' the required module for
 *       field type.
 *   - 'definitions' (required): An associative array used to describe the
 *     association in topic map. All keys need to be unique. Values are used to
 *     in configuration section and also in topic map as topic names.
 *   - 'class' (required): An associative array containing key value used to
 *     describe the association topic itself.
 *   - 'roles' (required): An associative array describing the roles used in
 *     association.
 *   - 'handler' (required): Processing function used to determine the members
 *     for the association. Please see sample handler below for more
 *     information.
 *   - 'handler_entity_type' (optional): If handler is dependent of certain type
 *     of entity, it's machine name can be declared here. If declared, then
 *     handler will only receive the given type of entity.
 */
function hook_topic_map_export_association_info() {

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
 * Sample handler declared in above association info structure.
 *
 * Association handler functions are expected to return association member
 * groups with minimum of two members.
 *
 * Notice: This is not a hook, so there is no requirement follow the naming
 * convention of the API(altough it is highly recommended).
 *
 * @param mixed $topic
 *   Topic object. See the _topic_map_export_create_topic() for more
 *   information.
 * @param Entity $entity
 *   Fully loaded entity. If the 'handler_entity_type' was defined in the
 *   hook_topic_map_export_association_info hook, then entity will always be of
 *   that type.
 * @param array $field_items
 *   $field_items(optional) are only given if the required field type is
 *   declared in the
 *   info hook.
 *
 * @return array
 *   An array of member groups. If no members can be found in given arguments
 *   then handler should return an empty array.
 *
 * @see hook_topic_map_export_node_association_info()
 */
function _topic_map_export_sample_handler_process_taxonomy_term($topic, $entity, $field_items) {

  $associations = array();

  foreach ($field_items as $key => $field_value) {

    $members = array();

    // The value $topic->type here could also replaced with 'taxonomy-referrer'
    $members[] = array('type' => $topic->type, 'id' => $topic->entity_id);
    $members[] = array('type' => 'taxonomy_term', 'id' => $field_value['tid']);

    $associations[] = $members;
  }

  return $associations;

}

/**
 * Association structures have been defined; modules may now modify structures.
 *
 * Please see the above hook_topic_map_export_node_association_info() for
 * more information.
 *
 * @see hook_topic_map_export_node_association_info()
 */
function hook_topic_map_export_association_info_alter(&$associations) {

  foreach ($associations as $info) {

    if ($info['definitions']['class'] = 'taxonomy-reference') {
      $info['handler'] = "my_module_topic_map_export_taxonomy_reference_custom_handler";
    }
  }

}
