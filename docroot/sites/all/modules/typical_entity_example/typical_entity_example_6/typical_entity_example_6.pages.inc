<?php

/**
 * @file
 * Callbacks for viewing entities.
 */

/**
 * Entity view callback.
 * 
 * @param object $entity
 *   Entity to view.
 * 
 * @return array
 *   Renderable array.
 */
function typical_entity_example_6_view($entity) {
  drupal_set_title(entity_label('typical_entity_example_6', $entity));

  // Return automatically generated view page.
  return entity_view('typical_entity_example_6', array(entity_id('typical_entity_example_6', $entity) => $entity));
}
