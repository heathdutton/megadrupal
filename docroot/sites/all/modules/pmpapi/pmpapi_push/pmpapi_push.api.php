<?php

/**
 * @file
 * Hooks provided by the PMPAPI push module.
 */


/**
 * Alter information from pmpapi_push_get_entities()
 *
 * @param array $entities
 *   Entity info being altered.
 */
function hook_pmpapi_push_get_entities_alter(&$entities) {
  // This would, for example, hide all user entities from the admin config form
  unset($entities['user']);
}

/**
 * Act on test to determine if entity should be pushed.
 *
 * @param object $entity
 *   Any drupal entity object
 * @param string $type
 *   The type of entity (node, file, etc.)
 */
function hook_pmpapi_push_entity_ok_to_push($entity, $type) {

  // If entity is a node of type 'story', only push if it has a particular program
  // program related to it (via entityreference)

  // In other words, push stories only from a certain show

  $machine_name = 'story'; //machine name of content type
  $field = 'field_program'; // machine name of program entityrefernce field
  $fishin_nid = 2; // nid of "Fishin' with Dave"
  if ($type == 'node' && $entity->type == $machine_name) {
    $language = (isset($entity->language)) ? $entity->language : LANGUAGE_NONE;
    $field_items = field_get_items($type, $entity, $field, $language);
    foreach($field_items as $field_item) {
      if ($field_item['target_id'] == $fishin_nid) {
        return TRUE;
      }
    }
    // If we fall through to here, we can assume it's a story, but not a
    // "Fishin' with Dave" story, so cancel push.
    // NB: If you want to cancel push, MAKE SURE that you return an explicit FALSE
    // (rather than just falling out of the function)
    return FALSE;
  }

  // The default return will likely be TRUE
  return TRUE;
}