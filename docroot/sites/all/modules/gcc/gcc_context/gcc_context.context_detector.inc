<?php

/**
 * @file
 * TODO.
 */

/**
 * Determine context by checking if the current entity is a group.
 */
function gcc_context_gcc_context_group_detector($entity_type, $entity) {

  if (gcc_is_group($entity)) {

    $entity_id = gcc_get_entity_id($entity_type, $entity);

    return array(
      array(

        'entity_type' => $entity_type,
        'entity_id' => $entity_id,
        'type' => GCC_CONTEXT_DIRECT,
      ),
    );
  }
}

/**
 * Determine context by checking if the current entity is a group content.
 */
function gcc_context_gcc_context_group_content_detector($entity_type, $entity) {

  if (gcc_is_group_content($entity)) {

    $groups = array();
    foreach ($entity->{GCC_FIELD_AUDIENCE}[LANGUAGE_NONE] as $item) {
      $groups[] = array(

        'entity_type' => $item['entity_type'],
        'entity_id' => $item['entity_id'],
        'type' => GCC_CONTEXT_INDIRECT,
      );
    }

    return $groups;
  }
}
